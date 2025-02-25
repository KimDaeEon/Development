#pragma once
#include <iostream>
#include <vector>
#include <list>
#include <set>
#include <ranges>
#include <thread>
#include <mutex>
#include <queue>
#include <Windows.h>
#include <future>
#include <stack>
#include <chrono>

#pragma once
using BYTE = unsigned char;
using int8 = __int8;
using int16 = __int16;
using int32 = __int32;
using int64 = __int64;
using uint8 = unsigned __int8;
using uint16 = unsigned __int16;
using uint32 = unsigned __int32;
using uint64 = unsigned __int64;

//#include <boost/type_index.hpp>
using namespace std;

namespace MakeUniquePtr
{
	class Base
	{
	public:
		Base()
		{
			//cout << "Base" << endl;
		}

		~Base()
		{
			//cout << "~Base" << endl;
		}
	};
	class Car : public Base
	{
	public:
		~Car() { cout << "~Car" << endl; }

		void Go() { cout << "Car go" << endl; }

		Car()
		{
			cout << "Car" << endl;
		}

		Car(const Car& c)
		{
			cout << "Const Car&" << endl;
		}

		Car(Car&& c) noexcept
		{
			cout << "Car&&" << endl;
		}
	};

	struct Freer
	{
		void operator()(void* p) const
		{
			cout << "free" << endl;
			free(p);
		}
	};

	template <typename T>
	struct Deleter
	{
		Deleter() = default;

		template<typename U>
		Deleter(const Deleter<U>& d)
		{

		}
		void operator()(T* p) const
		{
			cout << "delete" << endl;
			delete p;
		}
	};

	template <typename T>
	struct Deleter<T[]>
	{
		void operator()(T* p) const
		{
			cout << "delete[]" << endl;
			delete[] p;
		}
	};

	template <typename T, typename D = Deleter<T>> class myUnique
	{

	public:
		T* pobj;
		D del;
		explicit myUnique(T* p) : pobj(p) {}
		myUnique() {}
		myUnique(nullptr_t a) { pobj = nullptr; }
		explicit myUnique(T* p, const D& d) : pobj(p), del(d) {}
		explicit myUnique(T* p, D&& d) : pobj(p), del(move(d)) {}

		myUnique(const myUnique&) = delete;

		template<typename T2, typename D2>
		myUnique(myUnique<T2, D2>&& in)
		{
			pobj = move(in.pobj);
			del = move(in.del);
			in.pobj = nullptr;
		}

		/*template<typename T2, typename D2>
		myUnique& operator=(myUnique<T2, D2>&& up) {

		}*/

		~myUnique()
		{
			if (pobj != nullptr)
			{
				del(pobj);
			}
		}

		T& operator*() const { return *pobj; }
		T* operator->() const { return pobj; }

		explicit operator bool() const { return static_cast<bool>(pobj); }
	};

	template <typename T, typename D> class myUnique<T[], D>
	{

	public:
		T* pobj;
		D del;
		explicit myUnique(T* p) : pobj(p) {}
		myUnique() {}
		myUnique(nullptr_t a) { pobj = nullptr; }
		explicit myUnique(T* p, const D& d) : pobj(p), del(d) {}
		explicit myUnique(T* p, D&& d) : pobj(p), del(move(d)) {}

		myUnique(const myUnique&) = delete;

		template<typename T2, typename D2>
		myUnique(myUnique<T2, D2>&& in)
		{
			pobj = move(in.pobj);
			del = move(in.del);
			in.pobj = nullptr;
		}

		/*template<typename T2, typename D2>
		myUnique& operator=(myUnique<T2, D2>&& up) {

		}*/

		~myUnique()
		{
			if (pobj != nullptr)
			{
				del(pobj);
			}
		}

		T& operator[](int idx) const { return pobj[idx]; }
		T* operator->() const { return pobj; }

		explicit operator bool() const { return static_cast<bool>(pobj); }
	};
}

namespace myVector
{
	class Point
	{

	public:
		void foo()
		{
			cout << "Point::foo" << endl;
		}
		Point(Point&& p) noexcept
		{
			cout << "Point(Point&&)" << endl;
		}
		Point(const Point&& p) noexcept
		{
			cout << "Point(const Point&&)" << endl;
		}

		Point(const Point& p)
		{
			cout << "Point(const Point&)" << endl;
		}

		Point(Point& p)
		{
			cout << "Point(Point&)" << endl;
		}

		Point()
		{
			cout << "Point()" << endl;
		}

		Point(int x, int y) : x(x), y(y)
		{
			cout << "Point(x,y)" << endl;
		}

		~Point()
		{
			cout << "~Point()" << endl;
		}

	private:
		int x = 0;
		int y = 0;
	};

