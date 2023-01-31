using Generic.DoNotCreateGenericSpecializationOnBaseClassesOrInterfaces;
using Generic.IComparableStandard;
using Generic.UseDelegatesToDefineMethodConstraintsOnTypeParameters;
using Generic.AugmentMinimalInterfaceContractsWithExtensionMethods;
using ResourceManagement;
using System;
using System.Collections.Generic;
using System.Threading;
using System.Collections;
using System.Linq;
using LINQ.PreferIteratorMethodsToReturningCollections;
using LINQ.CreateComposableAPIsForSequences;
using System.Diagnostics;

namespace CSharpStudy
{
    class Program
    {
        #region Do Not Create Generic Specialization on Base Classes Or Interfaces
        // 이 타입이 있다고 해서 아래에서 MyDerived 인스턴스에 대해 이 함수가 호출되지 않고, 아래의 Generic이 호출된다.
        // https://social.msdn.microsoft.com/Forums/en-US/f7e35e9e-7622-4154-bc8d-ed6af180c669/can-a-generic-method-be-specialized?forum=csharplanguage
        // 아래와 같이 해두면 MyBase를 상속받은 클래스는 다 여기서 불릴 것 같지만.. 결과는 그렇지 않다.
        //public static void WriteMessage(MyBase b)
        //{
        //    Console.WriteLine("WriteMessage(MyBase)");
        //}

        //// WriteMessage(MyDerived)가 없다면 Generic으로 코드가 정확하게 MyDerived를 매개변수로 받는 WriteMessage가 생성되어서 이 함수가 호출된다.
        //// MyBase라는 기반클래스로 Generic이 특화되지 않는다는 점에 주목하자.
        //public static void WriteMessage<T>(T obj)
        //{
        //    Console.WriteLine("WriteMessage<T>(T)");
        //    Console.WriteLine(obj.ToString());

        //    // 쓰고 싶으면 체크해야할 타입이 적은 경우 아래와 같이 구현해서 어떤 함수를 호출할 지 정할 수 있다.
        //    //if (obj is MyBase)
        //    //{
        //    //    WriteMessage(obj as MyBase);
        //    //}
        //    //else if (obj is IMessageWriter)
        //    //{
        //    //    WriteMessage((IMessageWriter)obj);
        //    //}
        //    //else
        //    //{
        //    //    Console.WriteLine(obj.ToString());
        //    //}
        //}

        //// 아래도 위의 void WriteMessage(MyBase b)와 마찬가지로 하지 말아야할 방식이다.
        //public static void WriteMessage(IMessageWriter obj)
        //{
        //    Console.WriteLine("WriteMessage(IMessageWriter obj)");
        //    obj.WriteMessage();
        //}
        #endregion


        static void Main(string[] args)
        {
            #region Create Composable APIs for Sequences
            //List<int> a = new List<int>() { 1, 2, 3, 4 };
            //var i = foo.Square(foo.Unique(a));

            //foreach (var n in i)
            //{
            //    Console.WriteLine(n);
            //}
            #endregion

            #region Prefer Iterator Methods to Returning Collections
            ////var it = Iterator3.GenerateAlphabetSubset('1', 'b'); // 실행 전 에러 출력
            //var it = Iterator3.GenerateAlphabetSubset('a', 'b');

            //foreach(var item in it)
            //{
            //    Console.WriteLine(item);
            //}

            #endregion

            #region Augment Minimal Interface Contracts with Extension Methods
            //MyType a = new MyType();
            //a.Foo();
            //a.ExtendedFunction(); // 해당 타입의 ExtendedFunction이 호출, 이런 식으로 구현하면 안된다.
            //a.ExtendedFunction<MyType>();
            //((IFoo)a).ExtendedFunction(); // 위와 같은 확장 메서드 호출
            #endregion

            #region Do Not Create Generic Specialization on Base Classes Or Interfaces
            //MyDerived d = new MyDerived();
            //WriteMessage(d);
            //Console.WriteLine();

            //WriteMessage((IMessageWriter)d);
            //Console.WriteLine();

            //WriteMessage((MyBase)d);
            //Console.WriteLine();

            //AnotherType another = new AnotherType();
            //WriteMessage(another);
            //Console.WriteLine();

            //WriteMessage((IMessageWriter)another);
            #endregion

            #region Use Delegates to Define Method Constraints on Type Parameters
            //double[] xValues = { 1, 2, 3, 4 };
            //double[] yValues = { 1, 2, 3, 4 };

            //// Point가 defualt 생성자가 없으므로 Zip 함수와 Func를 이용해서 Point를 생성한다.
            //List<Point> values = new List<Point>(Utilities.Zip(xValues, yValues, (x, y) => new Point(x, y)));
            #endregion

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
