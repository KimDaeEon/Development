#pragma once

struct JobData
{
	JobData(weak_ptr<JobQueue> owner, JobRef job) : owner(owner), job(job)
	{
	}

	weak_ptr<JobQueue>	owner;
	JobRef				job;
};

// ��� ó������ �ʰ� ���� �ð� �Ŀ� ó���Ǵ� �۾� ������ �����ϴ� ����ü
struct JobTimerItem
{
	// myPriorityQueue�� ���� ���ȴ�.
	bool operator<(const JobTimerItem& other) const
	{
		// ���� myPriorityQueue�� less �Լ��� ���� �صξ
		// ��ȣ�� �ݴ�� �ؾ� �ð��� ���� ���� �۾��� ���� ����ȴ�.
		return tickToBeExecuted > other.tickToBeExecuted;
	}

	uint64 tickToBeExecuted = 0;
	JobData* jobData = nullptr;
};

// ---------------------------
//			JobTimer
// ---------------------------
class JobTimer
{
public:
	void Reserve(uint64 tickAfter, weak_ptr<JobQueue> owner, JobRef job);
	void Distribute(uint64 now);
	void Clear();

private:
	USE_LOCK;
	myPriorityQueue<JobTimerItem>	_items;
	Atomic<bool>					_distributing = false;
};

