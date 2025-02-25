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

			// �޸� �ʱ�ȭ �߿� ���ܰ� �߻��Ѵٸ� �޸� �ʱ�ȭ ����
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

			// �޸� �ʱ�ȭ �߿� ���ܰ� �߻��Ѵٸ� �޸� �ʱ�ȭ ����
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
			refCnt.fetch_add(1, std::memory_order_relaxed); // �޸� ���� �ٲ� ���x
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
			// �̰� �� �Ʒ��δ� ��� ���� ���ٲٰ� �ϴ� ��.
			// acquire �� �ڱ� ���� �ִ°� �Ʒ��� ������ �ϰ�, release �� �ڱ� �Ʒ� �ִ°� ���� ������ �Ѵ�.
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
	//	using iterator_category = random_access_iterator_tag; // C++ 11 ����
	//	// typedef random_access_iterator_tag iterator_category; // C++ 11 ����
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

	// �ݺ����� ������ ���� �ٸ��� �����ϴ� advance_imp() ����
	template<typename T>
	void advance_imp(T& it, typename iterator_traits<T>::difference_type sz, random_access_iterator_tag)
	{
		it = it + sz;
		cout << "advance using +" << endl;
	}

	// ���⼭ input_iterator_Tag �� ���� Ŭ������ random_access_iterator_tag �� �ƴϸ� �� ����� ���´�.
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
	// iterator_traits ���� difference_type �̶� ���� �־ + ���꿡 ���Ǵ� Ÿ�Ե� ����Ǿ� �ִٴ� ���� ����.
	void xadvance(T& it, typename iterator_traits<T>::difference_type sz)
	{
		// �Ʒ� �κп��� iterator_traits<T> �� ���ؼ� iterator_category �� ������ ���� �߿��ϴ�.�迭���� ���� T* ���¸� ������ ���� �߿�!
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
		// �̷��� �Ǹ� if constexpr�� �����̸� �ڵ� ��ü�� ������ �ȵǾ ������ �Ȼ����. C++ 17���� �̰� ���Ե�.
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
		requires std::random_access_iterator<T> // �Ʒ� �ڵ� ����ؼ� �Ȱ��� Concept �� �����.
	//requires std::is_same_v<std::random_access_iterator_tag, typename std::iterator_traits<T>::iterator_category>
	void xadvance(T& it, typename std::iterator_traits<T>::difference_type sz)
	{
		it = it + sz;
		cout << "advance +" << endl;
	}

	template<typename T>
		requires (!std::random_access_iterator<T>)
	// requires ������ !�� ���� ���� ��ȣ �ȿ� �ٿ����ϴ� ���� ���� ����.
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
		//using is_transparent = int; // int �̵� void �̵��� �������. find �ڵ庸�� Compare �ϴ� ��ü Ÿ�Կ� is_transparent �� �ֵ��� �����ߴ�.
	};
}

// Coercion : ����, ���⼭�� '���� ��ȯ'�̶�� �ǹ�
// �Ʒ� �ڵ带 ���� smartptr �� ���ø� �����ڸ� ���ؼ� ���� ��ȯ�� �ϰ� �ִ�.
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

		// �Ҹ���, ���� ��� ���� ���� ���� ..

		// smartptr(const smartptr<T>& sp) {} // �⺻���� ���������� T Ÿ���� Animal �̶� ����ġ�� �߻��Ѵ�.
		// smartptr(const smartptr<Dog>& sp) {} // Dog Ÿ�Կ� ���ؼ� ���� �� ������, �� ���� Ÿ�Կ� ���ؼ� �Ұ�
		template<typename U,
			typename = std::enable_if_t<std::is_convertible_v<U*, T*>> > // enable_if �� ���ָ� ��ȯ �Ұ��ÿ� �ƿ� �ڵ尡 ������ �ȵǸ鼭 ������ �� ����ϰ� ���´�.
		smartptr(const smartptr<U>& sp) : ptr(sp.ptr) {} // �̷��� ���ָ� ������ Ÿ�Կ� ���ؼ� ���� �� �ְ�, ��Ӱ����� ���� ������ ��ȯ�� �����ϴ�.

		template<typename> friend class smartptr; // typename �� ����� ����. ���ø����� ��������� Ŭ�������� �� Ŭ������ private ����� �����ϵ��� �ϴ� �뵵
	};

	//Dog* p1 = new Dog;
	//Animal* p2 = p1;

	//smartptr<Dog> sp1(new Dog);
	//smartptr<Animal> sp2 = sp1;

	//smartptr<int> sp3(new int);
	//smartptr<double> sp4 = sp3; // int* �� double* �� �Ͻ������� ��ȯ���� ���Ѵ�.
}

