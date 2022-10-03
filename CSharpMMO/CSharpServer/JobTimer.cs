using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using ServerCore;

namespace CSharpServer
{
    struct JobTimerElement : IComparable<JobTimerElement>
    {
        public int toBeExecutedTick; // 실행이 될 시간
        public Action action;

        public int CompareTo(JobTimerElement other)
        {
            // 상대방이 나보다 크면 위치가 바뀐다.
            // 가장 실행 시간이 적게 남은 것이 큐의 맨 위에 오는 것이다.
            return other.toBeExecutedTick - this.toBeExecutedTick;
        }
    }
    class JobTimer
    {
        PriorityQueue<JobTimerElement> _pq = new PriorityQueue<JobTimerElement>();
        object _lock = new object();

        // property 초기화 문법으로 reference type 의 경우 아래처럼 초기화한다.
        public static JobTimer Instance { get; } = new JobTimer(); 

        public void Push(Action action, int tickAfter)
        {
            JobTimerElement job;

            // 시스템이 켜지고 나서 지난 milliseconds 이다. int 같은 경우 대략 20억이므로,
            // 23 일 정도의 시간까지는 안전한 값이다. 
            // 오래 지속되는 이벤트 같은 경우는 이 시간을 기준으로 하면 안될 것 같다.
            job.toBeExecutedTick = System.Environment.TickCount + tickAfter;
            job.action = action;

            lock (_lock)
            {
                _pq.Push(job);
            }
        }

        public void Flush()
        {
            while (true)
            {
                int now = System.Environment.TickCount;

                JobTimerElement job;

                lock (_lock)
                {
                    if(_pq.Count == 0)
                    {
                        break;
                    }

                    job = _pq.Peek();

                    if(job.toBeExecutedTick > now)
                    {
                        break;
                    }

                    _pq.Pop();
                }

                job.action.Invoke();
            }
        }
    }
}
