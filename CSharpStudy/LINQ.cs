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
}
