using Generic.IComparableStandard;
using ResourceManagement;
using System;
using System.Collections.Generic;
using System.Threading;

namespace CSharpStudy
{
    public class temp
    {
        public int a;
        public int b;
    }

    class Program
    {

        static void Main(string[] args)
        {
            Action<object> b = (a) =>
            {
                Console.WriteLine(a);
                Console.WriteLine(a is string);
            };
            Action<string> a = b;

            

            #region DisposeTest
            //TestResourceBag();
            #endregion

            #region NativeMemoryTest
            //Thread t = new Thread(checkGCFunc);
            //t.IsBackground = true;
            //t.Start();

            //// 무한 루프를 돌며,
            //while (true)
            //{
            //    // GC Heap, 즉 관리 힙으로부터 배열 메모리를 할당
            //    int[] buf = new int[1024];
            //    {
            //        for (int i = 0; i < buf.Length; i++)
            //        {
            //            buf[i] = i;
            //        }
            //    }
            //}

            //// 무한 루프를 돌며,
            //while (true)
            //{
            //    // Native Heap, 비-관리 힙으로부터 배열 메모리 할당
            //    using (NativeMemory<int> buf = new NativeMemory<int>(1024))
            //    {
            //        Span<int> viewBuf = buf.GetSpan();
            //        for (int i = 0; i < viewBuf.Length; i++)
            //        {
            //            viewBuf[i] = i;
            //        }
            //    }
            //}
            #endregion
        }
        static void TestResourceBag()
        {
            while (true)
            {
                ResourceBag a = new ResourceBag();
                a.ExampleMethod();

                a = null;

                //GC.Collect();
                //GC.WaitForPendingFinalizers(); // 왼쪽과 같은 Finalizer 호출용 함수도 있다고 하는데, 호출 안하는 것과 큰 차이는 없는 것 같다.
            }
        }

        private static void checkGCFunc(object obj)
        {
            int old = 0;
            int checkCount = 0;

            while (true)
            {
                int count = 0;

                for (int i = 0; i < GC.MaxGeneration; i++)
                {
                    count += GC.CollectionCount(i);
                }

                // GC에서 처리된 새로운 객체 수 화면에 출력
                Console.WriteLine($"{checkCount++} : {(count - old)}");
                old = count;

                Thread.Sleep(1000);
            }
        }
    }


}
