using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CSharpServer.Contents
{
    public class JobQueue
    {
        JobTimer _jobTimer = new JobTimer(); // 미래에 시간을 두고 실행할 작업들
        Queue<IJob> _instantJobs = new Queue<IJob>(); // 당장 실행해야할 작업들
        object _lock = new object();
        bool _flushing = false; // 현재 _jobQueue 에 쌓인 job 들을 처리하고 있는지 여부를 나타낸다.

        public void PushFutureJob(int tickAfter, Action action) { PushFutureJob(tickAfter, new Job(action)); }
        public void PushFutureJob<T1>(int tickAfter, Action<T1> action, T1 t1) { PushFutureJob(tickAfter, new Job<T1>(action, t1)); }
        public void PushFutureJob<T1, T2>(int tickAfter, Action<T1, T2> action, T1 t1, T2 t2) { PushFutureJob(tickAfter, new Job<T1, T2>(action, t1, t2)); }
        public void PushFutureJob<T1, T2, T3>(int tickAfter, Action<T1, T2, T3> action, T1 t1, T2 t2, T3 t3) { PushFutureJob(tickAfter, new Job<T1, T2, T3>(action, t1, t2, t3)); }
        public void PushFutureJob(int tickAfter, IJob job)
        {
            _jobTimer.Push(job, tickAfter);
        }

        public void Push(Action action) { Push(new Job(action)); }
        public void Push<T1>(Action<T1> action, T1 t1) { Push(new Job<T1>(action, t1)); }
        public void Push<T1, T2>(Action<T1, T2> action, T1 t1, T2 t2) { Push(new Job<T1, T2>(action, t1, t2)); }
        public void Push<T1, T2, T3>(Action<T1, T2, T3> action, T1 t1, T2 t2, T3 t3) { Push(new Job<T1, T2, T3>(action, t1, t2, t3)); }
        public void Push(IJob job)
        {
            // 이렇게 단계별로 지역 변수에 받아서 lock 으로 관리되는 _flushing 의 상태를 가져올 수 있다는 것을 알아두자.
            bool flushing = false;

            lock (_lock)
            {
                _instantJobs.Enqueue(job);
                if (_flushing == false)
                {
                    flushing = _flushing = true;
                }
            }

            if (flushing)
            {
                Flush();
            }
        }

        void Flush()
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
                    _flushing = false; // 처리할 작업이 없으므로 작업 처리 중이지 않은 상태로 변경
                    return null;
                }

                return _instantJobs.Dequeue();
            }
        }
    }
}
