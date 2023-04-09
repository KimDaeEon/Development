#include "pch.h"
#include "JobQueue.h"
#include "GlobalQueue.h"

// ----------------------------
//			JobQueue
// ----------------------------
void JobQueue::Push(JobRef job, bool pushOnly)
{
	// 항상 _jobCount를 증가 시키고 _jobs.Push를 해야한다.
	// 그래야 Execute()의 processedJobCount를 체크하는 부분에서 문제가 생기지 않는다.
	const int32 prevCount = _jobCount.fetch_add(1);
	_jobs.Push(job);

	// 첫번째 job을 넣은 스레드는 실행도 담당
	if (prevCount == 0)
	{
		if (LCurrentJobQueue == nullptr && pushOnly == false)
		{
			Execute();
		}
		else
		{
			// 현재 할당받은 JobQueue가 있다면 GlobalQueue에 넘겨서 다른 스레드에서
			// 분산해서 처리하도록 한다.
			GGlobalQueue->Push(shared_from_this());
		}
	}
}

void JobQueue::Execute()
{
	LCurrentJobQueue = this;

	while (true)
	{
		myVector<JobRef> jobs;
		_jobs.PopAll(OUT jobs);

		const int32 jobCount = static_cast<int32>(jobs.size());
		for (int32 i = 0; i < jobCount; i++)
		{
			jobs[i]->Execute();
		}

		int processedJobCount = _jobCount.fetch_sub(jobCount);
		if (processedJobCount == jobCount)
		{
			LCurrentJobQueue = nullptr;
			return;
		}

		const uint64 now = ::GetTickCount64();
		if (now >= LEndTickCount)
		{
			LCurrentJobQueue = nullptr;
			GGlobalQueue->Push(shared_from_this());
			break;
		}
	}
}