	template<typename T> class myVec
	{
		T* buff;
		size_t size;
		size_t capacity;

	public:
		myVec(size_t sz) :size(sz), capacity(sz)
		{
			//buff = new T[sz];
			buff = static_cast<Point*>(operator new(sizeof(Point) * sz));

			int i = 0;

			// 메모리 초기화 중에 예외가 발생한다면 메모리 초기화 실행
			try
			{
				for (i = 0; i < sz; i++)
				{
					new(&buff[i]) T;
					//new(&buff[i]) T(value);
				}
			}
			catch (...)
			{
				for (int j = i - 1; j >= 0; --j)
				{
					buff[j].~T();
				}

				operator delete(buff);
				size = 0;
				capacity = 0;
			}
		}

		myVec(size_t sz, const T& value) :size(sz), capacity(sz)
		{
			//buff = new T[sz];
			buff = static_cast<Point*>(operator new(sizeof(Point) * sz));

			int i = 0;

			// 메모리 초기화 중에 예외가 발생한다면 메모리 초기화 실행
			try
			{
				for (i = 0; i < sz; i++)
				{
					new(&buff[i]) T(value);
				}
			}
			catch (...)
			{
				for (int j = i - 1; j >= 0; --j)
				{
					buff[j].~T();
				}

				operator delete(buff);
				size = 0;
				capacity = 0;
			}
		}

		~myVec()
		{
			//delete[] buff;
			for (int j = size - 1; j >= 0; --j)
			{
				buff[j].~T();
			}

			operator delete(buff);
			/*size = 0;
			capacity = 0;*/
		}
	};

	template <typename T> struct MyAlloc
	{
		T* allocate(size_t n)
		{
			T* p = static_cast<T*>(malloc(n * sizeof(T)));
			cout << "allocate, " << "size:" << n << endl;
			return p;
		}

		void deallocate(T* p, size_t n) noexcept
		{
			free(p);
			cout << "deallocate" << endl;
		}

		template<typename ... ARGS> void construct(T* p, ARGS&& ... args)
		{
			new(p) T(forward<ARGS>(args)...);
			cout << "MyAlloc construct" << endl;
		}

		void destroy(T* p)
		{
			p->~T();
			cout << "MyAlloc destroy" << endl;
		}

		using value_type = T;

		template<typename U> MyAlloc(const MyAlloc<U>&) noexcept {}
		MyAlloc() = default;

		template<typename U> struct rebind
		{
			typedef MyAlloc<U> other;
		};
	};

}

namespace ReferenceCounting
{
	class RefCountBase
	{
	protected:
		mutable atomic<int> refCnt{ 0 };

	public:
		void addRef() const
		{
			refCnt.fetch_add(1, std::memory_order_relaxed); // 메모리 순서 바뀌어도 상관x
		}
	};

	template<typename T>
	class RefCount : public RefCountBase
	{
	public:
		RefCount()
		{
			cout << "RefCount()" << endl;
		}

		RefCount(const RefCount& c)
		{
			cout << "RefCount(const RefCount&)" << endl;
		}

		RefCount(RefCount&& c) noexcept
		{
			cout << "RefCount(RefCount&& )" << endl;
		}

		void release() const
		{
			// 이거 위 아래로는 계산 순서 못바꾸게 하는 것.
			// acquire 이 자기 위에 있는거 아래로 못오게 하고, release 가 자기 아래 있는거 위로 못가게 한다.
			int ret = refCnt.fetch_sub(1, std::memory_order_acq_rel);

			if (ret == 1)
			{
				//delete static_cast<T*>(const_cast<RefCount*>(this));
				delete static_cast<const T*>(this);
			}
		}
	};

	template<typename T> class AutoPtr
	{
		T* obj;
	public:
		explicit AutoPtr(T* p = nullptr) :obj(p)
		{
			if (obj)
			{
				obj->addRef();
			}
		}

		AutoPtr(const AutoPtr<T>& ap) : obj(ap.obj)
		{
			if (obj)
			{
				obj->addRef();
			}
		}

		~AutoPtr()
		{
			if (obj)
			{
				obj->release();
			}
		}

	};

	class Truck : public RefCount<Truck>
	{
	public:
		~Truck() { cout << "~Truck()" << endl; }
	};
}

namespace AdvanceTagDispatching
{
	//struct input_iterator_tag {};
	//struct output_iterator_tag {};
	//struct forward_iterator_tag : input_iterator_tag{};
	//struct bidirectional_iterator_tag : forward_iterator_tag{};
	//struct random_access_iterator_tag : bidirectional_iterator_tag{};

	//template<typename T> class vector_iterator {
	//public:
	//	using iterator_category = random_access_iterator_tag; // C++ 11 이후
	//	// typedef random_access_iterator_tag iterator_category; // C++ 11 이전
	//};