namespace Ranges
{
	template<typename T>
	class view_interface : public std::ranges::view_base
	{

		// �ڽ��� ��ӹ��� �Ļ�Ŭ���� Ÿ������ ��ȯ��Ų��. begin()�� end() ���� ���� �������� �̷��� CRTP �� ����Ѵ�.
		T& Cast() { return static_cast<T&>(*this); }

	public:
		int size() { return Cast().end() - Cast().begin(); } // const �� �� ������ ������, �ϴ� �����ϰ�..
	};

	template<typename T> class take_view : public view_interface<take_view<T>>
	{
		T rng; // �� �κ��� �߿�, all �Լ��� ���ؼ� view �� ���δ� view �� ���纻����, view ���� Ÿ���� ���δ� ref_view ������ ��������.
		size_t count;

	public:
		take_view() = default; // �̰͵� view ���� üũ�ϴ� ������ �ִ�.
		take_view(T r, size_t c) : rng(r), count(c) {}

		auto begin() { return rng.begin(); }
		auto end() { return rng.begin() + count; }
	};

	// template deduction guide, ���ø� ���ڸ� �Ʒ��� ���� ��Ģ���� �߷��϶�� �˷��ִ� ���̴�.
	// �Ʒ� ���� ��� take_view(T& r, size_t c) �����ڰ� ȣ��� ���� T�� rvalue(��) �� �´ٸ� 'all_t �� ���ؼ� ���ߵ� T' Ÿ������ take_view �� ������ش�.
	template<typename T>
	//take_view(T&& t, size_t)->take_view<remove_cvref_t<decltype(views::all(t))>>; // �̷��� �ؾ��ϴµ�, �װ��� ����� ���� �Ʒ��� all_t<T> �̴�.
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

	// �̹� ǥ�ؿ� �Ʒ��� all �� �ִ�.
	template<typename T> auto all(T&& arg)
	{
		if constexpr (ranges::view<remove_cvref_t<T>>)
		{ // ���� concept �� ������ default ������ üũ, view_base ��� ���� üũ�� ����ִ�.
			cout << "view" << endl;

			// view �� ������ ���纻�� ��ȯ, �̸� ���ؼ� take_view(reverse_view(v),3); ���� �ӽð��� �޴� ���°� ����������.
			return remove_cvref_t<T>(arg);
		}
		else
		{
			cout << "not view" << endl;

			// view �� �ƴ� vector ���� ���� ������ ref_view �� ���μ� ��ȯ
			return ranges::ref_view<remove_cvref_t<T>>(arg);
		}
	}

	//vector v = { 1,2,3,4 };
	//take_view t(v, 2);

	//for (auto e : reverse_view(take_view(v,3))) { // ���� �ӽ� �����ε� ����� �����ϴ�.
	//	cout << e << endl;
	//}
}

namespace MultiThreadProgramming
{

	// Critical Section(User-Level Object)�� ã�ƺ��� SpinLock + Event�� Ȱ���� ���̶�� �Ѵ�.
	// https://stackoverflow.com/questions/5158191/choosing-between-critical-sections-mutex-and-spin-locks

	namespace SpinLockTest
	{
		// �Ʒ� ���� ���� SpinLock�� Sleep�� �Բ� ���� �ڵ��̴�.
		class SpinLock
		{
		public:
			void lock()
			{

				bool expected = false;
				bool desired = true;

				while (_locked.compare_exchange_strong(expected, desired) == false)
				{
					// expected�� reference �̱⿡ ���� _locked ������ ��� �ٲ��. �׷��� expected�� �츮�� ���ϴ� ������ �ٲ���� �Ѵ�.
					expected = false;

					//this_thread::sleep_for(std::chrono::milliseconds(0)); // 0 �и���
					this_thread::sleep_for(0ms); // ���� ����, ms�� operator�� ���ǵǾ� ����.
					//this_thread::yield();  // sleep_for(0)�� ��ǻ� ����.
				}

				// �Ʒ��� ���� CAS �ǻ��ڵ�
				// ������ڸ� _locked�� ����ִ� ���� expected�� �ְ�, _locked�� expected�� ������ desired ������ _locked�� �ٲ۴�.
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
				_locked.store(false); // atomic�̶� ����ó�� �ص� ������� �ϴ�.
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
		// Ŀ�� ������Ʈ�� �ٷ� ���� ����. ���� �Ʒ��� ��Ҹ� ���´�.
		// Usage Count, Ŀ�� ������Ʈ�� ���� Ƚ��, shared_ptr�� reference count�� �����ϴ�. �� ���� 0�̵Ǹ� Ŀ�� ������Ʈ�� �Ҹ�ȴ�.
		// Signal(��� ������ ����), Non-Signal(��� �Ұ����� ����) 2���� ���¸� ���´�.
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

				// Event�� Signal ���°� �ǰ� �Ѵ�. WaitForSingleObject �Լ��� ������� �����尡 ����� �ش� �Լ� �Ʒ� �ڵ带 �����Ѵ�.
				::SetEvent(handle);

				// �̷��� ���� ��ٷ��� �ϴ� ��� SpinLock�� ���� CPU�� ����ȴ�. �׷��� ���� ���� Event�� Ȱ���� ���̴�.
				this_thread::sleep_for(10000ms);
			}
		}

