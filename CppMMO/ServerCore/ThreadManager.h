#pragma once
#include <thread>
#include <functional>

// ThreadManager
class ThreadManager
{
public:
	ThreadManager();
	~ThreadManager();

	// function<void(void)> 형태여야 람다를 받을 수 있다.
	void Launch(function<void(void)> callback);
	void Join();

	static void InitTLS();
	static void DestoryTLS();
private:
	Mutex _lock;
	vector<thread> _threads;
};