	//template<typename T> class list_iterator {
	//public:
	//	using iterator_category = bidirectional_iterator_tag;
	//};

	//template<typename T> struct iterator_traits {
	//	typedef typename T::iterator_category iterator_category;
	//};
	//
	//template<typename T> struct iterator_traits <T*>{
	//	typedef typename random_access_iterator_tag iterator_category;
	//};

	// 반복자의 종류에 따라 다르게 동작하는 advance_imp() 제공
	template<typename T>
	void advance_imp(T& it, typename iterator_traits<T>::difference_type sz, random_access_iterator_tag)
	{
		it = it + sz;
		cout << "advance using +" << endl;
	}

	// 여기서 input_iterator_Tag 가 조상 클래스라서 random_access_iterator_tag 가 아니면 다 여길로 들어온다.
	template<typename T>
	void advance_imp(T& it, typename iterator_traits<T>::difference_type sz, input_iterator_tag)
	{

		while (sz--)
		{
			++it;
		}
		cout << "advance using ++" << endl;
	}

	template<typename T>
	// iterator_traits 에는 difference_type 이란 것이 있어서 + 연산에 사용되는 타입도 저장되어 있다는 것을 알자.
	void xadvance(T& it, typename iterator_traits<T>::difference_type sz)
	{
		// 아래 부분에서 iterator_traits<T> 를 통해서 iterator_category 를 꺼내는 것이 중요하다.배열같은 경우는 T* 형태를 꺼내는 것이 중요!
		advance_imp(it, sz, typename iterator_traits<T>::iterator_category());
	}
}

namespace AdvanceEnableIf
{
	template<typename T>
	enable_if_t<std::is_same_v<typename iterator_traits<T>::iterator_category, std::random_access_iterator_tag>, void >
		xadvance(T& it, typename std::iterator_traits<T>::difference_type sz)
	{
		it = it + sz;
		cout << "xadvance +" << endl;
	}

	template<typename T>
	enable_if_t<!std::is_same_v<typename iterator_traits<T>::iterator_category, std::random_access_iterator_tag>, void >
		xadvance(T& it, typename std::iterator_traits<T>::difference_type sz)
	{
		while (sz--)
		{
			++it;
		}
		cout << "xadvance ++" << endl;
	}
}

namespace AdvanceIfConstexpr
{
	template<typename T>
	void xadvance(T& it, typename std::iterator_traits<T>::difference_type sz)
	{
		// 이렇게 되면 if constexpr이 거짓이면 코드 자체가 생성이 안되어서 문제가 안생긴다. C++ 17부터 이게 도입됨.
		if constexpr (std::is_same_v<iterator_traits<T>::iterator_category, std::random_access_iterator_tag>)
		{
			it = it + sz;
			cout << "xadvance +" << endl;
		}
		else
		{
			while (sz--)
			{
				++it;
			}
			cout << "xadvance ++" << endl;
		}
	}
}

namespace AdvanceConcept
{
	template<typename T>
		requires std::random_access_iterator<T> // 아래 코드 대신해서 똑같은 Concept 가 생겼다.
	//requires std::is_same_v<std::random_access_iterator_tag, typename std::iterator_traits<T>::iterator_category>
	void xadvance(T& it, typename std::iterator_traits<T>::difference_type sz)
	{
		it = it + sz;
		cout << "advance +" << endl;
	}

	template<typename T>
		requires (!std::random_access_iterator<T>)
	// requires 다음에 !을 붙일 때에 괄호 안에 붙여야하는 것을 잊지 말자.
	//requires (!std::is_same_v<std::random_access_iterator_tag, typename std::iterator_traits<T>::iterator_category>)
	void xadvance(T& it, typename std::iterator_traits<T>::difference_type sz)
	{
		while (sz--)
		{
			++it;
		}
		cout << "xadvance ++" << endl;
	}
}

namespace BaseFromMember
{
	struct Buffer
	{
		Buffer(int a)
		{
			cout << "Buffer(int)" << endl;
		}
	};

	class PipeBuffer
	{
	public:
		PipeBuffer()
		{
			cout << "PipeBuffer()" << endl;
		}
	protected:
		Buffer buffer{ 100 };
	};

	struct Stream
	{
		Stream(Buffer& buf)
		{
			cout << "Stream(Buffer& buf)" << endl;
		}
	};

	class PipeStream : public PipeBuffer, public Stream
	{
	public:
		PipeStream() : Stream(buffer)
		{
			cout << "PipeStream()" << endl;
		}
	};
}

namespace IsTransparent
{
	struct People
	{
		string name;
		int age;

		People(string name, int age) : name(name), age(age)
		{

		}
	};

