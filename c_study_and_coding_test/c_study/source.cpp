#include <iostream>
#include <vector>
#include <list>
#include <set>
#include <ranges>

//#include <boost/type_index.hpp>
using namespace std;

namespace MakeUniquePtr {
	class Base {
	public:
		Base() {
			//cout << "Base" << endl;
		}

		~Base() {
			//cout << "~Base" << endl;
		}
	};
	class Car : public Base {
	public:
		~Car() { cout << "~Car" << endl; }

		void Go() { cout << "Car go" << endl; }

		Car() {
			cout << "Car" << endl;
		}

		Car(const Car& c) {
			cout << "Const Car&" << endl;
		}

		Car(Car&& c) noexcept {
			cout << "Car&&" << endl;
		}
	};

	struct Freer {
		void operator()(void* p) const {
			cout << "free" << endl;
			free(p);
		}
	};

	template <typename T>
	struct Deleter {
		Deleter() = default;

		template<typename U>
		Deleter(const Deleter<U>& d) {

		}
		void operator()(T* p) const {
			cout << "delete" << endl;
			delete p;
		}
	};

	template <typename T>
	struct Deleter<T[]> {
		void operator()(T* p) const {
			cout << "delete[]" << endl;
			delete[] p;
		}
	};

	template <typename T, typename D = Deleter<T>> class myUnique {

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
		myUnique(myUnique<T2, D2>&& in) {
			pobj = move(in.pobj);
			del = move(in.del);
			in.pobj = nullptr;
		}

		/*template<typename T2, typename D2>
		myUnique& operator=(myUnique<T2, D2>&& up) {

		}*/

		~myUnique() {
			if (pobj != nullptr) {
				del(pobj);
			}
		}

		T& operator*() const { return *pobj; }
		T* operator->() const { return pobj; }

		explicit operator bool() const { return static_cast<bool>(pobj); }
	};

	template <typename T, typename D> class myUnique<T[], D> {

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
		myUnique(myUnique<T2, D2>&& in) {
			pobj = move(in.pobj);
			del = move(in.del);
			in.pobj = nullptr;
		}

		/*template<typename T2, typename D2>
		myUnique& operator=(myUnique<T2, D2>&& up) {

		}*/

		~myUnique() {
			if (pobj != nullptr) {
				del(pobj);
			}
		}

		T& operator[](int idx) const { return pobj[idx]; }
		T* operator->() const { return pobj; }

		explicit operator bool() const { return static_cast<bool>(pobj); }
	};
}

namespace myVector {
	class Point {

	public:
		void foo() {
			cout << "Point::foo" << endl;
		}
		Point(Point&& p) noexcept {
			cout << "Point(Point&&)" << endl;
		}
		Point(const Point&& p) noexcept {
			cout << "Point(const Point&&)" << endl;
		}

		Point(const Point& p) {
			cout << "Point(const Point&)" << endl;
		}

		Point(Point& p) {
			cout << "Point(Point&)" << endl;
		}

		Point() {
			cout << "Point()" << endl;
		}

		Point(int x, int y) : x(x), y(y) {
			cout << "Point(x,y)" << endl;
		}

		~Point() {
			cout << "~Point()" << endl;
		}

	private:
		int x = 0;
		int y = 0;
	};

	template<typename T> class myVec {
		T* buff;
		size_t size;
		size_t capacity;

	public:
		myVec(size_t sz) :size(sz), capacity(sz) {
			//buff = new T[sz];
			buff = static_cast<Point*>(operator new(sizeof(Point) * sz));

			int i = 0;

			// 메모리 초기화 중에 예외가 발생한다면 메모리 초기화 실행
			try {
				for (i = 0; i < sz; i++) {
					new(&buff[i]) T;
					//new(&buff[i]) T(value);
				}
			}
			catch (...) {
				for (int j = i - 1; j >= 0; --j) {
					buff[j].~T();
				}

				operator delete(buff);
				size = 0;
				capacity = 0;
			}
		}

		myVec(size_t sz, const T& value) :size(sz), capacity(sz) {
			//buff = new T[sz];
			buff = static_cast<Point*>(operator new(sizeof(Point) * sz));

			int i = 0;

			// 메모리 초기화 중에 예외가 발생한다면 메모리 초기화 실행
			try {
				for (i = 0; i < sz; i++) {
					new(&buff[i]) T(value);
				}
			}
			catch (...) {
				for (int j = i - 1; j >= 0; --j) {
					buff[j].~T();
				}

				operator delete(buff);
				size = 0;
				capacity = 0;
			}
		}

		~myVec() {
			//delete[] buff;
			for (int j = size - 1; j >= 0; --j) {
				buff[j].~T();
			}

			operator delete(buff);
			/*size = 0;
			capacity = 0;*/
		}
	};

