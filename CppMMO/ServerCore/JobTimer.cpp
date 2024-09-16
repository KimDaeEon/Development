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

	LockGuard lg(_mutex);
	_items.push(JobTimerItem{ tickToBeExecuted, jobData });
}

void JobTimer::Distribute(uint64 now)
{
	// �� ���� �� �����常 �����Ų��.
	if (_distributing.exchange(true) == true)
	{
		return;
	}

	myVector<JobTimerItem> itemsToBeExecuted;
	{
		LockGuard lg(_mutex);

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

	// �����尡 �ϳ��� ���� �����Ƿ� ���⼭ �̷��� ������ �����ϴ�.
	for (JobTimerItem& item : itemsToBeExecuted)
	{
		// owner�� nullptr�� �ƴϸ� �Ʒ� if������ ���´�. ==�� �ƴϴ�. ����.
		if (JobQueueRef owner = item.jobData->owner.lock())
		{
			owner->Push(item.jobData->job, true);
		}
	}

	// ����
	_distributing.store(false);
}

void JobTimer::Clear()
{
	LockGuard lg(_mutex);

	while (_items.empty() == false)
	{
		const JobTimerItem& item = _items.top();

		// JobTimer::Reserve���� �޸�Ǯ�� ��������Ƿ� �̷��� �޸𸮸� �����Ѵ�.
		ObjectPool<JobData>::Push(item.jobData); 

		_items.pop();
	}
}