	struct PeopleCompare
	{
		bool operator()(const People& p1, const People& p2) const
		{
			return p1.name < p2.name;
		}

		bool operator()(const People& p1, string name) const
		{
			return p1.name < name;
		}

		bool operator()(string name, const People& p1) const
		{
			return p1.name < name;
		}

		using is_transparent = void;
		//using is_transparent = int; // int 이든 void 이든지 상관없다. find 코드보면 Compare 하는 객체 타입에 is_transparent 가 있도록 강제했다.
	};
}

// Coercion : 강제, 여기서는 '강제 변환'이라는 의미
// 아래 코드를 보면 smartptr 의 템플릿 생성자를 통해서 강제 변환을 하고 있다.
namespace CoercionByMemberTemplate
{
	class Animal {};

	class Dog : public Animal {};

	template<typename T> class smartptr
	{
		T* ptr = nullptr;
	public:
		smartptr() = default;
		smartptr(T* p) : ptr(p) {}

		// 소멸자, 참조 계수 등은 구현 생략 ..

		// smartptr(const smartptr<T>& sp) {} // 기본으로 생성되지만 T 타입이 Animal 이라서 불일치가 발생한다.
		// smartptr(const smartptr<Dog>& sp) {} // Dog 타입에 대해선 받을 수 있지만, 그 외의 타입에 대해선 불가
		template<typename U,
			typename = std::enable_if_t<std::is_convertible_v<U*, T*>> > // enable_if 를 써주면 변환 불가시에 아예 코드가 생성이 안되면서 이유가 더 깔끔하게 나온다.
		smartptr(const smartptr<U>& sp) : ptr(sp.ptr) {} // 이렇개 해주면 임의의 타입에 대해서 받을 수 있고, 상속관계일 때에 포인터 변환이 가능하다.

		template<typename> friend class smartptr; // typename 은 상관이 없다. 템플릿으로 만들어지는 클래스들이 이 클래스의 private 멤버에 접근하도록 하는 용도
	};

	//Dog* p1 = new Dog;
	//Animal* p2 = p1;

	//smartptr<Dog> sp1(new Dog);
	//smartptr<Animal> sp2 = sp1;

	//smartptr<int> sp3(new int);
	//smartptr<double> sp4 = sp3; // int* 가 double* 로 암시적으로 변환되지 못한다.
}

namespace Ranges
{
	template<typename T>
	class view_interface : public std::ranges::view_base
	{

		// 자신을 상속받은 파생클래스 타입으로 변환시킨다. begin()과 end() 같은 것을 꺼내려면 이렇게 CRTP 를 써야한다.
		T& Cast() { return static_cast<T&>(*this); }

	public:
		int size() { return Cast().end() - Cast().begin(); } // const 로 다 만들어야 하지만, 일단 간단하게..
	};

	template<typename T> class take_view : public view_interface<take_view<T>>
	{
		T rng; // 이 부분이 중요, all 함수로 인해서 view 를 감싸는 view 의 복사본인지, view 외의 타입을 감싸는 ref_view 인지가 정해진다.
		size_t count;

	public:
		take_view() = default; // 이것도 view 인지 체크하는 조건이 있다.
		take_view(T r, size_t c) : rng(r), count(c) {}

		auto begin() { return rng.begin(); }
		auto end() { return rng.begin() + count; }
	};

	// template deduction guide, 템플릿 인자를 아래와 같은 규칙으로 추론하라고 알려주는 것이다.
	// 아래 같은 경우 take_view(T& r, size_t c) 생성자가 호출될 때에 T가 rvalue(값) 가 온다면 'all_t 를 통해서 유추된 T' 타입으로 take_view 를 만들어준다.
	template<typename T>
	//take_view(T&& t, size_t)->take_view<remove_cvref_t<decltype(views::all(t))>>; // 이렇게 해야하는데, 그것을 축약한 것이 아래의 all_t<T> 이다.
	take_view(T&& t, size_t) -> take_view<views::all_t<T>>;

	template<typename T> class reverse_view : public view_interface<reverse_view<T>>
	{
		T rng;

	public:
		reverse_view() = default;
		reverse_view(T r) : rng(r) {}

		auto begin() { return make_reverse_iterator(rng.end()); }
		auto end() { return make_reverse_iterator(rng.begin()); }
	};

	// template deduction guide
	template<typename T>
	reverse_view(T&& t) -> reverse_view<views::all_t<T>>;

