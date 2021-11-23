#pragma once
#ifndef _WIN32_WINNT        
#define _WIN32_WINNT 0x0500    // 윈도우 2000 이상 지원
#endif 

#include <functional>
#include <stdio.h>
#include <iostream>
#include <crtdbg.h>
#include <conio.h>
#include <array>
#include <unordered_map>
// boost의 asio라이브러리 크로스 플렛폼에 관계된 라이브러리
#include <boost/asio/post.hpp>
#include <boost/function.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/singleton.hpp>
using namespace std;
#if _DEBUG
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define malloc(s) _malloc_dbg(s, _NORMAL_BLOCK, __FILE__, __LINE__)
#endif
using namespace boost::asio;


namespace Decorator {
    /* Component (interface) */
    class Widget {

    public:
        virtual void draw() = 0;
        virtual ~Widget() {}
    };
        
    /* ConcreteComponent */
    class TextField : public Widget {

    private:
        int width, height;

    public:
        TextField(int w, int h) {
            width = w;
            height = h;
        }

        void draw() {
            cout << "TextField: " << width << ", " << height << '\n';
        }
    };

    /* Decorator (interface) */
    class Decorator : public Widget {

    private:
        Widget* wid;       // reference to Widget

    public:
        Decorator(Widget* w) {
            wid = w;
        }

        void draw() {
            wid->draw();
        }

        ~Decorator() {
            delete wid;
        }
    };

    /* ConcreteDecoratorA */
    class BorderDecorator : public Decorator {

    public:
        BorderDecorator(Widget* w) : Decorator(w) { }
        void draw() {
            Decorator::draw();
            cout << "   BorderDecorator" << '\n';
        }
    };

    /* ConcreteDecoratorB */
    class ScrollDecorator : public Decorator {
    public:
        ScrollDecorator(Widget* w) : Decorator(w) { }
        void draw() {
            Decorator::draw();
            cout << "   ScrollDecorator" << '\n';
            test();
        }

        void test() {
            cout << "test" << endl;
        }
    };


    //Widget* aWidget = new BorderDecorator(
    //    new ScrollDecorator(
    //        new TextField(80, 24)));
    //aWidget->draw();
    //
    //ScrollDecorator* b = (ScrollDecorator*)aWidget;
    //b->test();
    //
    //delete aWidget;
}

namespace Singleton
{
   class test {
    public:
        static test& getIns() {
            if (ins == NULL) {
                ins = new test();
            }

            return *ins;
        }

        void func() {
            cout << "func" << endl;
        }
    private:
        test() {
            cout << "test 생성됨" << endl;
        }

        static test* ins;
    };
};


namespace Tree {
#pragma region tree
    struct Node

    {

        int data;

        Node* left;

        Node* right;

    };

    Node* Root;



    void InitTree(int data)

    {

        Root = (Node*)malloc(sizeof(Node));

        Root->data = data;

    }



    Node* AddChild(Node* Parent, int data, BOOL bLeft)

    {

        Node* New;



        New = (Node*)malloc(sizeof(Node));

        New->data = data;

        New->left = NULL;

        New->right = NULL;

        if (bLeft) {

            Parent->left = New;

        }
        else {

            Parent->right = New;

        }

        return New;

    }



    void PreOrder(Node* R)

    {

        printf("%d ", R->data);

        if (R->left) PreOrder(R->left);

        if (R->right) PreOrder(R->right);

    }



    void InOrder(Node* R)

    {

        if (R->left) InOrder(R->left);

        printf("%d ", R->data);

        if (R->right) InOrder(R->right);

    }



    void PostOrder(Node* R)

    {

        if (R->left) PostOrder(R->left);

        if (R->right) PostOrder(R->right);

        printf("%d ", R->data);

    }



    void FreeTree(Node* R)

    {

        if (R->left) FreeTree(R->left);

        if (R->right) FreeTree(R->right);

        free(R);

    }
}


namespace RvalueReference_Forwarding {

    class A {
    public:
        A([[maybe_unused]]int& _a) {
            cout << "A int& " << endl;
        }
    };
    class B {
    public:

        B(const int& _b) {
            cout << "B const int& " << endl;
        }

        B(int&& b) {
            cout << "B int&&" << endl;
        }


    };

    template <typename T, typename U>
    T* Factory(U&& arg) { // 다른 클래스를 생성해서 리턴해주는 함수
        return new T(static_cast<U&&>(arg));
    }

    /* int j = 10;
        int& i = j;
        int&& c = 20;

        A* aa = Factory<A>(i);
        B* bb = Factory<B>(10);*/
}
#pragma region RvalueReference


