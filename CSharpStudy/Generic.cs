using System;
using System.Collections.Generic;
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

}
