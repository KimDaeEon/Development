#pragma once
template<typename T>
class LockQueue
{
public:
	void Push(T item)
	{
		LockGuard lg(_mutex);
		_items.push(item);
	}

	T Pop()
	{
		LockGuard lg(_mutex);
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
		LockGuard lg(_mutex);
		_items = std::queue<T>();
	}

	void PopAll(OUT std::vector<T>& items)
	{
		LockGuard lg(_mutex);
		while (!_items.empty())
		{
			auto item = _items.front();
			items.push_back(item);
			_items.pop();
		}
	}

private:
	Mutex _mutex;
	std::queue<T> _items;
};