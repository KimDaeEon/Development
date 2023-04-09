#pragma once

struct JobData
{
	JobData(weak_ptr<JobQueue> owner, JobRef job) : owner(owner), job(job)
	{
	}

	weak_ptr<JobQueue>	owner;
	JobRef				job;
};

// 즉시 처리되지 않고 일정 시간 후에 처리되는 작업 정보를 저장하는 구조체
struct JobTimerItem
{
	// myPriorityQueue를 위해 사용된다.
	bool operator<(const JobTimerItem& other) const
	{
		// 현재 myPriorityQueue에 less 함수를 쓰게 해두어서
		// 부호를 반대로 해야 시간이 적게 남은 작업이 먼저 실행된다.
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

