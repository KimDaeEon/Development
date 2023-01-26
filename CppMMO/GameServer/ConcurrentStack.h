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
				return false; // pop ���������� �˷��ش�.
			}

			value = std::move(_stack.top());
			_stack.pop();

			return true; // pop ���������� �˷��ش�.
		}

		// ���� TryPop �Լ�ó�� ���� ������ ���鼭 Ȯ���ϴ� ���� �ƴ϶�, Stack�� ���빰�� ������ Wait�ϴ� Pop �Լ�.
		void WaitPop(T& value)
		{
			unique_lock<mutex> lock(_mutex);
			_condVar.wait(lock, [this] { return _stack.empty() == false; }); // TODO: ���� ĸ�� [this]  �̰� �Ⱦ��� _stack �κп��� ���������� Ȯ��
			value = std::move(_stack.front());
			_stack.pop();
		}

	private:
		stack<T> _stack;
		mutex _mutex;
		condition_variable _condVar;
	};

	// LockFree�� lock�� �Ⱦ��� ������, ��ǻ� �������̵� ���� ���� �ڵ尡 ���������� �ʴ�.
	// �ϴ� �̷��� �� �� �ִٴ� �� ������ �˾Ƶ���.
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
			// �� �κ��� ���������� ������ �Ͼ ���� ����. �׷��� Push�� Pop���� ������ �����ϴ�.
			Node* node = new Node(value);
			node->next = _head;

			// �Ʒ� �ڵ�� �ǻ� �Ʒ��� ������ atomic�ϰ� �Ѵٰ� ���� �ȴ�.
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

			// head�� node�� �ٲ۴�.
			while (_head.compare_exchange_weak(node->next, node) == false)
			{
				// �����ϸ� ���������� node->next = _head�� ���ֱ⿡ ���⼭ �� �� �� �ʿ䰡 ����.
			}
		}

		// head�� ���� �༮�� head�� �ǰ�, head ���� pop �Ǿ�� �Ѵ�.
		bool TryPop(T& value)
		{
			++_popCount;

			Node* prevHead = _head;

			// _head�� _head->next�� �ٲ۴�.
			while (prevHead && (_head.compare_exchange_weak(prevHead, prevHead->next) == false))
			{
				// �����ϸ� ���������� prevHead = _head�� ���ֱ⿡ ���⼭ �� �� �� �ʿ䰡 ����.
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
			// ȥ�� Pop�ϴ� ��Ȳ
			if (_popCount == 1)
			{

				// ȥ���̹Ƿ� ���� ����� �ٸ� �����͵鵵 ����
				Node* node = _pendingList.exchange(nullptr);

				if (--_popCount == 0)
				{
					// ����� ��Ұ� �����Ƿ� ���� ����, �̹� _pendingList�� �ִ� ���� �� ���Ա⿡ ���Ŀ� _popCount�� 1�� �Ǿ ���� ����.
					DeleteNodes(node);
				}
				else if (node != nullptr)
				{
					// ���� ������ ���̷� ���������Ƿ� �Ʊ� �����Դ� _pendingList�� ���� _pendingList �տ� ��ġ�Ͽ� �̾��ش�.
					ConcatenatePendingNodeList(node);
				}

				delete prevHead;
			}
			// �ٸ� �����忡���� Pop�ϴ� ��Ȳ, delete �� �� ����. ������Ų��.
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

		// ����Ʈ�� ������ ����(first)�� ��(last)�� �����ٵ�, �� ����Ʈ�� _pendingList �տ� ��ġ�Ͽ� �̾��ش�.
		void ConcatenatePendingNodeList(Node* first, Node* last)
		{
			last->next = _pendingList;

			// last->next = _pendingList, _pendingList = first �� �� �۾��� ���ÿ� ����
			// _pendingList�� �߰��� ����Ǵ��� ����ؼ� last->next = _pendingList�� �Ǿ ��ǲ ����Ʈ �� �ڿ� _pendingList�� �´�.
			while (_pendingList.compare_exchange_weak(last->next, first) == false)
			{
			}
		}

		// ����Ʈ�� ó�� �����͸� �Է��ؼ� ����Ʈ�� _pendingList �տ� ��ġ�Ͽ� �̾��ش�.
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
			// 1���� node�� �� �տ� ��ġ�Ͽ� �̾��� �Ŵϱ� node, node�� �ִ´�.
			ConcatenatePendingNodeList(node, node);
		}

	private:
		atomic<Node*> _head;

		atomic<uint32> _popCount = 0; // Pop�� ������ ������ ����
		atomic<Node*> _pendingList; // ���� �Ǿ�� �� ����
	};
}