#pragma endregion


namespace Casting {
    class refTest {
    public:
        int a;
        int b;

        virtual void func() {
            cout << "parent" << endl;
            cout << a << endl;
            cout << b << endl;
        }
    };


    class refTestSon : public refTest {
    public:
        int s_a;
        int s_b;

        void func() {
            cout << "son" << endl;
            cout << s_a << endl;
            cout << s_b << endl;
        }
    };
    void printStr(unsigned char* str) {
        cout << str << endl;
    }

    //refTest* p = new refTest();
        //refTestSon* s = new refTestSon();

        //refTest* temp1 = s; // 자식을 가리키는 부모 포인터
        //refTestSon* temp2 = dynamic_cast<refTestSon*>(temp1);
        //temp2 = static_cast<refTestSon*>(temp1);
        //temp2->func(); // 가능, 동적으로 pSonParent 가 가리키는 것이 자식임을 파악해서 temp로 넘겨줘서 문제가 없다.

        //temp1 = p; // 부모를 가리키는 부모 포인터
        //temp2 = dynamic_cast<refTestSon*>(temp1);
        ////temp2 = static_cast<refTestSon*>(temp1); // static cast로 하면 자식으로 바꿔서 할당했는데 부모 함수가 호출이 된다. temp1이 부모를 가리키고 있는데 이것을 체크 안해주는 것이다.
        //if (temp2 == NULL)
        //    cout << "dynamic_cast 실패해서 NULL이 되었다. temp1이 부모를 가리키고 있는 상황이다. 자식으로 변환될 수 없다." << endl;
        //else
        //    temp2->func();

        //delete p;
        //delete s;

}



namespace hash {
    class hashTestClass {
    public:
        int a;
        int b;

        bool operator==(const hashTestClass& a) const {
            if (this->a == a.a && this->b == a.b) {
                return true;
            }
            else {
                return false;
            }
        }
    };

    class hashTestFunc {
    public:
        size_t operator()(const hashTestClass& a) const {
            return a.a;
        }
    };
}

namespace RAII_SmartPointer {
    // https://openmynotepad.tistory.com/33 <- 링크 참조 
    // https://webnautes.tistory.com/1451 <- shared_ptr 참조 카운팅 관련 설명 자세함.
    class test {
    public:
        int a;
        int b;

        ~test(){
            cout << "a = " << a << endl;
            cout << "test destructor" << endl;
        }
    };

    void test_func(shared_ptr<test>& t) {
        cout << "in test_func()" << endl;
        cout << t.use_count() << endl;
    }

    /* shared_ptr<test> a(new test());
   weak_ptr<test> b = a;

   cout << a.use_count() << endl;
   cout << b.use_count() << endl;

   cout << b.expired() << endl;
   (*(b.lock())).a = 150;
   a.reset();*/
}

namespace C_Based_VirtualFuction {
    typedef struct s_person {
        char* name;
        int age;
        char* company;
        char* address;
    } t_person;

    typedef struct s_dog {
        char* name;
        int age;
        char* owner_name;
    } t_dog;

    void walk_person(void* raw_self) {
        t_person* self;

        self = (t_person*)raw_self;
        printf("%s은(는) 걸어간다..\n", self->name);
    }

    void walk_dog(void* raw_self) {
        t_dog* self;

        self = (t_dog*)raw_self;
        printf("%s은(는) 주인인 %s와(과) 함께 산책한다..\n", self->name, self->owner_name);
    }

    typedef struct s_bag {
        void** arr;
        int len;
        int max;
    } t_bag;

    // bool 정의
    typedef int t_bool;
    #define TRUE 1
    #define FALSE 0

    t_bool m(void* target, size_t size) { // 메모리 할당을 해주고 잘 되었는지 확인해주는 함수
        void** pt;

        pt = (void**)target;
        *pt = malloc(size);
        if (*pt == 0)
            return (FALSE);

        return (TRUE);
    }

    t_bag create_bag() {
        t_bag bag;

        bag.len = 0;
        bag.max = 10;

        if (!m(&bag.arr, sizeof(void*) * 10)) // 메모리할당 실패하면 종료
            exit(0);

        return bag;
    }

    void bag_add(t_bag* bag, void* item) {
        void** new_arr;

        bag->arr[bag->len] = (void*)item;
        bag->len += 1;
        if (bag->len > bag->max / 2) {
            bag->max *= 2;
            m(&new_arr, sizeof(void*) * bag->max);
            for (int i = 0; i < bag->len; i++)
                new_arr[i] = bag->arr[i];
            free(bag->arr);
            bag->arr = new_arr;
        }
    }

