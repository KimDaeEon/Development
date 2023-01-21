#pragma once
#include <mutex>

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