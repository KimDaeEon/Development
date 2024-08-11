#pragma once
template<typename T>
class LockQueue
{
public:
	void Push(T item)
	{
		WRITE_LOCK;
		_items.push(item);
	}

	T Pop()
	{
		WRITE_LOCK;
		if (_items.empty())
		{
			return T();
		}

		T ref = _items.front();
		_items.pop();
		return ref;
	}

	void Clear()
	{
		WRITE_LOCK;
		_items = std::queue<T>();
	}

	void PopAll(OUT std::vector<T>& items)
	{
		WRITE_LOCK;
		while (!_items.empty())
		{
			items.push_back(Pop());
		}
	}
private:
	USE_LOCK;
	std::queue<T> _items;
};