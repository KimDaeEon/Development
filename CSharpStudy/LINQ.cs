using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace LINQ
{
    // 컬렉션을 반환하기보다 이터레이터를 반환하는 것이 낫다
    namespace PreferIteratorMethodsToReturningCollections
    {
        public class Iterator1
        {
            public static IEnumerable<char> GenerateAlphabet()
            {
                var letter = 'a';
                while (letter <= 'z')
                {
                    yield return letter;
                    letter++;
                }
            }

            // 위와 같이 코드를 작성하면, 아래와 유사하게 컴파일러가 코드를 만들어준다.
            class EmbeddedIterator : IEnumerable<char>
            {
                public IEnumerator<char> GetEnumerator() => new LetterEnumerator();

                IEnumerator IEnumerable.GetEnumerator() => new LetterEnumerator();

                public static IEnumerable<char> GenerateAlphabet() => new EmbeddedIterator();

                private class LetterEnumerator : IEnumerator<char>
                {
                    // 이렇게 LetterEnumerator 내부에 letter를 저장해두고, 아래 MoveNext()와 Current 속성을 통해 foreach 문을 돌 때에
                    // 저장해둔 letter 값을 그때 그때 생성해서 전달해주는 것이다.
                    private char letter = (char)('a' - 1);

                    // 이로 인해 Collection으로 전달했을 때보다 메모리를 더 적게 사용할 수 있다.
                    // 이에 더해 IEnumerable은 ToList() 같은 메소드가 있어서 Collection으로 변경하기도 쉽다.
                    // 원하는 객체만 필요할 때에 생성하거나, 원한다면 전체 객체에 대한 Collection을 사용할 수도 있으니,
                    // Collection보다 Iterator를 반환하는 것이 사용자 입장에서 더 유연하다는 것이다.
                    public bool MoveNext()
                    {
                        letter++;
                        return letter <= 'z';
                    }

                    public char Current => letter;

                    object IEnumerator.Current => letter;

                    public void Reset() => letter = (char)('a' - 1);

                    void IDisposable.Dispose() { }
                }

            }

        }

        // 아래처럼 만들면 GenerateAlphabetSubset을 한 후에는 문제가 있는지 모르고, 나중에 이것을 foreach 문에서 사용해서 MoveNext()가 호출될 때에 문제가 생긴다.
        public class Iterator2
        {
            public static IEnumerator<char> GenerateAlphabetSubset(char first, char last)
            {
                // 이 부분에 대한 체크는 MoveNext()에서 처리된다. 이 코드로 인해 생성되는 코드는 아래 EmbeddedSubsetIterator를 참고
                if (first < 'a')
                {
                    throw new ArgumentException("first must be at least the letter a", nameof(first));
                }

                if (first > 'z')
                {
                    throw new ArgumentException("first must be no greater than z", nameof(first));
                }

                if (last < first)
                {
                    throw new ArgumentException("last must be at least as large as first", nameof(last));
                }

                if (last > 'z')
                {
                    throw new ArgumentException("last must not be past z", nameof(last));
                }

                var letter = first;

                while (letter <= last)
                {
                    yield return letter;
                    letter++;
                }
            }

            public class EmbeddedSubsetIterator : IEnumerable<char>
            {
                private readonly char first;
                private readonly char last;

                public EmbeddedSubsetIterator(char first, char last)
                {
                    this.first = first;
                    this.last = last;
                }

                public IEnumerator<char> GetEnumerator() => new LetterEnumerator(first, last);

                IEnumerator IEnumerable.GetEnumerator() => new LetterEnumerator(first, last);

                public static IEnumerable<char> GetnerateAlphabetSubset(char first, char last) => new EmbeddedSubsetIterator(first, last);

                private class LetterEnumerator : IEnumerator<char>
                {
                    private readonly char first;
                    private readonly char last;

                    private bool isInitialized = false;

                    public LetterEnumerator(char first, char last)
                    {
                        this.first = first;
                        this.last = last;
                    }

                    private char letter = (char)('a' - 1);

                    public bool MoveNext()
                    {
                        if (!isInitialized)
                        {
                            if (first < 'a')
                            {
                                throw new ArgumentException("first must be at least the letter a", nameof(first));
                            }

                            if (first > 'z')
                            {
                                throw new ArgumentException("first must be no greater than z", nameof(first));
                            }

                            if (last < first)
                            {
                                throw new ArgumentException("last must be at least as large as first", nameof(last));
                            }

                            if (last > 'z')
                            {
                                throw new ArgumentException("last must not be past z", nameof(last));
                            }

                            letter = (char)(first - 1);
                        }

                        letter++;
                        return letter <= last;
                    }

                    public char Current => letter;
                    object IEnumerator.Current => letter;
                    public void Reset() => isInitialized = false;
                    void IDisposable.Dispose() { }

                }
            }
        }

        // 그래서 아래처럼 만들어야 처음에 GenerateAlphabatSubset 함수 부분에서 exception이 발생한다.
        // Iterator를 리턴할 때에 체크해야할 조건이 있다면 아래와 같이 만들어서 Iterator 사용 전에 문제 사항을 알 수 있도록 하자.
        public class Iterator3
        {
            // 실제 문자를 생성하는 IEnumerable<char>는 GenerateAlphabetSubsetImpl에서 생성하고, 여기서는 조건만 체크한다.
            public static IEnumerable<char> GenerateAlphabetSubset(char first, char last)
            {
                if (first < 'a')
                {
                    throw new ArgumentException("first must be at least the letter a", nameof(first));
                }

                if (first > 'z')
                {
                    throw new ArgumentException("first must be no greater than z", nameof(first));
                }

                if (last < first)
                {
                    throw new ArgumentException("last must be at least as large as first", nameof(last));
                }

                if (last > 'z')
                {
                    throw new ArgumentException("last must not be past z", nameof(last));
                }

                return GenerateAlphabetSubsetImpl(first, last);
            }

            public static IEnumerable<char> GenerateAlphabetSubsetImpl(char first, char last)
            {
                var letter = first;
                while (letter <= last)
                {
                    yield return letter;
                    letter++;
                }
            }
        }
    }

    // 시퀀스에 사용할 수 있는 조합 가능한 API를 작성하라
    namespace CreateComposableAPIsForSequences
    {
        // 아래처럼 Input으로 시퀀스를 받고 시퀀스를 리턴하는 함수를 만들라는 것이다.
        // 그래야지 Unique(Square(a))와 같이 함수를 중첩해서 지연 실행할 수 있다.
        public class foo
        {
            // Square과 Unique는 조합되어서 '지연연산'이 되어 저장공간이 따로 필요 없고, 시퀀스 순회 자체도 1번만 하게 된다.
            // 시퀀스란 IEnumerable<T> 인터페이스 또는 IQueryable<T> 인터페이스를 구현하는 형식의 개체를 의미한다.
            public static IEnumerable<T> Unique<T>(IEnumerable<T> nums)
            {
                var uniqueVals = new HashSet<T>();
                Console.WriteLine("Entering Unique");

                foreach (var num in nums)
                {
                    if (!uniqueVals.Contains(num))
                    {
                        uniqueVals.Add(num);
                        Console.WriteLine("yield return Unique");
                        yield return num;
                        Console.WriteLine("re-entering Unique after yield return");
                    }
                }

                Console.WriteLine("Exiting Unique");
            }

            public static IEnumerable<int> Square(IEnumerable<int> nums)
            {
                Console.WriteLine("Entering Square");
                foreach (var num in nums)
                {
                    Console.WriteLine("yield return Sqaure");
                    yield return num * num;
                    Console.WriteLine("re-entering Square after yield return");
                }

                Console.WriteLine("Exiting Square");
            }
        }
    }

    // 바인딩된 변수는 수정하지 말라
    namespace AvoidModifyingBoundVariables
    {
        // 람다 표현식이 인스턴스 변수나 지역변수에 전혀 접근하지 않는 경우 코드 생성 예제
        public class temp1
        {

            public void foo()
            {
                int[] someNumbers = { 0, 1, 2, 3, 4, 5 };
                var answers = from n in someNumbers select n * n;
            }

            // 컴파일러가 위의 코드를 실행시키기 위해 만들어내는 코드
            private static int HiddenFunc(int n) => (n * n); 
            private static Func<int, int> HiddenDelegateDefinition; 
            public void foo2()
            {
                int[] someNumbers = new int[] { 0, 1, 2, 3, 4, 5 };
                if(HiddenDelegateDefinition == null)
                {
                    HiddenDelegateDefinition = new Func<int, int>(HiddenFunc); // 이렇게 Delegate가 정적으로 만들어져서 사용되는 것 명심
                }

                var answers = someNumbers.Select<int, int>(HiddenDelegateDefinition);
            }
        }

        // 람다 표현식이 인스턴스 변수에 접근, 지역변수에는 접근하지 않는 경우 코드 생성 예제
        public class temp2
        {
            public class ModFilter
            {
                private readonly int modulus;

                public ModFilter(int mod)
                {
                    modulus = mod;
                }

                public IEnumerable<int> FindValues(IEnumerable<int> sequence)
                {
                    return from n in sequence
                           where n % modulus == 0 // 인스턴스 변수를 참조하게 되었다.
                           select n * n; // 이전 예제와 이 부분은 동일
                }
            }

            // 위의 코드로 인해 컴파일러가 생성하는 코드
            public class ModFilterGeneratedByCompiler
            {
                private readonly int modulus;

                public ModFilterGeneratedByCompiler(int mod)
                {
                    modulus = mod;
                }

                // 새롭게 추가된 인스턴스 메서드, modulus가 멤버변수여서 그것을 참조하기 위해 인스턴스용 멤버 함수가 만들어졌다.
                private bool WhereClause(int n) => ((n % this.modulus) == 0);

                // 기존 메서드
                private static int HiddenFunc(int n) => (n * n);

                // 기존 델리게이트
                private static Func<int, int> HiddenDelegateDefinition;

                public IEnumerable<int> FindValues(IEnumerable<int> sequence)
                {
                    if (HiddenDelegateDefinition == null)
                    {
                        HiddenDelegateDefinition = new Func<int, int>(HiddenFunc);
                    }

                    // 새롭게 추가된 인스턴스 메서드 WhereClause가 쓰이게 되었다.
                    return sequence.Where<int>(
                        new Func<int, bool>(this.WhereClause)).Select<int, int>(HiddenDelegateDefinition);
                }
            }
        }

        // 람다 표현식이 인스턴스 변수와 지역 변수 모두 접근하는 경우 코드 생성 예제
        public class temp3
        {
            public class ModFilter
            {
                private readonly int modulus;

                public ModFilter(int mod)
                {
                    modulus = mod;
                }

                public IEnumerable<int> FindValues(IEnumerable<int> sequence)
                {
                    int numValues = 0;
                    return from n in sequence
                           where n % modulus == 0 // 이전 예전 부분과 동일
                           select n * n / ++numValues; // 여기서 지역 변수를 사용하는 것이 위의 예제와 다르다.
                }
            }

            // 위의 코드로 인해 컴파일러가 생성하는 코드
            public class ModFilterGeneratedByCompiler
            {
                // 기존 클래스 안에 Nested Class인 Closure가 생성이 되고,
                // 지역변수와 앞서 만들었던 HiddenFunction도 이 클래스에 포함된다.
                private sealed class Closure
                {
                    public ModFilterGeneratedByCompiler outer;
                    public int numValues;

                    public int SelectClause(int n) => ((n * n) / ++this.numValues);
                }

                private readonly int modulus;

                public ModFilterGeneratedByCompiler(int mod)
                {
                    this.modulus = mod;
                }

                private bool WhereClause(int n) => ((n % this.modulus) == 0);

                public IEnumerable<int> FindValues(IEnumerable<int> sequence)
                {
                    var c = new Closure();
                    c.outer = this; // 추후에 Closure 내부에서 다른 참조가 필요할 때에 사용

                    // 아래 부분이 정말 중요, C++ 람다처럼 값이 복사되어서 사용되는 것이 아니다.
                    // 지역 변수가 캡쳐된 블럭에서 발생한 지역 변수에 대한 수정 사항이 람다 함수에 그대로 반영된다고 보면 된다.
                    // 그래서 람다 함수에 캡쳐된 변수는 수정하지 않는 것이 좋다.
                    c.numValues = 0; 

                    return sequence.Where<int>(
                        new Func<int, bool>(this.WhereClause))
                        .Select<int, int>(new Func<int, int>(c.SelectClause));
                }
            }
        }

        
    }
}
