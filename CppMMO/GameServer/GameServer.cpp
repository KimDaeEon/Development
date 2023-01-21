﻿#include "pch.h"
#include <iostream>
#include <thread>
#include <mutex>
#include "ConcurrentQueue.h"
#include "ConcurrentStack.h"

LockQueue<int32> q;
LockStack<int32> s;

void Push()
{
	while (true)
	{
		int32 value = rand() % 100;
		q.Push(value);

		this_thread::sleep_for(10ms);
	}
}

void Pop()
{
	while (true)
	{
		int32 data = 3;

		//q.WaitPop(data);
		if (q.TryPop(data))
		{
			cout << data << endl;
		}
	}
}

int main()
{
	thread t1(Push);
	thread t2(Pop);

	t1.join();
	t2.join();
}