    void bag_foreach(t_bag* bag, void(*func)(void*)) {
        for (int i = 0; i < bag->len; i++)
            func(bag->arr[i]);
    }

    typedef struct s_walkable { // walk 기능을 구현하고 있음을 알려주는 구조체, t_bag 에 저장되는 것들은 모두 t_walkable 을 저장하고 있다고 생각해야 한다.
        void (*walk)(void* raw_self); // 실행할 함수
        void* object; // 실행할 주체, 어떤 함수를 실행할지를 정해준다.
    }t_walkable;

    typedef struct s_vtables {
        t_bag walkables;
    } t_vtables;

    t_vtables* get_vtables() {
        static t_vtables t;

        if (!t.walkables.arr)
            t.walkables = create_bag();
        return &t;
    }

    t_walkable* create_walkable(void* obj, void(*walk_handler)(void*)) {
        t_walkable* walkable; // 실행할 함수, 실행할 주체가 담겨 있다.

        m(& walkable, sizeof(t_walkable));
        walkable->object = obj;
        walkable->walk = walk_handler;
        bag_add(&get_vtables()->walkables, walkable);
        return walkable;
   }

    void create_person(t_person p) {
        t_person* person;

        m(&person, sizeof(t_person));
        person->address = p.address;
        person->age = p.age;
        person->company = p.company;
        person->name = p.name;
        create_walkable(person, walk_person);
    }

    void create_dog(t_dog d) {
        t_dog* dog;

        m(&dog, sizeof(t_dog));
        dog->age = d.age;
        dog->name = d.name;
        dog->owner_name = d.owner_name;
        create_walkable(dog, walk_dog);
    }

    void do_walk(void* ptr) {
        t_walkable* walkable;

        walkable = (t_walkable*)ptr;
        walkable->walk(walkable->object);
    }

    void all_walk() {
        bag_foreach(&get_vtables()->walkables, do_walk);
    }

    void test_all_walk() {
        t_person p;
        p.address = (char*)"사당동";
        p.age = 31;
        p.name = (char*)"김대언";
        p.company = (char*)"LoadComplete";

        t_dog dog;
        dog.age = 5;
        dog.name = (char*)"뿌꾸";
        dog.owner_name = (char*)"김대언";

        create_person(p);
        create_dog(dog);
        all_walk();

    }

    // test_all_walk();
}


namespace ExceptionTest {
    class stack_unwind {
    public:
        int a;
        stack_unwind() {
            cout << "stack_unwind constructor" << endl;
            throw 123;
        }
        ~stack_unwind() {
            cout << "stack_unwind destructor" << endl;
        }
    };
    void throw_test(int a) {
        if (a == 1)
            throw out_of_range("exception test 1");
        else
            throw runtime_error("exception test 2");
    }

    void stack_unwindling_test() {
        stack_unwind a;

        throw_test(2);
    }

    /*try {
            stack_unwindling_test();
        }
        catch (int e) {
            cout << e << endl;
        }
        catch (...) {
            cout << "default" << endl;
        }*/
}

namespace effective_cpp {
    namespace ch3_use_const {
        class tt {
        public:
            static const int aa = 50;
            int scores[aa];

            tt() {
                cout << "const" << endl;
            }

            ~tt() {
                cout << "dest" << endl;
            }
        };

        class CTextBook {
        public:
            size_t length() const;

            CTextBook(const char* in) {
                pText = (char*)in;
            }
            char* pText;

            mutable size_t textLength;
            mutable bool isLengthValid;

            void t1() const {
                cout << "t1 const 용" << endl;
            }

            void t1() {
                cout << "t1 non-const 용" << endl;
            }

            const char& operator[](size_t position) const {
                cout << "const [] 호출" << endl;
                return pText[position];
            }

            char& operator[](size_t position) {
                cout << "non-const [] 호출" << endl;

                return
                    const_cast<char&>(
                        (static_cast<const CTextBook&>(*this))[position]
                        );
            }

            //char bb[] = "asdfasdf";
            //CTextBook a(bb);
            //const CTextBook c(a);

            //a[0] = 't';
            //cout << a.pText << endl;
            //cout << c.pText << endl;
            //
            //a.t1();
            //c.t1();
        };
    } // ch3_use_const 

    namespace ch4_must_initialize_object {
        class t {
        public:
            const int a;
            int b;

            t() :a(10), b(10) {
                cout << "t 기본 생성자 호출" << endl;
            }
        };

        class dt : public t {
        public:
            const int a;
            int b;

