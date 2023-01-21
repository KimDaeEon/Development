#pragma once

#include <mutex>

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