using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Generic
{
    namespace IComparableStandard
    {
        // IComparable을 쓰면 Object로만 받을 수 있어서 부차적인 연산이 필요하다. 구버전 호환이 필요없다면 IComparable<T>를 쓰자.
        public class Temp : IComparable<Temp>, IComparable
        {
            public int A { get; set; }
            public int B { get; set; }

            public int CompareTo(Temp other)
            {
                Console.WriteLine("Temp other");
                return A.CompareTo(other.A);
            }

            // IComparable을 위한 것, .NET Framework 2.0 이전 개발 코드에서 사용하려면 IComparable이 있어야 한다.
            public int CompareTo(object obj)
            {
                Console.WriteLine("object other");
                if (!(obj is Temp))
                {
                    throw new ArgumentException("Argument is not a Temp", nameof(obj));
                }

                Temp otherTemp = (Temp)obj;

                return this.CompareTo(otherTemp);
            }

            // CompareTo와의 일관성을 위해서 관계 연산자도 오버로딩을 해야한다.
            public static bool operator <(Temp left, Temp right) => left.CompareTo(right) < 0;
            public static bool operator <=(Temp left, Temp right) => left.CompareTo(right) <= 0;
            public static bool operator >(Temp left, Temp right) => left.CompareTo(right) > 0;
            public static bool operator >=(Temp left, Temp right) => left.CompareTo(right) >= 0;


            // C# 에서 Singleton을 구현할 때에 쓰는 문법이다. Lazy Initialization에서 유래되었다.
            private static Lazy<BComparer> bComp = new Lazy<BComparer>(() => new BComparer());
            public static IComparer<Temp> BCompare => bComp.Value;

            // IComparable로 정의한 것 이외에 추가적으로 무언갈 비교하고 싶을 때에 IComparer를 사용하면 된다.
            private class BComparer : IComparer<Temp>
            {
                int IComparer<Temp>.Compare(Temp left, Temp right) => left.B.CompareTo(right.B);
            }

            // Func와 유사한데, 같은 타입 인자 2개를 받아서 int를 결과로 리턴하는 delegate 표준이다.
            public static Comparison<Temp> CompareByB => (left, right) => left.B.CompareTo(right.B);
        }
    }

    // 타입 매개변수가 IDisposable을 구현한 경우를 대비
    namespace PrepareIDisposableType
    {
        public interface IEngine
        {
            void DoWork();
        }

        public class EngineDriver1<T> where T : IEngine, new()
        {
            public void GetThingsDone()
            {
                T driver = new T();

                // Generic 사용 시에 T 타입이 IDisposable을 구현했는지를 고려해야한다.
                // 아래처럼 using문을 사용하면 IDisposable이 구현되었으면 Dispose()를 호출하고,
                // 구현되지 않았으면 아래의 driver.DoWork(); 부분만 실행이 된다.
                using (driver as IDisposable)
                {
                    driver.DoWork();
                }
            }
        }

        // 타입 매개변수가 IDisposable을 구현할 것을 가정할 경우, 아래처럼 위에보다 처리가 더 복잡하다.
        // sealed가 안붙으면 ResourceManagement에 나온 내용대로 표준 IDisposable 구현 방식을 따라야 한다. (protected Dispose(), Finalizer() ..)
        public sealed class EngineDriver2<T> : IDisposable where T : IEngine, new()
        {
            // 아래와 같이 T타입을 만들어주는 Func<T> 를 넣어야 한다. 후에 Value 속성이 호출되면 그 때에 초기화되어 사용된다.
            // 생성 작업이 오래 걸릴 경우 이런 식으로 지연된 초기화를 사용한다.
            private Lazy<T> driver = new Lazy<T>(() => new T());

            public void GetThingsDone() => driver.Value.DoWork();

            public void Dispose()
            {
                if (driver.IsValueCreated) // driver.Value가 호출되어 초기화가 되었다면 해당 플래그가 1로 설정이 된다.
                {
                    var resource = driver.Value as IDisposable;
                    resource?.Dispose();
                }
            }
        }

        // T에서 Dispose 관련 내용을 알아서 처리하도록 한 방식
        public sealed class EngineDriver3<T> where T : IEngine
        {
            private T driver; // null로 초기화된다.

            // IEngine을 구현한 타입 T가 DoWork()에석 Dispose를 책임지도록 한다.
            // 그러면 EngineDriver2<T>처럼 복잡하게 구현하지 않아도 된다.
            public EngineDriver3(T driver)
            {
                this.driver = driver;
            }

            public void GetTHingsDone()
            {
                driver.DoWork();
            }
        }
    }

    // 타입 매개변수에 대해 메서드 제약조건을 설정할 때에는 델리게이트를 활용
    namespace UseDelegatesToDefineMethodConstraintsOnTypeParameters
    {
        public static class Example
        {
            // Func<T, T, T>를 써서 T타입에 대한 Add함수 구현을 외부에서 하도록 강제하였다.
            // 이렇게 한다면 T에 대해 인터페이스 기반 제약조건을 설정하지 않아도 된다.
            public static T Add<T>(T left, T right, Func<T, T, T> AddFunc) => AddFunc(left, right);
        }

        public class Point
        {
            public double X { get; }
            public double Y { get; }

            // 실수를 받는 생성자
            public Point(double x, double y)
            {
                this.X = x;
                this.Y = y;
            }



            // 아래와 같이 사용하면 interface를 만들어서 T에 대한 제약조건을 주지 않아도, 특정 인자를 반환하는 조건을 강제할 수 있다.
            // 파일 입출력을 처리하는 생성자
            public Point(System.IO.TextReader reader)
            {
                string line = reader.ReadLine();
                string[] fields = line.Split(',');

                if (fields.Length != 2)
                {
                    throw new InvalidOperationException("Input format incorrect");
                }

                double value;
                if (!double.TryParse(fields[0], out value))
                {
                    throw new InvalidOperationException("Could not parse X value");
                }
                else
                {
                    X = value;
                }

                if (!double.TryParse(fields[1], out value))
                {
                    throw new InvalidOperationException("Could not parse Y value");
                }
                else
                {
                    Y = value;
                }
            }
        } // class Point

        public class Utilities
        {
            public static IEnumerable<TOutput> Zip<T1, T2, TOutput>(
                IEnumerable<T1> left, IEnumerable<T2> right,
                Func<T1, T2, TOutput> generator
                )
            {
                IEnumerator<T1> leftSequence = left.GetEnumerator();
                IEnumerator<T2> rightSequence = right.GetEnumerator();

                while (leftSequence.MoveNext() && rightSequence.MoveNext())
                {
                    yield return generator(leftSequence.Current, rightSequence.Current);
                }

                leftSequence.Dispose();
                rightSequence.Dispose();
            }
        } // class Utilities
    }

    namespace DoNotCreateGenericSpecializationOnBaseClassesOrInterfaces
    {
        public class MyBase
        {
        }

        public interface IMessageWriter
        {
            void WriteMessage();
        }

        public class MyDerived : MyBase, IMessageWriter
        {
            void IMessageWriter.WriteMessage() =>
                Console.WriteLine("MyDerived.WriteMessage");
        }

        public class AnotherType : IMessageWriter
        {
            public void WriteMessage() =>
                Console.WriteLine("AnotherType.WriteMessage");
        }
    }

    // 인터페이스는 간략히 정의하고 기능의 확장은 확장 메서드를 사용하라
    namespace AugmentMinimalInterfaceContractsWithExtensionMethods
    {
        public interface IFoo
        {
            public void Foo();
        }

        public class MyType : IFoo
        {
            public void Foo()
            {
                Console.WriteLine("MyType.Foo()");
            }

            // 아래와 같이 구현하면 안된다. Extension 함수와 겹치게 되면 예상치 못한 동작을 할 수 있다.
            public void ExtendedFunction()
            {
                Console.WriteLine("MyType.ExtendedFunction()");
            }
        }

        // IFoo를 상속받은 클래스의 기존 기능을 조합해서 새로운 기능을 만들 수 있다면,
        // IFoo에 구현해야할 함수를 추가하지말고 확장 메서드를 통해 기능을 추가하는 것이 낫다.
        public static class FooExtension
        {
            public static void ExtendedFunction<T>(this T input) where T : IFoo
            {
                Console.WriteLine("FooExtension.ExtendedFunction()");
            }
        }
    }

    // 확장 메서드를 이용하여 구체화된 제네릭 타입을 개선하라
    namespace ConsiderEnhancingConstructedTypesWithExtensionMethods
    {
        public static class Enumerable
        {
            // 이런 식으로 IEnumerable<int>라는 구체화된 제네릭 타입에 '평균' 기능을 추가할 수 있는 것이다.
            // 이렇게 하면 상속받으면서 이것저것 따로 구현할 필요도 없고, LINQ 사용 시에도 일관성있게 IEnumerable<int> 형태로 사용할 수 있다.
            public static int Average(this IEnumerable<int> sequence)
            {
                int sum = 0;
                foreach (int i in sequence)
                {
                    sum += i;
                }

                return sum / sequence.Count();
            }
        }
    }

} // namespace Generic