            dt() :a(20), b(20) {
                cout << "dt 기본 생성자 호출" << endl;
            }
        };

        class tt {
        public:
            // 선언된 순서대로 멤버 초기화리스트에서 초기화된다.
            t a;
            dt b;
            int c;

            tt() : a(), b(), c() { // b(), a(), c()를 해도 a,b,c 순으로 초기화된다.
                // 위와 같이 멤버 이름에 인수를 안넣어서 기본 생성자를 호출할 수도 있다.
                cout << "tt 멤버 초기화 리스트 쓴 생성자" << endl;
            }

            ~tt() {
                cout << "tt 파괴자 호출" << endl;
            }

            //tt(const t& in1, const int in2){
            //    // 멤버 초기화 리스트 안써서 in1이 기본 생성자를 호출하는 경우
            //    cout << "멤버 초기화 리스트 안쓴 생성자" << endl;
            //    a = in1;
            //    b = in2;
            //}
        };

        tt& f() {
            static tt a;
            cout << "f" << endl;
            cout << a.a.a << endl;
            cout << a.b.a << endl;

            return a;
        }

        /*tt a = f();
        a.a.b = 30;
        cout << a.a.b << endl;*/
    } // ch4_must_initialize_object 

    namespace  ch_6_ban_unnecessary_default_function {

        class uncopyable {
        public:
            uncopyable() {};
            ~uncopyable() {};

        private:
            // 이렇게 해놓으면 이것을 상속받는 클래스는 대입 과정에서 아래 함수들이 호출될 수 밖에 없기에 
            // = 연산과 복사 생성자가 기능이 막히는 것이다.
            uncopyable(const uncopyable& rhs) {};
            uncopyable& operator =(const uncopyable& rhs) {};
        };

        class HomeForSale : uncopyable {
        public:
            int id;
            HomeForSale(int _id = 0) :id(_id) {

            }
        };


        //HomeForSale i;
        //HomeForSale i2 = i;

    } // ch_6_ban_unnecessary_default_function 

    namespace ch_12_copy_everything {
        class customer {
        public:
            string name;

            customer() : name("default"){
                cout << "customer default constructor" << endl;
            }
            customer(const customer& rhs) : name(rhs.name){
                cout << "customer copy constructor" << endl;
            }

            customer& operator=(const customer& rhs) {
                cout << "customer = operator" << endl;
                name = rhs.name;
                return *this;
            }
        };

        class vip_customer : public customer {
        public:
            int vip_num;
            vip_customer() :vip_num(0) {
                cout << "vip_customer default constructor" << endl;
            }

            vip_customer(const vip_customer& rhs) : customer(rhs), vip_num(rhs.vip_num) {
                cout << "vip_customer copy constructor" << endl;
            }

            customer& operator=(const vip_customer& rhs) {
                cout << "vip_customer = operator" << endl;
                customer::operator=(rhs);
                vip_num = rhs.vip_num;
                return *this;
            }
        };

        //vip_customer b;

        //b.name = "aa";
        //b.vip_num = 10;

        //vip_customer c(b);

        //vip_customer a;
        //a = b;

        //cout << a.name << endl;
        //cout << c.name << endl;

    }// ch_12_copy_everything 
}

namespace variadic_arguments_and_templates {
    // 가변 인자 및 템플릿 테스트용 코드
    template <typename T>
    void myPrint(T arg) {
        cout << arg << endl;
    }

    template <typename T, typename... Types>
    void myPrint(T arg, Types... args) {
        cout << arg << endl;
        myPrint(args...);
    }

    #define makeArray(TypeName, variableName, ...)\
    char TypeName[5] = ###variableName; \
    myPrint(##__VA_ARGS__);


    //int c = 1000;
    //makeArray(AAA, c, 10);

}


struct myStruct {
    int a=3;
    int b=4;
};

int32_t HoldItem(short* a)  
{
    return ::InterlockedIncrement16(a);
}

template <class T, typename std::enable_if<std::is_integral<T>::value,
    T>::type* = nullptr>
    void do_stuff(T& t) {
    std::cout << "do_stuff integral\n";
    // 정수 타입들을 받는 함수 (int, char, unsigned, etc.)
}

template <class T,
    typename std::enable_if<std::is_class<T>::value, T>::type* = nullptr>
    void do_stuff(T& t) {
    // 일반적인 클래스들을 받음
}



class myClass : public boost::serialization::singleton<myClass> {
public:
    int a;
    int value_type;
};



//template <typename T>
//struct enable_if<true, T> {
//    typedef T type;
//
//    enable_if() {
//        cout << "enable_if true 있는 버전" << endl;
//    }
//};



namespace enable_if_test {

