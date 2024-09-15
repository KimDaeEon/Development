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
using LINQ.AvoidModifyingBoundVariables;
using System.Diagnostics;
using System.Threading.Tasks;
using System.Runtime.InteropServices;
using System.Buffers;

namespace CSharpStudy
{
    #region  Decouple Iterations from Actions, Predicates, and Functions
    public static class MyExtension
    {
        // LINQ에서 사용하는 Where이 아래와 유사하다고 보면 된다.
        // 순회방식(시퀀스)과 Predicate(Function, Action도 넓게 보면)이 분리되어서 작동됨을 알 수 있다.
        public static IEnumerable<T> MyWhere<T>(this IEnumerable<T> sequence, Predicate<T> filterFunc)
        {
            if (sequence == null)
            {
                throw new ArgumentNullException(nameof(sequence), "sequence must not be null");
            }

            if (filterFunc == null)
            {
                throw new ArgumentNullException("Predicate mut not be null");
            }

            foreach (T item in sequence)
            {
                if (filterFunc(item))
                {
                    yield return item;
                }
            }
        }

        public static IEnumerable<T> MySelect<T>(this IEnumerable<T> sequence, Func<T, T> func)
        {
            if (sequence == null)
            {
                throw new ArgumentNullException(nameof(sequence), "sequence must not be null");
            }

            foreach (T element in sequence)
            {
                yield return func(element);
            }
        }

        //List<int> li = new List<int>() { 1, 2, 3, 4 };

        //foreach (var item in MyWhere(MySelect(li, (item)=>item*item), (item) => item % 2 == 0))
        //{
        //    Console.WriteLine(item);
        //}
    }
    #endregion

    public class ExceptiontTest
    {
        public static bool LogException(Exception e)
        {
            Console.WriteLine();
            Console.WriteLine("Logging..");
            Console.WriteLine(e);
            Console.WriteLine();

            return false;
        }

        public static void MakeException(int a)
        {
            if (a == 1)
            {
                throw new ArgumentException("ArgumentException");
            }
            else
            {
                throw new TimeoutException("TimeOutException");
            }
        }
    }


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

        #region new vs override
        public class b
        {
            public virtual void foo()
            {
                Console.WriteLine("b foo()");
            }
        }

        public class d1 : b
        {
            public new void foo() 
            {
                Console.WriteLine("new d1 foo()");
            }
        }
        
        public class d2 : b
        {
            public override void foo() 
            {
                Console.WriteLine("override d2 foo()");
            }
        }
        #endregion

        #region ResourceManagementTest
        public class ManagedResource
        {
            public byte[] a;

            public ManagedResource()
            {
                // 1MB 크기의 배열을 초기화
                a = new byte[1024 * 1024];
                Console.WriteLine("ManagedResource 생성자 호출됨");
            }
        }

        [StructLayout(LayoutKind.Sequential)]
        public struct UnmanagedResource
        {
            [MarshalAs(UnmanagedType.ByValArray, SizeConst = 1024 * 1024)]
            public byte[] field1;
        }

        #endregion

        static void Main(string[] args)
        {
            #region new vs override
            //// 아래 코드를 실행해보면 override와 new의 차이를 알 수 있다.
            //b t1 = new d1();
            //b t2 = new d2();

            //t1.foo();
            //t2.foo();
            #endregion

            // 예외 필터의 다른 활용 예를 살펴보자
            #region Leverage Side Effects in Exception Filters
            //int input = int.Parse(Console.ReadLine());
            //try
            //{
            //    ExceptiontTest.MakeException(input);
            //}
            //// 아래를 보면 예외 필터를 써서 모든 Exception에 대해서 로깅을 한다.
            //// catch 안의 내용은 when 내부가 false이면 실행이 안되고 다음 catch문으로 넘어가서 처리된다는 것도 알아두자.
            //// 아래처럼 작성하면 Debugging 시에는 로그가 뜨고, 디버깅이 아닐 때에는 로그가 안나오게 할 수 있다.
            //catch(Exception e) when ((System.Diagnostics.Debugger.IsAttached) && ExceptiontTest.LogException(e) )
            //{
            //    Console.WriteLine("Can't happen");
            //}
            //catch (ArgumentException e)
            //{
            //    Console.WriteLine("Do something for AE");
            //}

            //catch (TimeoutException e)
            //{
            //    Console.WriteLine("Do something for TE");
            //}
            //// 맨 위의 코드와는 반대로 앞에서 처리되지 않은 예외에 대해서 로깅하는 방법
            ////catch (Exception e) when (ExceptiontTest.LogException(e))
            ////{
            ////    Console.WriteLine("Can't happen");
            ////}
            #endregion

            #region Avoid Modifying Bound Variables
            //// 람다 표현식에서 인스턴스 변수만 접근하는 경우
            //List<int> li = new List<int>() { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
            //{
            //    temp2.ModFilter a = new temp2.ModFilter(5);
            //    temp2.ModFilterGeneratedByCompiler b = new temp2.ModFilterGeneratedByCompiler(5);

            //    foreach (var i in a.FindValues(li))
            //    {
            //        Console.WriteLine(i);
            //    }

            //    foreach (var i in b.FindValues(li))
            //    {
            //        Console.WriteLine(i);
            //    }
            //}

            //// 람다 표현식에서 인스턴스 변수 + 지역 변수 모두 접근하는 경우
            //{
            //    temp3.ModFilter a = new temp3.ModFilter(5);
            //    temp3.ModFilterGeneratedByCompiler b = new temp3.ModFilterGeneratedByCompiler(5);

            //    foreach (var i in a.FindValues(li))
            //    {
            //        Console.WriteLine(i);
            //    }

            //    foreach (var i in b.FindValues(li))
            //    {
            //        Console.WriteLine(i);
            //    }
            //}
            #endregion

            #region  Decouple Iterations from Actions, Predicates, and Functions
            //List<int> a = new List<int>() { 1, 2, 3, 4 };

            //// LINQ 구문을 쓰면 아래와 같이 컴파일러가 확장 함수가 바꿔준다고 보면 된다.
            //foreach (var i in a.MyWhere(item => item % 2 == 0).MySelect(item => item * item))
            //{
            //    Console.WriteLine(i);
            //}
            #endregion

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

            #region ResourceManagementTest
            //int iterations = 10000;

            //// ObjectPool을 이용해 SomeResource를 관리
            //ObjectPool<ManagedResource> resourcePool = new ObjectPool<ManagedResource>(10);

            //// 시간 측정을 위한 Stopwatch 시작
            //Stopwatch stopwatch = Stopwatch.StartNew();

            //for (int i = 0; i < iterations; i++)
            //{
            //    // 풀에서 객체를 가져옴 (Pop)
            //    ManagedResource resource = resourcePool.Pop();

            //    // 사용 후 객체를 풀에 반환 (Push)
            //    resourcePool.Push(resource);
            //}

            //// 시간 측정 종료
            //stopwatch.Stop();

            //Console.WriteLine($"C# Object pool test completed in {stopwatch.Elapsed.TotalSeconds} seconds.");

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
