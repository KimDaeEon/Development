using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GameServer
{
    public class JobQueue
    {
        JobTimer _timer = new JobTimer();
        Queue<IJob> _jobQueue = new Queue<IJob>();
        object _lock = new object();

        public int TimerCount { get { return _timer.Count; } }
        public int JobCount { get { lock (_lock) { return _jobQueue.Count; } } }

        public IJob PushFutureJob(int tickAfter, Action action) { return PushFutureJob(tickAfter, new Job(action)); }
        public IJob PushFutureJob<T1>(int tickAfter, Action<T1> action, T1 t1) { return PushFutureJob(tickAfter, new Job<T1>(action, t1)); }
        public IJob PushFutureJob<T1, T2>(int tickAfter, Action<T1, T2> action, T1 t1, T2 t2) { return PushFutureJob(tickAfter, new Job<T1, T2>(action, t1, t2)); }
        public IJob PushFutureJob<T1, T2, T3>(int tickAfter, Action<T1, T2, T3> action, T1 t1, T2 t2, T3 t3) { return PushFutureJob(tickAfter, new Job<T1, T2, T3>(action, t1, t2, t3)); }
        public IJob PushFutureJob<T1, T2, T3, T4>(int tickAfter, Action<T1, T2, T3, T4> action, T1 t1, T2 t2, T3 t3, T4 t4) { return PushFutureJob(tickAfter, new Job<T1, T2, T3, T4>(action, t1, t2, t3, t4)); }
        public IJob PushFutureJob<T1, T2, T3, T4, T5>(int tickAfter, Action<T1, T2, T3, T4, T5> action, T1 t1, T2 t2, T3 t3, T4 t4, T5 t5) { return PushFutureJob(tickAfter, new Job<T1, T2, T3, T4, T5>(action, t1, t2, t3, t4, t5)); }
        public IJob PushFutureJob<T1, T2, T3, T4, T5, T6>(int tickAfter, Action<T1, T2, T3, T4, T5, T6> action, T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6) { return PushFutureJob(tickAfter, new Job<T1, T2, T3, T4, T5, T6>(action, t1, t2, t3, t4, t5, t6)); }
        public IJob PushFutureJob<T1, T2, T3, T4, T5, T6, T7>(int tickAfter, Action<T1, T2, T3, T4, T5, T6, T7> action, T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6, T7 t7) { return PushFutureJob(tickAfter, new Job<T1, T2, T3, T4, T5, T6, T7>(action, t1, t2, t3, t4, t5, t6, t7)); }

        public IJob PushFutureJob(int tickAfter, IJob job)
        {
            _timer.Push(job, tickAfter);
            return job;
        }

        public void PushJob(Action action) { PushJob(new Job(action)); }
        public void PushJob<T1>(Action<T1> action, T1 t1) { PushJob(new Job<T1>(action, t1)); }
        public void PushJob<T1, T2>(Action<T1, T2> action, T1 t1, T2 t2) { PushJob(new Job<T1, T2>(action, t1, t2)); }
        public void PushJob<T1, T2, T3>(Action<T1, T2, T3> action, T1 t1, T2 t2, T3 t3) { PushJob(new Job<T1, T2, T3>(action, t1, t2, t3)); }
        public void PushJob<T1, T2, T3, T4>(Action<T1, T2, T3, T4> action, T1 t1, T2 t2, T3 t3, T4 t4) { PushJob(new Job<T1, T2, T3, T4>(action, t1, t2, t3, t4)); }
        public void PushJob<T1, T2, T3, T4, T5>(Action<T1, T2, T3, T4, T5> action, T1 t1, T2 t2, T3 t3, T4 t4, T5 t5) { PushJob(new Job<T1, T2, T3, T4, T5>(action, t1, t2, t3, t4, t5)); }
        public void PushJob<T1, T2, T3, T4, T5, T6>(Action<T1, T2, T3, T4, T5, T6> action, T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6) { PushJob(new Job<T1, T2, T3, T4, T5, T6>(action, t1, t2, t3, t4, t5, t6)); }
        public void PushJob<T1, T2, T3, T4, T5, T6, T7>(Action<T1, T2, T3, T4, T5, T6, T7> action, T1 t1, T2 t2, T3 t3, T4 t4, T5 t5, T6 t6, T7 t7) { PushJob(new Job<T1, T2, T3, T4, T5, T6, T7>(action, t1, t2, t3, t4, t5, t6, t7)); }

        public void PushJob(IJob job)
        {
            lock (_lock)
            {
                _jobQueue.Enqueue(job);
            }
        }

        public void Flush()
        {
            _timer.Flush();

            while (true)
            {
                IJob job = Pop();
                if (job == null)
                    return;

                job.Execute();
            }
        }

        IJob Pop()
        {
            lock (_lock)
            {
                if (_jobQueue.Count == 0)
                {
                    return null;
                }
                return _jobQueue.Dequeue();
            }
        }
    }
}