	// 이미 표준에 아래의 all 이 있다.
	template<typename T> auto all(T&& arg)
	{
		if constexpr (ranges::view<remove_cvref_t<T>>)
		{ // 옆의 concept 를 들어가보면 default 생성자 체크, view_base 상속 여부 체크가 들어있다.
			cout << "view" << endl;

			// view 를 받으면 복사본을 반환, 이를 통해서 take_view(reverse_view(v),3); 같은 임시값을 받는 형태가 가능해진다.
			return remove_cvref_t<T>(arg);
		}
		else
		{
			cout << "not view" << endl;

			// view 가 아닌 vector 같은 것을 받으면 ref_view 로 감싸서 반환
			return ranges::ref_view<remove_cvref_t<T>>(arg);
		}
	}

	//vector v = { 1,2,3,4 };
	//take_view t(v, 2);

	//for (auto e : reverse_view(take_view(v,3))) { // 이제 임시 변수로도 사용이 가능하다.
	//	cout << e << endl;
	//}
}

namespace MultiThreadProgramming
{

	// Critical Section(User-Level Object)은 찾아보니 SpinLock + Event를 활용한 것이라고 한다.
	// https://stackoverflow.com/questions/5158191/choosing-between-critical-sections-mutex-and-spin-locks

	namespace SpinLockTest
	{
		// 아래 같은 경우는 SpinLock과 Sleep이 함께 사용된 코드이다.
		class SpinLock
		{
		public:
			void lock()
			{

				bool expected = false;
				bool desired = true;

				while (_locked.compare_exchange_strong(expected, desired) == false)
				{
					// expected는 reference 이기에 기존 _locked 값으로 계쏙 바뀐다. 그래서 expected를 우리가 원하는 값으로 바꿔줘야 한다.
					expected = false;

					//this_thread::sleep_for(std::chrono::milliseconds(0)); // 0 밀리초
					this_thread::sleep_for(0ms); // 위와 같다, ms가 operator로 정의되어 있음.
					//this_thread::yield();  // sleep_for(0)과 사실상 같다.
				}

				// 아래는 위의 CAS 의사코드
				// 요약하자면 _locked가 들고있는 값은 expected에 넣고, _locked가 expected와 같으면 desired 값으로 _locked를 바꾼다.
				//if (_locked == expected) {
				//	expected = _locked;
				//	_locked = desired;
				//	return true;
				//}
				//else {
				//	expected = _locked;
				//	return false;
				//}
			}

			void unlock()
			{
				//_locked = false;
				_locked.store(false); // atomic이라서 위에처럼 해도 상관없긴 하다.
			}

		private:
			atomic<bool> _locked = false;
		};


		const int Limit = 1000000;
		int sum = 0;
		SpinLock sl;

		void Add()
		{
			for (int32 i = 0; i < Limit; i++)
			{
				lock_guard<SpinLock> guard(sl);
				sum++;
			}
		}

		void Sub()
		{
			for (int32 i = 0; i < Limit; i++)
			{
				lock_guard<SpinLock> guard(sl);
				sum--;
			}
		}

		void foo()
		{
			thread t1(Add);
			thread t2(Sub);

			t1.join();
			t2.join();

			cout << sum << endl;
		}
	}

	namespace EventTest
	{
		// 커널 오브젝트를 다룰 때에 쓴다. 보통 아래의 요소를 갖는다.
		// Usage Count, 커널 오브젝트의 참조 횟수, shared_ptr의 reference count와 유사하다. 이 값이 0이되면 커널 오브젝트가 소멸된다.
		// Signal(사용 가능한 상태), Non-Signal(사용 불가능한 상태) 2가지 상태를 갖는다.
		HANDLE handle;
		mutex m;
		queue<int32> q;

		void Produce()
		{
			while (true)
			{
				{
					unique_lock<mutex> ul(m);
					q.push(100);
				}

				// Event가 Signal 상태가 되게 한다. WaitForSingleObject 함수로 대기중인 스레드가 깨어나서 해당 함수 아래 코드를 실행한다.
				::SetEvent(handle);

				// 이렇게 오래 기다려야 하는 경우 SpinLock을 쓰면 CPU만 낭비된다. 그래서 위와 같이 Event를 활용한 것이다.
				this_thread::sleep_for(10000ms);
			}
		}

		void Consume()
		{
			while (true)
			{
				// 이것으로 인해 thread가 깨어나지 않고 sleep 상태에 있게 되고 CPU 소모를 안하게 된다. 진단 검사를 켜보면 CPU 점유율이 사라지는 것을 볼 수 있다.
				// bManualReset을 FALSE로 해놓아서 자동으로 Non-Signal 상태로 돌아간다.
				::WaitForSingleObject(handle, INFINITE);
				//::ResetEvent(handle); // bManualReset을 TRUE로 했다면 이렇게 아래에서 이벤트를 초기화해주어야 한다.

				unique_lock<mutex> lock(m);
				if (q.empty() == false)
				{
					int32 data = q.front();
					q.pop();
					cout << q.size() << endl;
				}
			}
		}