	template <typename T> struct MyAlloc {
		T* allocate(size_t n) {
			T* p = static_cast<T*>(malloc(n * sizeof(T)));
			cout << "allocate, " << "size:" << n << endl;
			return p;
		}

		void deallocate(T* p, size_t n) noexcept {
			free(p);
			cout << "deallocate" << endl;
		}

		template<typename ... ARGS> void construct(T* p, ARGS&& ... args) {
			new(p) T(forward<ARGS>(args)...);
			cout << "MyAlloc construct" << endl;
		}

		void destroy(T* p) {
			p->~T();
			cout << "MyAlloc destroy" << endl;
		}

		using value_type = T;

		template<typename U> MyAlloc(const MyAlloc<U>&) noexcept {}
		MyAlloc() = default;

		template<typename U> struct rebind {
			typedef MyAlloc<U> other;
		};
	};

}

namespace ReferenceCounting {
	class RefCountBase {
	protected:
		mutable atomic<int> refCnt{ 0 };

	public:
		void addRef() const {
			refCnt.fetch_add(1, std::memory_order_relaxed); // 메모리 순서 바뀌어도 상관x
		}
	};

	template<typename T>
	class RefCount : public RefCountBase {
	public:
		RefCount() {
			cout << "RefCount()" << endl;
		}

		RefCount(const RefCount& c) {
			cout << "RefCount(const RefCount&)" << endl;
		}

		RefCount(RefCount&& c) noexcept {
			cout << "RefCount(RefCount&& )" << endl;
		}

		void release() const {
			// 이거 위 아래로는 계산 순서 못바꾸게 하는 것.
			// acquire 이 자기 위에 있는거 아래로 못오게 하고, release 가 자기 아래 있는거 위로 못가게 한다.
			int ret = refCnt.fetch_sub(1, std::memory_order_acq_rel);

			if (ret == 1) {
				//delete static_cast<T*>(const_cast<RefCount*>(this));
				delete static_cast<const T*>(this);
			}
		}
	};

	template<typename T> class AutoPtr {
		T* obj;
	public:
		explicit AutoPtr(T* p = nullptr) :obj(p) {
			if (obj) {
				obj->addRef();
			}
		}

		AutoPtr(const AutoPtr<T>& ap) : obj(ap.obj) {
			if (obj) {
				obj->addRef();
			}
		}

		~AutoPtr() {
			if (obj) {
				obj->release();
			}
		}

	};

	class Truck : public RefCount<Truck> {
	public:
		~Truck() { cout << "~Truck()" << endl; }
	};
}

namespace AdvanceTagDispatching {
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
	void advance_imp(T& it, typename iterator_traits<T>::difference_type sz, random_access_iterator_tag) {
		it = it + sz;
		cout << "advance using +" << endl;
	}

	// 여기서 input_iterator_Tag 가 조상 클래스라서 random_access_iterator_tag 가 아니면 다 여길로 들어온다.
	template<typename T>
	void advance_imp(T& it, typename iterator_traits<T>::difference_type sz, input_iterator_tag) {

		while (sz--) {
			++it;
		}
		cout << "advance using ++" << endl;
	}

	template<typename T>
	// iterator_traits 에는 difference_type 이란 것이 있어서 + 연산에 사용되는 타입도 저장되어 있다는 것을 알자.
	void xadvance(T& it, typename iterator_traits<T>::difference_type sz) {
		// 아래 부분에서 iterator_traits<T> 를 통해서 iterator_category 를 꺼내는 것이 중요하다.배열같은 경우는 T* 형태를 꺼내는 것이 중요!
		advance_imp(it, sz, typename iterator_traits<T>::iterator_category());
	}
}

namespace AdvanceEnableIf {
	template<typename T>
	enable_if_t<std::is_same_v<typename iterator_traits<T>::iterator_category, std::random_access_iterator_tag>, void >
		xadvance(T& it, typename std::iterator_traits<T>::difference_type sz) {
		it = it + sz;
		cout << "xadvance +" << endl;
	}

	template<typename T>
	enable_if_t<!std::is_same_v<typename iterator_traits<T>::iterator_category, std::random_access_iterator_tag>, void >
		xadvance(T& it, typename std::iterator_traits<T>::difference_type sz) {
		while (sz--) {
			++it;
		}
		cout << "xadvance ++" << endl;
	}
}

