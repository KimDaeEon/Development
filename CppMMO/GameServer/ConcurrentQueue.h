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
				return false; // pop ���������� �˷��ش�.
			}

			value = std::move(_queue.front());
			_queue.pop();

			return true; // pop ���������� �˷��ش�.
		}

		// ���� TryPop �Լ�ó�� ���� ������ ���鼭 Ȯ���ϴ� ���� �ƴ϶�, Queue�� ���빰�� ������ Wait�ϴ� Pop �Լ�.
		void WaitPop(T& value)
		{
			unique_lock<mutex> lock(_mutex);
			_condVar.wait(lock, [this] { return _queue.empty() == false; }); // ���� ĸ�� [this]�� ������ ��� ������ ������ �Ұ����ϴ�.
			value = std::move(_queue.front());
			_queue.pop();
		}

	private:
		queue<T> _queue;
		mutex _mutex;
		condition_variable _condVar;
	};

	// Stack���� ����� ���� stack�� Pop�� ������ ������ �Ͼ�� �����ϸ� �Ǿ��µ�, Queue���� ���� push�� pop ��ο��� ������ �߻��� �� �ִ�.
	// ������ ǥ�����ڸ� Stack���� ���� _head ���� ����� ������ �����ϸ� �Ǿ��µ�, Queue�� _head�� tail�� ����� ���� ��� �����ؾ� �Ѵ�.
	// �׷��� ���� ����, ���ɵ� lock�� �� ���̰� ���ų� ���� ��쵵 �ִٰ� �Ͽ��� �̷� ���� �ִٴ� �͸� �˾Ƶ���.
	template<typename T>
	class LockFreeQueue
	{
		struct Node;

		struct CountedNodePtr
		{
			int32 externalCount;
			Node* ptr = nullptr;
		};

		// �Ʒ����� ��Ʈ�� 32��Ʈ�� ������ �Ͽ��� NodeCounter�� 32��Ʈ�� ����ü�� �ȴ�.
		struct NodeCounter
		{
			uint32 internalCount : 30; // ������ ��ȯ ����

			// Push & Pop ���� ������ ����, Push Pop �� ������ 1�� ����
			// 0�� �Ǹ� ������ Queue���� ������ ���̹Ƿ� �޸� ������ �����Ѵ�.
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
		// �����ڿ��� ���̳�带 �����ؼ� _head�� _tail�� ����Ų��. �����Ͱ� �ֳ� ���ĸ� _head == _tail�� �Ǵ��Ѵ�.
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
			// ������ �ϳ��� �߰��Ǹ�, �����Ͱ� ���� ���� ��嵵 1�� �߰��ȴ�. �׻� ���� ��尡 �����ϰ� �ȴ�.
			unique_ptr<T> newData = make_unique<T>(value);
			CountedNodePtr dummy;
			dummy.ptr = new Node;
			dummy.externalCount = 1;

			CountedNodePtr oldTail = _tail.load();

			while (true)
			{
				// ������ ȹ�� (externalCount�� ������ ���� +1�� �����尡 ȹ��)
				IncreaseExternalCount(_tail, oldTail);

				// ������ ȹ�� (data�� ���� ��ȯ�� �����尡 ȹ��)
				T* oldData = nullptr;

				// �� �����尡 Push�� ���� �ƴ϶�� oldTail�� dummy�� ���̰�, oldData�� null�� ���̴�.
				// �׷��Ƿ� ������ �ִ� dummy �����Ϳ� newData�� �־��ְ�, dummy�� �߰��ؼ� tail �ڿ� �޾��ָ� �ȴ�.
				if (oldTail.ptr->data.compare_exchange_strong(oldData, newData.get()))
				{
					oldTail.ptr->next = dummy;
					oldTail = _tail.exchange(dummy);
					FreeExternalCount(oldTail);

					// newData�� oldTail.ptr ������ �̵��� �Ǿ����Ƿ� release�� ���� �������� �����ϰ� �Ͽ� 
					// newData�� ���ÿ��� �Ҹ� �ÿ� �̹� �������� �ʴ� �����Ϳ� ���� �Ҹ��ڸ� ȣ������ �ʰ� �Ѵ�.
					newData.release();
					break;
				}

				// ������ �� ���� ���
				oldTail.ptr->ReleaseRef();
			}
		}

		shared_ptr<T> TryPop()
		{
			CountedNodePtr oldHead = _head.load();

			while (true)
			{
				// ������ ȹ�� (externalCount�� ������ ���� +1 ��� ������ ȹ��)
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

		// counter�� ExternalCount�� 1������Ų��. 
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
				newCounter.externalCountRemaining--; // �̰� 0�� �ƴϸ� Push�� Pop ���ʿ��� �ǵ帮�� �ִ� ��Ȳ�̴�.
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