		void foo()
		{
			// 아래와 같이 Event 할당해서 사용 가능
			handle = ::CreateEvent(NULL/*보안속성*/, TRUE/*bManualReset, 수동 리셋여부*/, FALSE/*bInitialState, 이벤트의 초기 상태*/, NULL/*lpName, 이벤트 이름*/);

			thread t1(Produce);
			thread t2(Consume);

			t1.join();
			t2.join();
			::CloseHandle(handle);
		}
	}


	// 이제 Event를 쓰기보다는 ConditionVariable을 사용하는 것이 권장된다. 성능상 이점이 있다.
	namespace ConditionVariableTest
	{
		// Condition Variable은 User-Level Object라고 한다. 사실 내부적으로 Keyed Events를 쓰지만 프로세스간에 공유가 안되기에 User-Level Object라고 한다.
		// Keyed Events는 Events를 미리 만들어놓고 Hash 형태로 존재하는 Event를 Key를 통해 가져다 쓰는 새로운 사용 방식이라고 한다. 
		// // TODO: 맞게 이해했는지 헷갈려서 나중에 쓸 일 있으면 재확인 필요
		// http://joeduffyblog.com/2006/11/28/windows-keyed-events-critical-sections-and-new-vista-synchronization-features/
		// https://stackoverflow.com/questions/1215402/difference-between-event-object-and-condition-variable
		condition_variable cv;
		queue<int32> q;
		HANDLE handle;
		mutex m;

		void Produce()
		{
			while (true)
			{
				{
					unique_lock<mutex> lock(m);
					q.push(100);
				}

				// notify_all()은 wait 중인 모든 스레드에 통지, notify_one()은 wait 중인 하나의 스레드만 깨운다.
				// 이게 위의 lock잡힌 부분에 들어가지 않는 이유는, cv.wait()에서 깨어나자마자 다시 lock을 잡으려 할 것이기 때문에,
				// lock이 잡힌 상태에서 cv.notify_one();을 하는 것은 리소스 낭비이다. 그래서 권장되지 않는다.
				cv.notify_one();
			}
		}

		void Consume()
		{
			while (true)
			{
				unique_lock<mutex> lock(m);

				// 아래 코드는 다음과 같이 작동한다.
				// 1) lock을 잡고
				// 2) 넣어준 Predicate 조건 확인
				// - 만족 O, cv.wait() 아래 코드를 실행 (lock을 풀지 않음)
				// - 만족 X, lock을 풀어주고, 대기(Wait) 상태
				cv.wait(lock, []() { return q.empty() == false; });

				// 아래 코드가 실행될 때에 lock이 잡힌 상태란 것을 명심하자.
				//while (q.empty() == false) // cv.wait에서 q.empty()를 체크하기에 이 부분이 필요가 없다.
				{
					int32 data = q.front();
					q.pop();
					cout << q.size() << endl;
				}
			}
		}
	}

	// 아래는 mutex, condition_variable까지 쓰지 않아도 되는 간단한 비동기 작업을 위한 도구들이다.
	// 파일 비동기 읽기와 같은 일회성 작업을 위해서 사용할 수 있다.
	namespace FutureTest
	{
		// 1) async: 원하는 함수를 비동기로 실행
		// 2) promise: 결과물을 promise를 통해 future에 전달
		// 3) packaged_task: 원하는 함수의 실행 결과를 이것을 통해 future에 전달, 이거보다 async가 편해서 async를 더 많이 쓸 것 같다.

		int64 Calculate()
		{
			int64 sum = 0;
			cout << "Calculate 시작" << endl;
			for (int32 i = 0; i < 10000; i++)
			{
				sum += i;
			}

			return sum;
		}

		class Temp
		{
		public:
			int64 TempFunc() { cout << "TempFunc()" << endl; return 1; }
		};


		void PromiseWorker(promise<string>&& p)
		{
			p.set_value("promise");
		}

		void PackagedTaskWorker(packaged_task<int64(void)>&& task)
		{
			task();
		}
		void foo()
		{

			// future 사용법
			{
				// async로 실행할 때에 3가지 옵션이 있다.
				// 1) deferred -> lazy evaluation, 지연해서 get()을 실행할 때에 연산, 멀티 스레딩은 아니다. 
				// 2) async -> 별도의 쓰레드에서 바로 시작
				// 3) deferred | async -> 둘 중 알아서 골라서, 여기서는 async로 실행된다.
				future<int64> f1 = async(launch::async | launch::deferred, Calculate);

				// future.get()을 실행하면 future는 empty가 된다. 한 번 get()을 호출했다면 재사용하면 안된다.
				f1.get();

				Temp t;
				// 멤버 함수를 future에서 사용하려면 아래와 같이 인수를 넣어주면 된다.
				future<int64> f2 = async(launch::async, &Temp::TempFunc, t);
				f2.get();
			}

			// promise 사용법
			{
				promise<string> p;
				future<string> f = p.get_future();

				thread t(PromiseWorker, move(p));

				cout << f.get() << endl;
				t.join();
			}

			// packaged_task 사용법
			{
				packaged_task<int64(void)> task(Calculate);
				future<int64> f = task.get_future();

				thread t(PackagedTaskWorker, move(task));
				cout << f.get() << endl;

				t.join();
			}

		}
	}

