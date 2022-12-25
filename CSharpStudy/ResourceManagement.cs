using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace ResourceManagement
{
    // 비관리 메모리 사용 용도 클래스
    // 아래 코드를 보면 Finalizer가 없다. Struct는 Stack에서 사용되는 것이기에 Finalizer 구현을 막아두었다.
    // 아래 링크 참조, 요약하자면 "GC가 할당한 것은 GC가 해제하고, Stack에 할당된 것은 나중에 재사용할 때에 다시 덮어쓸 것이므로 그대로 둔다."
    // https://stackoverflow.com/questions/2146434/destroying-a-struct-object-in-c

    // ref struct는 struct를 stack에만 할당하게 하는 제약을 주기 위해 만들어졌다.
    // 자세한 정보는 링크로.. https://learn.microsoft.com/en-us/dotnet/csharp/language-reference/builtin-types/ref-struct
    public unsafe ref struct NativeMemory<T> where T : unmanaged 
    {
        int _size;
        IntPtr _ptr;

        public NativeMemory(int size)
        {
            _size = size;

            long lSize = _size;
            lSize *= sizeof(T);

            IntPtr bufSize = new IntPtr(lSize);
            _ptr = Marshal.AllocHGlobal(bufSize);
        }

        public Span<T> GetSpan()
        {
            return new Span<T>(_ptr.ToPointer(), _size);
        }

        public void Dispose()
        {
            if(_ptr == IntPtr.Zero)
            {
                return;
            }

            Marshal.FreeHGlobal(_ptr);
            _ptr = IntPtr.Zero;
        }
    }

    // 관리 클래스를 만들 경우 지켜야할 표준 Dispose 
    public class ResourceBag : IDisposable
    {
        private bool disposed = false;

        public void Dispose()
        {
            Dispose(true);

            // Finalizer 발동 억제
            GC.SuppressFinalize(this);
        }

        protected virtual void Dispose(bool isDisposing)
        {
            Console.WriteLine("Dispose(bool) 호출");

            if (disposed)
            {
                return;
            }

            if (isDisposing)
            {
                // 관리 리소스 정리
            }

            // 비관리 리소스 정리

            disposed = true;
        }

        public void ExampleMethod()
        {
            if (disposed)
            {
                throw new ObjectDisposedException($"{this.GetType()} object is used when disposed.");
            }
            else
            {
                //Console.WriteLine("some operation");
            }
        }

        // 되도록 아래는 최후의 보루로 생각하고 의존하지 말자.
        // https://stackoverflow.com/questions/59654198/c-sharp-out-of-scope-objects-not-getting-collected/59654357#59654357
        ~ResourceBag()
        {
            Dispose(false);
        }
    }

    public class DerivedRsourceBag : ResourceBag
    {
        // 파생 클래스도 자신만의 disposed 플래그를 관리하는 것이 중요.
        private bool disposed = false;

        protected override void Dispose(bool isDisposing)
        {
            if (disposed)
            {
                return;
            }

            if (isDisposing)
            {
                // 관리 리소스 정리
            }

            // 비관리 리소스 정리


            // 이 부분에서 GC.SuppressedFinalize() 처리
            base.Dispose(isDisposing);

            disposed = true;
        }

        ~DerivedRsourceBag()
        {
            // 비관리 리소스 정리
            Dispose(false);
        }
    }
}
