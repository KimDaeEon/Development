#pragma once

#include <mutex>

namespace Archive
{
	template<typename T>
	class LockStack
	{
	public:
		LockStack() {}

		LockStack(const LockStack&) = delete;
		LockStack& operator=(const LockStack&) = delete;

		void Push(T value)
		{
			lock_guard<mutex> lock(_mutex);
			_stack.push(std::move(value));
			_condVar.notify_one();
		}

		bool TryPop(T& value)
		{
			lock_guard<mutex> lock(_mutex);
			if (_stack.empty())
			{
				return false; // pop 실패했음을 알려준다.
			}

			value = std::move(_stack.top());
			_stack.pop();

			return true; // pop 성공했음을 알려준다.
		}

		// 위의 TryPop 함수처럼 무한 루프를 돌면서 확인하는 것이 아니라, Stack의 내용물이 없으면 Wait하는 Pop 함수.
		void WaitPop(T& value)
		{
			unique_lock<mutex> lock(_mutex);
			_condVar.wait(lock, [this] { return _stack.empty() == false; }); // TODO: 람대 캡쳐 [this]  이거 안쓰면 _stack 부분에서 에러나는지 확인
			value = std::move(_stack.front());
			_stack.pop();
		}

	private:
		stack<T> _stack;
		mutex _mutex;
		condition_variable _condVar;
	};

	// LockFree는 lock을 안쓰긴 하지만, 사실상 성능차이도 별로 없고 코드가 직관적이지 않다.
	// 일단 이렇게 쓸 수 있다는 것 정도만 알아두자.
	template<typename T>
	class LockFreeStack
	{
		struct Node
		{
			Node(const T& value) : data(value), next(nullptr)
			{

			}

			T data;
			Node* next;
		};

	public:

		void Push(const T& value)
		{
			// 이 부분은 지역변수라서 경합이 일어날 일이 없다. 그래서 Push가 Pop보다 구현이 간단하다.
			Node* node = new Node(value);
			node->next = _head;

			// 아래 코드는 실상 아래의 연산을 atomic하게 한다고 보면 된다.
			/*if (_head == node->next)
			{
				_head = node;
				return true;
			}
			else
			{
				node->next = _head;
				return false;
			}*/

			// head를 node로 바꾼다.
			while (_head.compare_exchange_weak(node->next, node) == false)
			{
				// 실패하면 내부적으로 node->next = _head를 해주기에 여기서 뭘 더 할 필요가 없다.
			}
		}

		// head값 다음 녀석이 head가 되고, head 값이 pop 되어야 한다.
		bool TryPop(T& value)
		{
			++_popCount;

			Node* prevHead = _head;

			// _head를 _head->next로 바꾼다.
			while (prevHead && (_head.compare_exchange_weak(prevHead, prevHead->next) == false))
			{
				// 실패하면 내부적으로 prevHead = _head를 해주기에 여기서 뭘 더 할 필요가 없다.
			}

			if (prevHead == nullptr)
			{
				return false;
			}

			value = prevHead->data;
			TryDelete(prevHead);
			return true;
		}

		void TryDelete(Node* prevHead)
		{
			// 혼자 Pop하는 상황
			if (_popCount == 1)
			{

				// 혼자이므로 삭제 예약된 다른 데이터들도 삭제
				Node* node = _pendingList.exchange(nullptr);

				if (--_popCount == 0)
				{
					// 끼어든 요소가 없으므로 삭제 진행, 이미 _pendingList에 있는 것은 다 들고왔기에 이후에 _popCount가 1이 되어도 문제 없다.
					DeleteNodes(node);
				}
				else if (node != nullptr)
				{
					// 누가 간발의 차이로 끼어들었으므로 아까 가져왔던 _pendingList를 현재 _pendingList 앞에 배치하여 이어준다.
					ConcatenatePendingNodeList(node);
				}

				delete prevHead;
			}
			// 다른 스레드에서도 Pop하는 상황, delete 할 수 없다. 원복시킨다.
			else
			{
				ConcatenatePendingNode(prevHead);
				--_popCount;
			}
		}


		static void DeleteNodes(Node* node)
		{
			while (node)
			{
				Node* next = node->next;
				delete node;
				node = next;
			}
		}

		// 리스트가 들어오면 시작(first)과 끝(last)이 있을텐데, 그 리스트를 _pendingList 앞에 배치하여 이어준다.
		void ConcatenatePendingNodeList(Node* first, Node* last)
		{
			last->next = _pendingList;

			// last->next = _pendingList, _pendingList = first 이 두 작업을 동시에 실행
			// _pendingList가 중간에 변경되더라도 계속해서 last->next = _pendingList가 되어서 인풋 리스트 맨 뒤에 _pendingList가 온다.
			while (_pendingList.compare_exchange_weak(last->next, first) == false)
			{
			}
		}

		// 리스트의 처음 포인터만 입력해서 리스트를 _pendingList 앞에 배치하여 이어준다.
		void ConcatenatePendingNodeList(Node* node)
		{
			Node* last = node;
			while (last->next)
			{
				last = last->next;
			}

			ConcatenatePendingNodeList(node, last);
		}

		void ConcatenatePendingNode(Node* node)
		{
			// 1개의 node만 맨 앞에 배치하여 이어줄 거니까 node, node로 넣는다.
			ConcatenatePendingNodeList(node, node);
		}

	private:
		atomic<Node*> _head;

		atomic<uint32> _popCount = 0; // Pop을 실행한 쓰레드 개수
		atomic<Node*> _pendingList; // 삭제 되어야 할 노드들
	};
}
