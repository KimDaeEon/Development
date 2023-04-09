#include "pch.h"
#include "ThreadManager.h"
#include "CoreTLS.h"
#include "CoreGlobal.h"
#include "GlobalQueue.h"
#include "JobQueue.h"

// ThreadManager
ThreadManager::ThreadManager()
{
	// Main Thread
	InitTLS();
}

ThreadManager::~ThreadManager()
{
	Join();
}

void ThreadManager::Launch(function<void(void)> callback)
{
	LockGuard guard(_lock);

	_threads.push_back(thread([=]()
		{
			InitTLS();
			callback();
			DestoryTLS();
		}));
}

void ThreadManager::Join()
{
	for (thread& t : _threads)
	{
		if (t.joinable())
		{
			t.join();
		}
	}

	_threads.clear();
}

void ThreadManager::InitTLS()
{
	static Atomic<uint32> SThread = 1;
	LThreadId = SThread.fetch_add(1);
}

// 혹시나 InitTLS()에서 동적으로 할당하는 부분이 생긴다면 여기서 해제
void ThreadManager::DestoryTLS()
{

}

void ThreadManager::DoGlobalQueueJobs()
{
	while (true)
	{
		uint64 now = ::GetTickCount64();
		if (now > LEndTickCount)
		{
			break;
		}

		JobQueueRef jobQueue = GGlobalQueue->Pop();
		if (jobQueue == nullptr)
		{
			break;
		}

		jobQueue->Execute();
	}
}

void ThreadManager::DistributeFutureJobs()
{
	const uint64 now = ::GetTickCount64();

	GJobTimer->Distribute(now);
}