		void Consume()
		{
			while (true)
			{
				// �̰����� ���� thread�� ����� �ʰ� sleep ���¿� �ְ� �ǰ� CPU �Ҹ� ���ϰ� �ȴ�. ���� �˻縦 �Ѻ��� CPU �������� ������� ���� �� �� �ִ�.
				// bManualReset�� FALSE�� �س��Ƽ� �ڵ����� Non-Signal ���·� ���ư���.
				::WaitForSingleObject(handle, INFINITE);
				//::ResetEvent(handle); // bManualReset�� TRUE�� �ߴٸ� �̷��� �Ʒ����� �̺�Ʈ�� �ʱ�ȭ���־�� �Ѵ�.

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
			// �Ʒ��� ���� Event �Ҵ��ؼ� ��� ����
			handle = ::CreateEvent(NULL/*���ȼӼ�*/, TRUE/*bManualReset, ���� ���¿���*/, FALSE/*bInitialState, �̺�Ʈ�� �ʱ� ����*/, NULL/*lpName, �̺�Ʈ �̸�*/);

			thread t1(Produce);
			thread t2(Consume);

			t1.join();
			t2.join();
			::CloseHandle(handle);
		}
	}


	// ���� Event�� ���⺸�ٴ� ConditionVariable�� ����ϴ� ���� ����ȴ�. ���ɻ� ������ �ִ�.
	namespace ConditionVariableTest
	{
		// Condition Variable�� User-Level Object��� �Ѵ�. ��� ���������� Keyed Events�� ������ ���μ������� ������ �ȵǱ⿡ User-Level Object��� �Ѵ�.
		// Keyed Events�� Events�� �̸� �������� Hash ���·� �����ϴ� Event�� Key�� ���� ������ ���� ���ο� ��� ����̶�� �Ѵ�. 
		// // TODO: �°� �����ߴ��� �򰥷��� ���߿� �� �� ������ ��Ȯ�� �ʿ�
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

				// notify_all()�� wait ���� ��� �����忡 ����, notify_one()�� wait ���� �ϳ��� �����常 �����.
				// �̰� ���� lock���� �κп� ���� �ʴ� ������, cv.wait()���� ����ڸ��� �ٽ� lock�� ������ �� ���̱� ������,
				// lock�� ���� ���¿��� cv.notify_one();�� �ϴ� ���� ���ҽ� �����̴�. �׷��� ������� �ʴ´�.
				cv.notify_one();
			}
		}