namespace AdvanceIfConstexpr {
	template<typename T>
	void xadvance(T& it, typename std::iterator_traits<T>::difference_type sz) {
		// 이렇게 되면 if constexpr이 거짓이면 코드 자체가 생성이 안되어서 문제가 안생긴다. C++ 17부터 이게 도입됨.
		if constexpr (std::is_same_v<iterator_traits<T>::iterator_category, std::random_access_iterator_tag>) {
			it = it + sz;
			cout << "xadvance +" << endl;
		}
		else {
			while (sz--) {
				++it;
			}
			cout << "xadvance ++" << endl;
		}
	}
}

namespace AdvanceConcept {
	template<typename T>
	requires std::random_access_iterator<T> // 아래 코드 대신해서 똑같은 Concept 가 생겼다.
	//requires std::is_same_v<std::random_access_iterator_tag, typename std::iterator_traits<T>::iterator_category>
		void xadvance(T& it, typename std::iterator_traits<T>::difference_type sz) {
		it = it + sz;
		cout << "advance +" << endl;
	}

	template<typename T>
	requires (!std::random_access_iterator<T>)
		// requires 다음에 !을 붙일 때에 괄호 안에 붙여야하는 것을 잊지 말자.
		//requires (!std::is_same_v<std::random_access_iterator_tag, typename std::iterator_traits<T>::iterator_category>)
		void xadvance(T& it, typename std::iterator_traits<T>::difference_type sz) {
		while (sz--) {
			++it;
		}
		cout << "xadvance ++" << endl;
	}
}

namespace BaseFromMember {
	struct Buffer {
		Buffer(int a) {
			cout << "Buffer(int)" << endl;
		}
	};

	class PipeBuffer {
	public:
		PipeBuffer() {
			cout << "PipeBuffer()" << endl;
		}
	protected:
		Buffer buffer{ 100 };
	};

	struct Stream {
		Stream(Buffer& buf) {
			cout << "Stream(Buffer& buf)" << endl;
		}
	};

	class PipeStream : public PipeBuffer, public Stream {
	public:
		PipeStream() : Stream(buffer) {
			cout << "PipeStream()" << endl;
		}
	};
}

namespace IsTransparent {
	struct People {
		string name;
		int age;

		People(string name, int age) : name(name), age(age) {

		}
	};

	struct PeopleCompare {
		bool operator()(const People& p1, const People& p2) const {
			return p1.name < p2.name;
		}

		bool operator()(const People& p1, string name) const {
			return p1.name < name;
		}

		bool operator()(string name, const People& p1) const {
			return p1.name < name;
		}

		using is_transparent = void;
		//using is_transparent = int; // int 이든 void 이든지 상관없다. find 코드보면 Compare 하는 객체 타입에 is_transparent 가 있도록 강제했다.
	};
}

// Coercion : 강제, 여기서는 '강제 변환'이라는 의미
// 아래 코드를 보면 smartptr 의 템플릿 생성자를 통해서 강제 변환을 하고 있다.
namespace CoercionByMemberTemplate {
	class Animal {};

	class Dog : public Animal {};

	template<typename T> class smartptr {
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

namespace Ranges {
	template<typename T>
	class view_interface : public std::ranges::view_base {

		// 자신을 상속받은 파생클래스 타입으로 변환시킨다. begin()과 end() 같은 것을 꺼내려면 이렇게 CRTP 를 써야한다.
		T& Cast() { return static_cast<T&>(*this); }

	public:
		int size() { return Cast().end() - Cast().begin(); } // const 로 다 만들어야 하지만, 일단 간단하게..
	};

	template<typename T> class take_view : public view_interface<take_view<T>> {
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
	take_view(T&& t, size_t)->take_view<views::all_t<T>>;

	template<typename T> class reverse_view : public view_interface<reverse_view<T>> {
		T rng;

	public:
		reverse_view() = default;
		reverse_view(T r) : rng(r) {}

		auto begin() { return make_reverse_iterator(rng.end()); }
		auto end() { return make_reverse_iterator(rng.begin()); }
	};

	// template deduction guide
	template<typename T>
	reverse_view(T&& t)->reverse_view<views::all_t<T>>;

	// 이미 표준에 아래의 all 이 있다.
	template<typename T> auto all(T&& arg) {
		if constexpr (ranges::view<remove_cvref_t<T>>) { // 옆의 concept 를 들어가보면 default 생성자 체크, view_base 상속 여부 체크가 들어있다.
			cout << "view" << endl;

			// view 를 받으면 복사본을 반환, 이를 통해서 take_view(reverse_view(v),3); 같은 임시값을 받는 형태가 가능해진다.
			return remove_cvref_t<T>(arg);
		}
		else {
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

using namespace Ranges;
int main()
{
	vector<int> v = { 1,2,3,4 };
	reverse_view tv(v);

	cout << tv.size() << endl;

	return 1;
}