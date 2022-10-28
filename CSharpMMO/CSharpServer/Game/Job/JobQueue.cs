using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CSharpServer.Game
{
    public class JobQueue
    {
        JobTimer _jobTimer = new JobTimer(); // 미래에 시간을 두고 실행할 작업들
        Queue<IJob> _instantJobs = new Queue<IJob>(); // 당장 실행해야할 작업들
        object _lock = new object();
        public IJob PushFutureJob(int tickAfter, Action action) { return PushFutureJob(tickAfter, new Job(action)); }
        public IJob PushFutureJob<T1>(int tickAfter, Action<T1> action, T1 t1) { return PushFutureJob(tickAfter, new Job<T1>(action, t1)); }
        public IJob PushFutureJob<T1, T2>(int tickAfter, Action<T1, T2> action, T1 t1, T2 t2) { return PushFutureJob(tickAfter, new Job<T1, T2>(action, t1, t2)); }
        public IJob PushFutureJob<T1, T2, T3>(int tickAfter, Action<T1, T2, T3> action, T1 t1, T2 t2, T3 t3) { return PushFutureJob(tickAfter, new Job<T1, T2, T3>(action, t1, t2, t3)); }
        public IJob PushFutureJob(int tickAfter, IJob job)
        {
            _jobTimer.Push(job, tickAfter);
            return job;
        }

        public void Push(Action action) { Push(new Job(action)); }
        public void Push<T1>(Action<T1> action, T1 t1) { Push(new Job<T1>(action, t1)); }
        public void Push<T1, T2>(Action<T1, T2> action, T1 t1, T2 t2) { Push(new Job<T1, T2>(action, t1, t2)); }
        public void Push<T1, T2, T3>(Action<T1, T2, T3> action, T1 t1, T2 t2, T3 t3) { Push(new Job<T1, T2, T3>(action, t1, t2, t3)); }
        public void Push(IJob job)
        {
            lock (_lock)
            {
                _instantJobs.Enqueue(job);
            }
        }

        public void Flush()
        {
            _jobTimer.Flush();

            while (true)
            {
                IJob job = Pop();
                if (job == null)
                {
                    // queue 에서 일을 다 꺼내서 처리했으면 종료
                    return;
                }

                job.Execute();
            }
        }

        IJob Pop()
        {
            lock (_lock)
            {
                if (_instantJobs.Count == 0)
                {
                    return null;
                }

                return _instantJobs.Dequeue();
            }
        }
    }
}
