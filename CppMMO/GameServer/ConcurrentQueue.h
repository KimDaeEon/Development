#pragma once
#include <mutex>

namespace Archive
{
	template<typename T>
	class LockQueue
	{
	public:
		LockQueue() {}

		LockQueue(const LockQueue&) = delete;
		LockQueue& operator=(const LockQueue&) = delete;

		void Push(T value)
		{
			lock_guard<mutex> lock(_mutex);
			_queue.push(std::move(value));
			_condVar.notify_one();
		}

		bool TryPop(T& value)
		{
			lock_guard<mutex> lock(_mutex);
			if (_queue.empty())
			{
				return false; // pop 실패했음을 알려준다.
			}

			value = std::move(_queue.front());
			_queue.pop();

			return true; // pop 성공했음을 알려준다.
		}

		// 위의 TryPop 함수처럼 무한 루프를 돌면서 확인하는 것이 아니라, Queue의 내용물이 없으면 Wait하는 Pop 함수.
		void WaitPop(T& value)
		{
			unique_lock<mutex> lock(_mutex);
			_condVar.wait(lock, [this] { return _queue.empty() == false; }); // 람다 캡쳐 [this]가 없으면 멤버 변수에 접근이 불가능하다.
			value = std::move(_queue.front());
			_queue.pop();
		}

	private:
		queue<T> _queue;
		mutex _mutex;
		condition_variable _condVar;
	};

	// Stack보다 어려운 것이 stack은 Pop할 때에만 경합이 일어나서 주의하면 되었는데, Queue같은 경우는 push와 pop 모두에서 경합이 발생할 수 있다.
	// 간단히 표현하자면 Stack같은 경우는 _head 값이 변경될 때에만 주의하면 되었는데, Queue는 _head와 tail이 변경될 때를 모두 주의해야 한다.
	// 그래서 정말 어려운데, 성능도 lock과 별 차이가 없거나 나쁜 경우도 있다고 하여서 이런 것이 있다는 것만 알아두자.
	template<typename T>
	class LockFreeQueue
	{
		struct Node;

		struct CountedNodePtr
		{
			int32 externalCount;
			Node* ptr = nullptr;
		};

		// 아래에서 비트를 32비트만 쓰도록 하여서 NodeCounter는 32비트의 구조체가 된다.
		struct NodeCounter
		{
			uint32 internalCount : 30; // 참조권 반환 관련

			// Push & Pop 다중 참조권 관련, Push Pop 할 때마다 1씩 감소
			// 0이 되면 완전히 Queue에서 꺼내진 것이므로 메모리 해제를 진행한다.
			uint32 externalCountRemaining : 2;
		};

		struct Node
		{
			Node()
			{
				NodeCounter newCount;
				newCount.internalCount = 0;
				newCount.externalCountRemaining = 2;
				count.store(newCount);

				next.ptr = nullptr;
				next.externalCount = 0;
			}

			void ReleaseRef()
			{
				NodeCounter oldCounter = count.load();

				while (true)
				{
					NodeCounter newCounter = oldCounter;
					newCounter.internalCount--;

					if (count.compare_exchange_strong(oldCounter, newCounter))
					{
						if (newCounter.internalCount == 0 && newCounter.externalCountRemaining == 0)
						{
							delete this;
						}
						break;
					}
				}
			}

			atomic<T*> data;
			atomic<NodeCounter> count;
			CountedNodePtr next;
		};

	public:
		// 생성자에서 더미노드를 생성해서 _head와 _tail이 가리킨다. 데이터가 있냐 없냐를 _head == _tail로 판단한다.
		LockFreeQueue()
		{
			CountedNodePtr node;
			node.ptr = new Node;
			node.externalCount = 1;

			_head.store(node);
			_tail.store(node);
		}
		LockFreeQueue(const LockFreeQueue&) = delete;
		LockFreeQueue& operator=(const LockFreeQueue&) = delete;

		void Push(const T& value)
		{
			// 데이터 하나가 추가되면, 데이터가 들어가고 더미 노드도 1개 추가된다. 항상 더미 노드가 존재하게 된다.
			unique_ptr<T> newData = make_unique<T>(value);
			CountedNodePtr dummy;
			dummy.ptr = new Node;
			dummy.externalCount = 1;

			CountedNodePtr oldTail = _tail.load();

			while (true)
			{
				// 참조권 획득 (externalCount를 현시점 기준 +1한 스레드가 획득)
				IncreaseExternalCount(_tail, oldTail);

				// 소유권 획득 (data를 먼저 교환한 스레드가 획득)
				T* oldData = nullptr;

				// 딴 스레드가 Push한 것이 아니라면 oldTail은 dummy일 것이고, oldData도 null일 것이다.
				// 그러므로 이전에 있던 dummy 데이터에 newData를 넣어주고, dummy를 추가해서 tail 뒤에 달아주면 된다.
				if (oldTail.ptr->data.compare_exchange_strong(oldData, newData.get()))
				{
					oldTail.ptr->next = dummy;
					oldTail = _tail.exchange(dummy);
					FreeExternalCount(oldTail);

					// newData는 oldTail.ptr 쪽으로 이동이 되었으므로 release를 통해 소유권을 포기하게 하여 
					// newData가 스택에서 소멸 시에 이미 존재하지 않는 데이터에 대해 소멸자를 호출하지 않게 한다.
					newData.release();
					break;
				}

				// 소유권 못 얻은 경우
				oldTail.ptr->ReleaseRef();
			}
		}

		shared_ptr<T> TryPop()
		{
			CountedNodePtr oldHead = _head.load();

			while (true)
			{
				// 참조권 획득 (externalCount를 현시점 기준 +1 경우 참조권 획득)
				IncreaseExternalCount(_head, oldHead);

				Node* ptr = oldHead.ptr;
				if (ptr == _tail.load().ptr)
				{
					ptr->ReleaseRef();
					return shared_ptr<T>();
				}

				if (_head.compare_exchange_strong(oldHead, ptr->next))
				{
					T* res = ptr->data.exchange(nullptr);
					FreeExternalCount(oldHead);
					return shared_ptr<T>(res);
				}

				ptr->ReleaseRef();
			}
		}

	private:

		// counter의 ExternalCount를 1증가시킨다. 
		static void IncreaseExternalCount(atomic<CountedNodePtr>& counter, CountedNodePtr& oldCounter)
		{
			while (true)
			{
				CountedNodePtr newCounter = oldCounter;
				newCounter.externalCount++;

				if (counter.compare_exchange_strong(oldCounter, newCounter))
				{
					oldCounter.externalCount = newCounter.externalCount;
					break;
				}
			}
		}

		static void FreeExternalCount(CountedNodePtr& oldNodePtr)
		{
			Node* ptr = oldNodePtr.ptr;
			const int32 countIncrease = oldNodePtr.externalCount - 2;

			NodeCounter oldCounter = ptr->count.load();

			while (true)
			{
				NodeCounter newCounter = oldCounter;
				newCounter.externalCountRemaining--; // 이게 0이 아니면 Push나 Pop 양쪽에서 건드리고 있는 상황이다.
				newCounter.internalCount += countIncrease;

				if (ptr->count.compare_exchange_strong(oldCounter, newCounter))
				{
					if (newCounter.internalCount == 0 && newCounter.externalCountRemaining == 0)
					{
						delete ptr;
					}
					break;
				}
			}
		}

	private:
		atomic<CountedNodePtr> _head;
		atomic<CountedNodePtr> _tail;
	};
}
