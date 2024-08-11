#pragma once
#include "Job.h"
#include "JobTimer.h"

// ----------------------------
//			JobQueue
// ----------------------------
class JobQueue : public enable_shared_from_this<JobQueue>
{
	// Push, Execute같은 Private 함수에 접근 가능한 클래스
	friend class ThreadManager;
	friend class JobTimer;

public:
	// 일반 콜백 처리용
	void PushJob(JobCallbackType&& callback)
	{
		Push(ObjectPool<Job>::MakeShared(std::move(callback)));
	}

	// JobQueue를 상속받은 객체의 멤버 함수 작업 처리용
	template<typename T, typename Ret, typename... Args>
	void PushJob(Ret(T::* memFunc)(Args...), Args... args)
	{
		shared_ptr<T> owner = static_pointer_cast<T>(shared_from_this());
		Push(ObjectPool<Job>::MakeShared(owner, memFunc, std::forward<Args>(args)...));
	}

	// 일반 콜백 예약 작업 처리용
	void PushFutureJob(uint64 tickAfter, JobCallbackType&& callback)
	{
		JobRef job = ObjectPool<Job>::MakeShared(std::move(callback));
		GJobTimer->Reserve(tickAfter, shared_from_this(), job);
	}

	// JobQueue를 상속받은 객체의 멤버 함수 예약 작업 처리용
	template<typename T, typename Ret, typename... Args>
	void PushFutureJob(uint64 tickAfter, Ret(T::* memFunc)(Args...), Args... args)
	{
		shared_ptr<T> owner = static_pointer_cast<T>(shared_from_this());
		JobRef job = ObjectPool<Job>::MakeShared(owner, memFunc, std::forward<Args>(args)...);
		GJobTimer->Reserve(tickAfter, shared_from_this(), job);
	}

	void		ClearJobs() { _jobs.Clear(); }

private:
	void		Push(JobRef job, bool pushOnly = false);
	void		Execute();

protected:
	LockQueue<JobRef>	_jobs; // TODO: 추후 MyQueue 쓰게 변경 필요
	Atomic<int32>		_jobCount = { 0 };
};