	namespace CacheTest
	{
		constexpr int BUFFER_SIZE = 10000;
		int buffer[BUFFER_SIZE][BUFFER_SIZE];
		void foo()
		{
			{
				uint64 start = GetTickCount64();
				int64 sum = 0;

				for (int32 i = 0; i < BUFFER_SIZE; i++)
				{
					for (int32 j = 0; j < BUFFER_SIZE; j++)
					{
						sum += buffer[i][j]; // 붙어 있는 데이터들을 순차적으로 순회
					}
				}

				uint64 end = GetTickCount64();

				cout << "Elapsed Tick " << end - start << endl;
			}

			{
				uint64 start = GetTickCount64();
				int64 sum = 0;

				for (int32 i = 0; i < BUFFER_SIZE; i++)
				{
					for (int32 j = 0; j < BUFFER_SIZE; j++)
					{
						sum += buffer[j][i]; // 위와 다르게 배열을 건너뛰면서 붙어있지 않은 주소들을 참조한다.
					}
				}

				uint64 end = GetTickCount64();

				cout << "Elapsed Tick " << end - start << endl; // 2~3배 정도 위 방식이 더 빠르다.
			}

		}
	}

	namespace CpuPipeLineTest
	{
		int32 x = 0;
		int32 y = 0;
		int32 r1 = 0;
		int32 r2 = 0;
		volatile bool ready = false;

		// 아래 같은 경우 y = 1 과 r1 = x의 순서를 바꿔도 해당 함수에서는 순서상 문제가 없으므로 코드 재배치가 발생한다.
		// 컴파일러는 단일 스레드를 기준으로 해당 함수를 검사하기에, r1 = x를 먼저하는게 더 빠르다고 판단되면 코드 재배치를 실행하는 것이다.

		// 그리고 여기에 더해서 CPU가 실행하는 코드 순서를 뒤집을 수 있다는 것도 알아두자.
		// CPU 명령어는 간단히 비유하면 빨래할 때에 세탁 -> 탈수 -> 건조의 과정을 거치듯이 단계별로 실행이 된다. 
		// 이 때에 CPU 명령어가 요청된 순서대로 처리하기보다 명령어 순서를 바꾸어서 
		// 앞단의 명령어가 빠르게 끝나게 하면 많은 명령어를 여러 단계에서 동시에 실행할 수 있기 때문에 이러한 현상이 발생한다.
		// https://resilient-923.tistory.com/170 TODO: 모르는 내용이 많다.. 나중에 해당 내용파야할 필요가 있다면 링크에서 언급한 내용 찾아서 공부하자.
		void ThreadFunc1()
		{
			while (!ready) {}
			y = 1;
			r1 = x;
		}

		void ThreadFunc2()
		{
			while (!ready) {}
			x = 1;
			r2 = y;
		}

		void foo()
		{
			int32 count = 0;

			while (true)
			{
				ready = false;
				count++;

				x = y = r1 = r2 = 0;

				thread t1(ThreadFunc1);
				thread t2(ThreadFunc2);

				ready = true; // 이를 통해 t1, t2 동시 시작하게 해보는 것

				t1.join();
				t2.join();

				// 직관적으로 생각했을 때에 r1과 r2가 t1과 t2가 join() 된 후에 0이 나오면 안된다.
				// 하지만 멀티스레딩 환경에서 x = 1과 y = 1을 할당하기 전에 r1 = x와 r2 = y 코드가 실행될 수 있다.
				// '코드 재배치'가 일어나서 r1 = x 나 r2 = y가 윗줄의 코드보다 먼저 실행되기 때문이다.
				if (r1 == 0 && r2 == 0)
				{
					break;
				}
			}

			cout << count << endl;
		}
	}

	namespace MemoryModelTest
	{
		// Memory Model (정책)
		// 1) Sequentially Consistent: 가장 엄격, 컴파일러 최적화 여지가 없음(직관적)
		// memory_order_seq_cst 옵션을 통해 사용
		// 가시성 보장 (Cache 간에 값 불일치 문제 해결)
		// 코드 재배치 방지
		// intel이나 AMD CPU라면 기본적으로 순차적 일관성을 보장해서 웬만하면 이것을 쓰자! ARM의 경우는 이것을 쓰면 성능차이가 좀 난다고 한다.

