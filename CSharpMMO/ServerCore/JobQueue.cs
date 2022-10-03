using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ServerCore
{
    public interface IJobQueue
    {
        void Push(Action job);
    }
    public class JobQueue : IJobQueue
    {
        Queue<Action> _jobQueue = new Queue<Action>();
        object _lock = new object();
        bool _flushing= false; // 현재 _jobQueue 에 쌓인 job 들을 처리하고 있는지 여부를 나타낸다.

        public void Push(Action job)
        {
            // 이렇게 단계별로 지역 변수에 받아서 lock 으로 관리되는 _flushing 의 상태를 가져올 수 있다는 것을 알아두자.
            bool flushing = false;

            lock (_lock)
            {
                _jobQueue.Enqueue(job);
                if(_flushing == false)
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
            while (true)
            {
                Action action = Pop();
                if(action == null)
                {
                    // queue 에서 일을 다 꺼내서 처리했으면 종료
                    return;
                }

                action.Invoke();
            }
        }

        Action Pop()
        {
            lock (_lock)
            {
                if(_jobQueue.Count == 0)
                {
                    _flushing = false; // 처리할 작업이 없으므로 작업 처리 중이지 않은 상태로 변경
                    return null;
                }

                return _jobQueue.Dequeue();
            }
        }
    }
}

