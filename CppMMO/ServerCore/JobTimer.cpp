#include "pch.h"
#include "JobTimer.h"
#include "JobQueue.h"

// ---------------------------
//			JobTimer
// ---------------------------
void JobTimer::Reserve(uint64 tickAfter, weak_ptr<JobQueue> owner, JobRef job)
{
	const uint64 tickToBeExecuted = ::GetTickCount64() + tickAfter;
	JobData* jobData = ObjectPool<JobData>::Pop(owner, job);

	WRITE_LOCK; // JobTimer는 전역으로 사용될 것이기에 락을 걸어준다.
	_items.push(JobTimerItem{ tickToBeExecuted, jobData });
}

void JobTimer::Distribute(uint64 now)
{
	// 한 번에 한 스레드만 통과시킨다.
	if (_distributing.exchange(true) == true)
	{
		return;
	}

	myVector<JobTimerItem> itemsToBeExecuted;
	{
		WRITE_LOCK;

		while (_items.empty() == false)
		{
			const JobTimerItem& item = _items.top();
			if (now < item.tickToBeExecuted)
			{
				break;
			}

			itemsToBeExecuted.push_back(item);
			_items.pop();
		}
	}

	// 스레드가 하나만 들어와 있으므로 여기서 이렇게 수행이 가능하다.
	for (JobTimerItem& item : itemsToBeExecuted)
	{
		// owner가 nullptr이 아니면 아래 if문으로 들어온다. ==가 아니다. 주의.
		if (JobQueueRef owner = item.jobData->owner.lock())
		{
			owner->Push(item.jobData->job, true);
		}
	}

	// 해제
	_distributing.store(false);
}

void JobTimer::Clear()
{
	WRITE_LOCK;

	while (_items.empty() == false)
	{
		const JobTimerItem& item = _items.top();

		// JobTimer::Reserve에서 메모리풀을 사용했으므로 이렇게 메모리를 해제한다.
		ObjectPool<JobData>::Push(item.jobData); 

		_items.pop();
	}
}