		// 2) Acquire-Release (acquire, release, acq_rel):
		// memory_order_release와 memory_order_acquire를 통해 사용
		// 중간 정도의 자유로움
		// produce 쪽에서 memory_order_release, consume 쪽에서 memory_order_acquire 로 사용
		// release 명령 이전의 메모리 명령들이, release 이후로 재배치되는 것을 금지 (특정 작업을 다 한 후에 값을 줄거니까, 앞의 특정 작업이 끝날때까지 기다려준다고 보기)
		// acquire 명령 이후의 메모리 명령들이, acquire 앞으로 재배치되는 것을 금지 (값을 받고나서 특정 작업을 할 것이니, 값 받고난 다음 특정 작업이 끝나도록 기다려준다고 보기)
		// 가시성 보장

		// 3) Relaxed (relaxed): 자유로움, 컴파일러가 마음대로 최적화 한다. 코드 재배치 등이 일어나기에 결과 예측이 잘 안된다.
		// memory_order_relaxed 옵션을 통해 사용
		// 너무나도 자유롭다.
		// 가시성 보장 X
		// 코드 재배치 방지 X
		// 동일 객체에 대한 관전 순서만 보장

		atomic<bool> ready;
		int32 value;
		void Produce()
		{
			value = 10;
			ready.store(true, memory_order::memory_order_seq_cst); // 코드 재배치 발생하지 않는다.
			//ready.store(true, memory_order::memory_order_relaxed); // 코드 재배치 일어난다. 웬만해선 쓸 일이 없다.
			//ready.store(true, memory_order::memory_order_release); // 이거 아래로 코드가 못 넘어온다.

			// 위에처럼 atomic을 사용하지 않고도 아래와 같은 코드를 통해 코드 재배치를 막을 수 있다는 것도 알아두자.
			// 그래도 atomic을 쓰면 알아서 이런 것들을 해주니까 atomic을 쓰는 경우가 많다.
			//atomic_thread_fence(memory_order::memory_order_release);
		}

		void Consume()
		{
			while (ready.load(memory_order::memory_order_seq_cst) == false);
			//while (ready.load(memory_order::memory_order_relaxed) == false);
			//while (ready.load(memory_order::memory_order_acquire) == false); // 이거 위로 코드가 못 넘어간다.


			cout << value << endl;
		}

		atomic<bool> flag;
		void foo1()
		{

			// flag = true
			flag.store(true, memory_order::memory_order_seq_cst); // 위의 연산과 이 연산이 같다.

			bool val = flag.load(memory_order::memory_order_seq_cst);

			// 이전 flag 값을 prev에 넣고, flag값을 해당 값으로 수정, 넣으면서 이전 값 가져오려면 exchange 함수를 써야한다.
			{
				bool prev = flag.exchange(true);
			}

			// CAS
			{
				bool expected = false;
				bool desired = true;
				flag.compare_exchange_strong(expected, desired); // flag 값이 expected 값이 나오면 반드시 desired를 넣어줌, 위에 자세한 설명있으니 참고
				// flag.compare_exchange_weak(expected, desired); // 이런 함수도 있는데, 원래는 expected가 flag와 같아도 하드웨어 문제로 desired를 넣는 것에 실패할 수 있다. 
				// 이 때에 strong은 반복해서 desired를 넣어주고, weak는 그냥 안넣어주는 점이 다르다.
			}
		}

		void foo2()
		{
			ready = false;
			value = 0;
			thread t1(Produce);
			thread t2(Consume);

			t1.join();
			t2.join();
		}
	}

	namespace ThreadLocalStorageTest
	{
		// 보통 thread_local 로 쓰는 변수는 앞에 L(Local)을 붙인다고 한다. 
		// 일반 전역변수인 경우 앞에 G를 붙이는 컨벤션이 있다는 것도 참고.
		thread_local int32 LThreadId = 0;

		void ThreadMain(int32 threadId)
		{
			LThreadId = threadId;

			while (true)
			{
				cout << LThreadId << endl;
				this_thread::sleep_for(1s);
			}
		}

		// 아래 코드가 실행되면 LThreadId를 출력하는데 각 스레드별로 다른 값이 나온다. 스레드별로 고유한 변수 LThreadId 생성되었기 때문이다.
		// 위에서 LThreadId의 thread_local 부분을 제외하면 모두 같은 값이 나오게 된다.
		void foo()
		{
			vector<thread> threads;

			for (int i = 0; i < 10; i++)
			{
				int32 threadId = i + 1;
				threads.push_back(thread(ThreadMain, threadId));
			}

			for (int i = 0; i < 10; i++)
			{
				threads[i].join();
			}
		}
	}

}