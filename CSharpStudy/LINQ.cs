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
                if(first < 'a')
                {
                    throw new ArgumentException("first must be at least the letter a", nameof(first));
                }

                if(first > 'z')
                {
                    throw new ArgumentException("first must be no greater than z", nameof(first));
                }

                if(last < first)
                {
                    throw new ArgumentException("last must be at least as large as first", nameof(last));
                }

                if(last > 'z')
                {
                    throw new ArgumentException("last must not be past z", nameof(last));
                }

                var letter = first;

                while(letter <= last)
                {
                    yield return letter;
                    letter++;
                }
            }

            public class EmbeddedSubsetIterator : IEnumerable<char>
            {
                private readonly char first;
                private readonly char last;

                public EmbeddedSubsetIterator (char first, char last)
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
                            if(first < 'a')
                            {
                                throw new ArgumentException("first must be at least the letter a", nameof(first));
                            }

                            if(first > 'z')
                            {
                                throw new ArgumentException("first must be no greater than z", nameof(first));
                            }

                            if(last < first)
                            {
                                throw new ArgumentException("last must be at least as large as first", nameof(last));
                            }

                            if(last > 'z')
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
                if(first < 'a')
                {
                    throw new ArgumentException("first must be at least the letter a", nameof(first));
                }

                if(first > 'z')
                {
                    throw new ArgumentException("first must be no greater than z", nameof(first));
                }

                if(last < first)
                {
                    throw new ArgumentException("last must be at least as large as first", nameof(last));
                }

                if(last > 'z')
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
}
