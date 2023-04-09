#include "pch.h"
#include "JobQueue.h"
#include "GlobalQueue.h"

// ----------------------------
//			JobQueue
// ----------------------------
void JobQueue::Push(JobRef job, bool pushOnly)
{
	// �׻� _jobCount�� ���� ��Ű�� _jobs.Push�� �ؾ��Ѵ�.
	// �׷��� Execute()�� processedJobCount�� üũ�ϴ� �κп��� ������ ������ �ʴ´�.
	const int32 prevCount = _jobCount.fetch_add(1);
	_jobs.Push(job);

	// ù��° job�� ���� ������� ���൵ ���
	if (prevCount == 0)
	{
		if (LCurrentJobQueue == nullptr && pushOnly == false)
		{
			Execute();
		}
		else
		{
			// ���� �Ҵ���� JobQueue�� �ִٸ� GlobalQueue�� �Ѱܼ� �ٸ� �����忡��
			// �л��ؼ� ó���ϵ��� �Ѵ�.
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
