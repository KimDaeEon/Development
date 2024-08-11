#pragma once
#include "Job.h"
#include "JobTimer.h"

// ----------------------------
//			JobQueue
// ----------------------------
class JobQueue : public enable_shared_from_this<JobQueue>
{
	// Push, Execute���� Private �Լ��� ���� ������ Ŭ����
	friend class ThreadManager;
	friend class JobTimer;

public:
	// �Ϲ� �ݹ� ó����
	void PushJob(JobCallbackType&& callback)
	{
		Push(ObjectPool<Job>::MakeShared(std::move(callback)));
	}

	// JobQueue�� ��ӹ��� ��ü�� ��� �Լ� �۾� ó����
	template<typename T, typename Ret, typename... Args>
	void PushJob(Ret(T::* memFunc)(Args...), Args... args)
	{
		shared_ptr<T> owner = static_pointer_cast<T>(shared_from_this());
		Push(ObjectPool<Job>::MakeShared(owner, memFunc, std::forward<Args>(args)...));
	}

	// �Ϲ� �ݹ� ���� �۾� ó����
	void PushFutureJob(uint64 tickAfter, JobCallbackType&& callback)
	{
		JobRef job = ObjectPool<Job>::MakeShared(std::move(callback));
		GJobTimer->Reserve(tickAfter, shared_from_this(), job);
	}

	// JobQueue�� ��ӹ��� ��ü�� ��� �Լ� ���� �۾� ó����
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
	LockQueue<JobRef>	_jobs; // TODO: ���� MyQueue ���� ���� �ʿ�
	Atomic<int32>		_jobCount = { 0 };
};

