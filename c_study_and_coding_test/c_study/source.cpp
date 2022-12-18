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

			// �޸� �ʱ�ȭ �߿� ���ܰ� �߻��Ѵٸ� �޸� �ʱ�ȭ ����
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

			// �޸� �ʱ�ȭ �߿� ���ܰ� �߻��Ѵٸ� �޸� �ʱ�ȭ ����
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
			refCnt.fetch_add(1, std::memory_order_relaxed); // �޸� ���� �ٲ� ���x
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
			// �̰� �� �Ʒ��δ� ��� ���� ���ٲٰ� �ϴ� ��.
			// acquire �� �ڱ� ���� �ִ°� �Ʒ��� ������ �ϰ�, release �� �ڱ� �Ʒ� �ִ°� ���� ������ �Ѵ�.
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
	void advance_imp(T& it, typename iterator_traits<T>::difference_type sz, random_access_iterator_tag) {
		it = it + sz;
		cout << "advance using +" << endl;
	}

	// ���⼭ input_iterator_Tag �� ���� Ŭ������ random_access_iterator_tag �� �ƴϸ� �� ����� ���´�.
	template<typename T>
	void advance_imp(T& it, typename iterator_traits<T>::difference_type sz, input_iterator_tag) {

		while (sz--) {
			++it;
		}
		cout << "advance using ++" << endl;
	}

	template<typename T>
	// iterator_traits ���� difference_type �̶� ���� �־ + ���꿡 ���Ǵ� Ÿ�Ե� ����Ǿ� �ִٴ� ���� ����.
	void xadvance(T& it, typename iterator_traits<T>::difference_type sz) {
		// �Ʒ� �κп��� iterator_traits<T> �� ���ؼ� iterator_category �� ������ ���� �߿��ϴ�.�迭���� ���� T* ���¸� ������ ���� �߿�!
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
		// �̷��� �Ǹ� if constexpr�� �����̸� �ڵ� ��ü�� ������ �ȵǾ ������ �Ȼ����. C++ 17���� �̰� ���Ե�.
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
	requires std::random_access_iterator<T> // �Ʒ� �ڵ� ����ؼ� �Ȱ��� Concept �� �����.
	//requires std::is_same_v<std::random_access_iterator_tag, typename std::iterator_traits<T>::iterator_category>
		void xadvance(T& it, typename std::iterator_traits<T>::difference_type sz) {
		it = it + sz;
		cout << "advance +" << endl;
	}

	template<typename T>
	requires (!std::random_access_iterator<T>)
		// requires ������ !�� ���� ���� ��ȣ �ȿ� �ٿ����ϴ� ���� ���� ����.
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
		//using is_transparent = int; // int �̵� void �̵��� �������. find �ڵ庸�� Compare �ϴ� ��ü Ÿ�Կ� is_transparent �� �ֵ��� �����ߴ�.
	};
}

// Coercion : ����, ���⼭�� '���� ��ȯ'�̶�� �ǹ�
// �Ʒ� �ڵ带 ���� smartptr �� ���ø� �����ڸ� ���ؼ� ���� ��ȯ�� �ϰ� �ִ�.
namespace CoercionByMemberTemplate {
	class Animal {};

	class Dog : public Animal {};

	template<typename T> class smartptr {
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

namespace Ranges {
	template<typename T>
	class view_interface : public std::ranges::view_base {

		// �ڽ��� ��ӹ��� �Ļ�Ŭ���� Ÿ������ ��ȯ��Ų��. begin()�� end() ���� ���� �������� �̷��� CRTP �� ����Ѵ�.
		T& Cast() { return static_cast<T&>(*this); }

	public:
		int size() { return Cast().end() - Cast().begin(); } // const �� �� ������ ������, �ϴ� �����ϰ�..
	};

	template<typename T> class take_view : public view_interface<take_view<T>> {
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

	// �̹� ǥ�ؿ� �Ʒ��� all �� �ִ�.
	template<typename T> auto all(T&& arg) {
		if constexpr (ranges::view<remove_cvref_t<T>>) { // ���� concept �� ������ default ������ üũ, view_base ��� ���� üũ�� ����ִ�.
			cout << "view" << endl;

			// view �� ������ ���纻�� ��ȯ, �̸� ���ؼ� take_view(reverse_view(v),3); ���� �ӽð��� �޴� ���°� ����������.
			return remove_cvref_t<T>(arg);
		}
		else {
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

using namespace Ranges;
int main()
{
	vector<int> v = { 1,2,3,4 };
	reverse_view tv(v);

	cout << tv.size() << endl;

	return 1;
}