		void Consume()
		{
			while (true)
			{
				unique_lock<mutex> lock(m);

				// �Ʒ� �ڵ�� ������ ���� �۵��Ѵ�.
				// 1) lock�� ���
				// 2) �־��� Predicate ���� Ȯ��
				// - ���� O, cv.wait() �Ʒ� �ڵ带 ���� (lock�� Ǯ�� ����)
				// - ���� X, lock�� Ǯ���ְ�, ���(Wait) ����
				cv.wait(lock, []() { return q.empty() == false; });

				// �Ʒ� �ڵ尡 ����� ���� lock�� ���� ���¶� ���� �������.
				//while (q.empty() == false) // cv.wait���� q.empty()�� üũ�ϱ⿡ �� �κ��� �ʿ䰡 ����.
				{
					int32 data = q.front();
					q.pop();
					cout << q.size() << endl;
				}
			}
		}
	}

	// �Ʒ��� mutex, condition_variable���� ���� �ʾƵ� �Ǵ� ������ �񵿱� �۾��� ���� �������̴�.
	// ���� �񵿱� �б�� ���� ��ȸ�� �۾��� ���ؼ� ����� �� �ִ�.
	namespace FutureTest
	{
		// 1) async: ���ϴ� �Լ��� �񵿱�� ����
		// 2) promise: ������� promise�� ���� future�� ����
		// 3) packaged_task: ���ϴ� �Լ��� ���� ����� �̰��� ���� future�� ����, �̰ź��� async�� ���ؼ� async�� �� ���� �� �� ����.

		int64 Calculate()
		{
			int64 sum = 0;
			cout << "Calculate ����" << endl;
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

			// future ����
			{
				// async�� ������ ���� 3���� �ɼ��� �ִ�.
				// 1) deferred -> lazy evaluation, �����ؼ� get()�� ������ ���� ����, ��Ƽ �������� �ƴϴ�. 
				// 2) async -> ������ �����忡�� �ٷ� ����
				// 3) deferred | async -> �� �� �˾Ƽ� ���, ���⼭�� async�� ����ȴ�.
				future<int64> f1 = async(launch::async | launch::deferred, Calculate);

				// future.get()�� �����ϸ� future�� empty�� �ȴ�. �� �� get()�� ȣ���ߴٸ� �����ϸ� �ȵȴ�.
				f1.get();

				Temp t;
				// ��� �Լ��� future���� ����Ϸ��� �Ʒ��� ���� �μ��� �־��ָ� �ȴ�.
				future<int64> f2 = async(launch::async, &Temp::TempFunc, t);
				f2.get();
			}

			// promise ����
			{
				promise<string> p;
				future<string> f = p.get_future();

				thread t(PromiseWorker, move(p));

				cout << f.get() << endl;
				t.join();
			}

			// packaged_task ����
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
						sum += buffer[i][j]; // �پ� �ִ� �����͵��� ���������� ��ȸ
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
						sum += buffer[j][i]; // ���� �ٸ��� �迭�� �ǳʶٸ鼭 �پ����� ���� �ּҵ��� �����Ѵ�.
					}
				}

				uint64 end = GetTickCount64();

				cout << "Elapsed Tick " << end - start << endl; // 2~3�� ���� �� ����� �� ������.
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

		// �Ʒ� ���� ��� y = 1 �� r1 = x�� ������ �ٲ㵵 �ش� �Լ������� ������ ������ �����Ƿ� �ڵ� ���ġ�� �߻��Ѵ�.
		// �����Ϸ��� ���� �����带 �������� �ش� �Լ��� �˻��ϱ⿡, r1 = x�� �����ϴ°� �� �����ٰ� �ǴܵǸ� �ڵ� ���ġ�� �����ϴ� ���̴�.

		// �׸��� ���⿡ ���ؼ� CPU�� �����ϴ� �ڵ� ������ ������ �� �ִٴ� �͵� �˾Ƶ���.
		// CPU ��ɾ�� ������ �����ϸ� ������ ���� ��Ź -> Ż�� -> ������ ������ ��ġ���� �ܰ躰�� ������ �ȴ�. 
		// �� ���� CPU ��ɾ ��û�� ������� ó���ϱ⺸�� ��ɾ� ������ �ٲپ 
		// �մ��� ��ɾ ������ ������ �ϸ� ���� ��ɾ ���� �ܰ迡�� ���ÿ� ������ �� �ֱ� ������ �̷��� ������ �߻��Ѵ�.
		// https://resilient-923.tistory.com/170 TODO: �𸣴� ������ ����.. ���߿� �ش� �����ľ��� �ʿ䰡 �ִٸ� ��ũ���� ����� ���� ã�Ƽ� ��������.
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

				ready = true; // �̸� ���� t1, t2 ���� �����ϰ� �غ��� ��

				t1.join();
				t2.join();

				// ���������� �������� ���� r1�� r2�� t1�� t2�� join() �� �Ŀ� 0�� ������ �ȵȴ�.
				// ������ ��Ƽ������ ȯ�濡�� x = 1�� y = 1�� �Ҵ��ϱ� ���� r1 = x�� r2 = y �ڵ尡 ����� �� �ִ�.
				// '�ڵ� ���ġ'�� �Ͼ�� r1 = x �� r2 = y�� ������ �ڵ庸�� ���� ����Ǳ� �����̴�.
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
		// Memory Model (��å)
		// 1) Sequentially Consistent: ���� ����, �����Ϸ� ����ȭ ������ ����(������)
		// memory_order_seq_cst �ɼ��� ���� ���
		// ���ü� ���� (Cache ���� �� ����ġ ���� �ذ�)
		// �ڵ� ���ġ ����
		// intel�̳� AMD CPU��� �⺻������ ������ �ϰ����� �����ؼ� �����ϸ� �̰��� ����! ARM�� ���� �̰��� ���� �������̰� �� ���ٰ� �Ѵ�.

		// 2) Acquire-Release (acquire, release, acq_rel):
		// memory_order_release�� memory_order_acquire�� ���� ���
		// �߰� ������ �����ο�
		// produce �ʿ��� memory_order_release, consume �ʿ��� memory_order_acquire �� ���
		// release ��� ������ �޸� ��ɵ���, release ���ķ� ���ġ�Ǵ� ���� ���� (Ư�� �۾��� �� �� �Ŀ� ���� �ٰŴϱ�, ���� Ư�� �۾��� ���������� ��ٷ��شٰ� ����)
		// acquire ��� ������ �޸� ��ɵ���, acquire ������ ���ġ�Ǵ� ���� ���� (���� �ް��� Ư�� �۾��� �� ���̴�, �� �ް� ���� Ư�� �۾��� �������� ��ٷ��شٰ� ����)
		// ���ü� ����

		// 3) Relaxed (relaxed): �����ο�, �����Ϸ��� ������� ����ȭ �Ѵ�. �ڵ� ���ġ ���� �Ͼ�⿡ ��� ������ �� �ȵȴ�.
		// memory_order_relaxed �ɼ��� ���� ���
		// �ʹ����� �����Ӵ�.
		// ���ü� ���� X
		// �ڵ� ���ġ ���� X
		// ���� ��ü�� ���� ���� ������ ����

		atomic<bool> ready;
		int32 value;
		void Produce()
		{
			value = 10;
			ready.store(true, memory_order::memory_order_seq_cst); // �ڵ� ���ġ �߻����� �ʴ´�.
			//ready.store(true, memory_order::memory_order_relaxed); // �ڵ� ���ġ �Ͼ��. �����ؼ� �� ���� ����.
			//ready.store(true, memory_order::memory_order_release); // �̰� �Ʒ��� �ڵ尡 �� �Ѿ�´�.

			// ����ó�� atomic�� ������� �ʰ� �Ʒ��� ���� �ڵ带 ���� �ڵ� ���ġ�� ���� �� �ִٴ� �͵� �˾Ƶ���.
			// �׷��� atomic�� ���� �˾Ƽ� �̷� �͵��� ���ִϱ� atomic�� ���� ��찡 ����.
			//atomic_thread_fence(memory_order::memory_order_release);
		}

		void Consume()
		{
			while (ready.load(memory_order::memory_order_seq_cst) == false);
			//while (ready.load(memory_order::memory_order_relaxed) == false);
			//while (ready.load(memory_order::memory_order_acquire) == false); // �̰� ���� �ڵ尡 �� �Ѿ��.


			cout << value << endl;
		}

		atomic<bool> flag;
		void foo1()
		{

			// flag = true
			flag.store(true, memory_order::memory_order_seq_cst); // ���� ����� �� ������ ����.

			bool val = flag.load(memory_order::memory_order_seq_cst);

			// ���� flag ���� prev�� �ְ�, flag���� �ش� ������ ����, �����鼭 ���� �� ���������� exchange �Լ��� ����Ѵ�.
			{
				bool prev = flag.exchange(true);
			}

			// CAS
			{
				bool expected = false;
				bool desired = true;
				flag.compare_exchange_strong(expected, desired); // flag ���� expected ���� ������ �ݵ�� desired�� �־���, ���� �ڼ��� ���������� ����
				// flag.compare_exchange_weak(expected, desired); // �̷� �Լ��� �ִµ�, ������ expected�� flag�� ���Ƶ� �ϵ���� ������ desired�� �ִ� �Ϳ� ������ �� �ִ�. 
				// �� ���� strong�� �ݺ��ؼ� desired�� �־��ְ�, weak�� �׳� �ȳ־��ִ� ���� �ٸ���.
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
		// ���� thread_local �� ���� ������ �տ� L(Local)�� ���δٰ� �Ѵ�. 
		// �Ϲ� ���������� ��� �տ� G�� ���̴� �������� �ִٴ� �͵� ����.
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

		// �Ʒ� �ڵ尡 ����Ǹ� LThreadId�� ����ϴµ� �� �����庰�� �ٸ� ���� ���´�. �����庰�� ������ ���� LThreadId �����Ǿ��� �����̴�.
		// ������ LThreadId�� thread_local �κ��� �����ϸ� ��� ���� ���� ������ �ȴ�.
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