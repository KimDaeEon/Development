using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace ServerCore
{
    // 재귀적 lock 을 허용할 지(No)
    // 스핀락 정책 (5000번 정도 loop 반복되면 yield 해서 다른 스레드가 실행되도록)
    class Lock
    {
        // C# 에서는 아래처럼 const 를 붙이면 자동으로 static 이 된다.
        const int EMPTY_FLAG = 0x00000000;
        const int WRITE_MASK = 0x7FFF0000;
        const int READ_MASK = 0x0000FFFF;
        const int MAX_SPIN_COUNT = 5000;

        // [Unused(1비트)] [WriteThreadId(15비트)] [ReadCount(16비트)]
        // ReadCount는 Read Thread 의 갯수를 의미
        // WritherThreadId는 Write 를 작업 중인 Thread 의 Id, Write 는 한 번에 한 쓰레드만 가능하다.

        int _flag = EMPTY_FLAG;
        int _writeCount = 0;

        public void WriteLock()
        {
            // Write 쓰레드나 Read 쓰레드의 Lock 이 없을 때에 소유권을 얻는다.
            int desired = (Thread.CurrentThread.ManagedThreadId << 16) & WRITE_MASK;

            while (true)
            {
                for (int i = 0; i < MAX_SPIN_COUNT; i++)
                {
                    // 시도를 해서 성공하면 return
                    // 일단 여기서 소유권을 얻은 스레드가 자신의 threadId를 _flag 에 입력하므로, 다른 스레드는 == EMPTY_FLAG에서 걸려서 
                    // lock 을 획득할 수 없다.
                    if (Interlocked.CompareExchange(ref _flag, desired, EMPTY_FLAG) == EMPTY_FLAG)
                        return;
                }

                Thread.Yield();
            }

        }

        public void WriteUnlock()
        {
            Interlocked.Exchange(ref _flag, EMPTY_FLAG);
        }

        public void ReadLock()
        {
            while (true)
            {
                for(int i=0; i < MAX_SPIN_COUNT; i++)
                {
                    // WriterLock이 없으면 lock 획득 가능, ReaderLock이 있다면 ReaderLock의 cnt 1 증가

                    // 이 부분이 헷갈리는데, expected = flag & READ_MASK를 하면 _flag의 ReadCnt 값이 출력된다.
                    // WriteUnlock을 하면 flag 값이 0이 되고, ReadThread만 lock 을 획득하고 있으면 expected 값이 flag 의 값과 같아서 계속해서 증가시킬 수 있지만,
                    // WriteLock이 있다면 앞단의 비트 때문에 expected와 flag의 값이 다르기에 값이 바뀌지 못하고 == expected 에서 계속 false가 떨어진다.
                    // 이로 인해 lock 을 획득하지 못하는 것이니 잘 기억해두자.
                 
                    int expected = _flag & READ_MASK;
                    if (Interlocked.CompareExchange(ref _flag, expected + 1, expected) == expected)
                        return;

                }

                Thread.Yield();
            }
        }

        public void ReadUnlock()
        {
            Interlocked.Decrement(ref _flag);
        }
    }
}
