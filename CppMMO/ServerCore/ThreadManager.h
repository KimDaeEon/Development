#pragma once
#include <thread>
#include <functional>

// ThreadManager
class ThreadManager
{
public:
	ThreadManager();
	~ThreadManager();

	// function<void(void)> ���¿��� ���ٸ� ���� �� �ִ�.
	void Launch(function<void(void)> callback);
	void Join();

	static void InitTLS();
	static void DestoryTLS();
private:
	Mutex _lock;
	vector<thread> _threads;
};