    template <bool _Test, typename _Ty = void>
    struct my_enable_if {
        my_enable_if() {
            cout << "my_enable_if _Ty = void" << endl;
        }
    };

    template <typename _Ty>
    struct my_enable_if<true, _Ty> {
        using type = _Ty;

        my_enable_if() {
            cout << "my_enable_if _Ty = true" << endl;
        }
    };


    template <typename T>
    typename enable_if_t<is_class_v<T>, void>
        ffff(T a) {
        cout << "class 형" << endl;
    }

    template <typename T>
    typename enable_if_t<is_integral_v<T>, void>
        ffff(T a) {
        cout << "int 형" << endl;
    }

    //my_enable_if<true, int> a;
    //my_enable_if<false, int> b;


}




namespace IOCP_test {

    int iocpTest = -1;

    struct myOverlapped {
        OVERLAPPED overLapped;
        int number;
    };

    DWORD WINAPI myCallBack(LPVOID completionPort) {
        DWORD NumberOfByteTransfered = 0;
        VOID* CompletionKey = NULL;
        OVERLAPPED* overlappedPointer = NULL;


        while (true) {
            auto success = GetQueuedCompletionStatus(
                (HANDLE)completionPort,
                &NumberOfByteTransfered,
                (LPDWORD)&CompletionKey,
                &overlappedPointer,
                INFINITE);


            if (success) {
                myOverlapped* mO = (myOverlapped*)overlappedPointer;
                while (true) {
                    iocpTest = mO->number;
                }
                //Sleep(10);
                //PostQueuedCompletionStatus(completionPort, 0, 0, overlappedPointer);
            }
        }
    }


    void IOCP_test() { 
        // sleep을 안걸었는데.. 왜 5개 스레드가 모두 작동할까? 
        // cout 같은 IO 인터럽트가 호출되면 IOCP에서 그 스레드는 작업 중이 아닌 스레드로 판단하는 것 같다.
        int workerThreadCount = 5; 
        HANDLE hIOCP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 3);
        vector<HANDLE> workerThreadVector;

        DWORD NumberOfByteTransfered = 0;
        VOID* CompletionKey = NULL;
        OVERLAPPED* overlappedPointer = NULL;

        for (DWORD i = 0; i < workerThreadCount; i++)
        {
            HANDLE WorkerThread = CreateThread(NULL, 0, myCallBack, hIOCP, 0, NULL);
            workerThreadVector.push_back(WorkerThread);
        }

        myOverlapped a1;
        a1.number = 1;

        myOverlapped a2;
        a2.number = 2;

        myOverlapped a3;
        a3.number = 3;

        myOverlapped a4;
        a4.number = 4;

        myOverlapped a5;
        a5.number = 5;

        if (hIOCP) {
            PostQueuedCompletionStatus(hIOCP, 0, 0, (LPOVERLAPPED)&a1);
            PostQueuedCompletionStatus(hIOCP, 0, 0, (LPOVERLAPPED)&a2);
            PostQueuedCompletionStatus(hIOCP, 0, 0, (LPOVERLAPPED)&a3);
            PostQueuedCompletionStatus(hIOCP, 0, 0, (LPOVERLAPPED)&a4);
            PostQueuedCompletionStatus(hIOCP, 0, 0, (LPOVERLAPPED)&a5);
        }

        char key;
        while (true) {
            key = _getch();
            cout << iocpTest;
            if (key == 'e') {
                break;
            }

            if (key == 'n') {
                cout << endl;
            }

        }

        for (DWORD i = 0; i < workerThreadVector.size(); i++)
        {
            CloseHandle(workerThreadVector[i]);
        }

        if (hIOCP) {
            CloseHandle(hIOCP);
        }
    }
    //IOCP_test::IOCP_test();

}

namespace emplace_back_test {

    class vt {
    public:
        vt() {
            cout << "기본 생성자 호출" << endl;
        }

        vt(const vt& _a) {
            cout << "복사 생성자 호출" << endl;
        }

        vt(vt&& _a) {
            cout << "이동 생성자 호출" << endl;
        }
    };

    //vector<vt> bb;
    //vt item;
    //cout << endl;

    //cout << "emplace_back인 경우" << endl;
    //bb.emplace_back(item);

    //cout << endl;
    //cout << "push_back인 경우" << endl;
    //bb.push_back(item);

    // 참조 링크 https://shaeod.tistory.com/630
}



int main()
{

    IOCP_test::IOCP_test();

    _CrtDumpMemoryLeaks();  
    return 0;
}
