#pragma once
#ifndef _WIN32_WINNT        
#define _WIN32_WINNT 0x0500    // ������ 2000 �̻� ����
#endif 

#include <functional>
#include <stdio.h>
#include <iostream>
#include <crtdbg.h>
#include <conio.h>
#include <array>
#include <unordered_map>
#include <bitset>
#include <json/json.h>

// boost�� asio���̺귯�� ũ�ν� �÷����� ����� ���̺귯��
#include <boost/asio/post.hpp>
#include <boost/function.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/singleton.hpp>
#include <boost/format.hpp>

#include <Windows.h>
#include <mutex>
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
            cout << "test ������" << endl;
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
    T* Factory(U&& arg) { // �ٸ� Ŭ������ �����ؼ� �������ִ� �Լ�
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

        //refTest* temp1 = s; // �ڽ��� ����Ű�� �θ� ������
        //refTestSon* temp2 = dynamic_cast<refTestSon*>(temp1);
        //temp2 = static_cast<refTestSon*>(temp1);
        //temp2->func(); // ����, �������� pSonParent �� ����Ű�� ���� �ڽ����� �ľ��ؼ� temp�� �Ѱ��༭ ������ ����.

        //temp1 = p; // �θ� ����Ű�� �θ� ������
        //temp2 = dynamic_cast<refTestSon*>(temp1);
        ////temp2 = static_cast<refTestSon*>(temp1); // static cast�� �ϸ� �ڽ����� �ٲ㼭 �Ҵ��ߴµ� �θ� �Լ��� ȣ���� �ȴ�. temp1�� �θ� ����Ű�� �ִµ� �̰��� üũ �����ִ� ���̴�.
        //if (temp2 == NULL)
        //    cout << "dynamic_cast �����ؼ� NULL�� �Ǿ���. temp1�� �θ� ����Ű�� �ִ� ��Ȳ�̴�. �ڽ����� ��ȯ�� �� ����." << endl;
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
    // https://openmynotepad.tistory.com/33 <- ��ũ ���� 
    // https://webnautes.tistory.com/1451 <- shared_ptr ���� ī���� ���� ���� �ڼ���.
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
        printf("%s��(��) �ɾ��..\n", self->name);
    }

    void walk_dog(void* raw_self) {
        t_dog* self;

        self = (t_dog*)raw_self;
        printf("%s��(��) ������ %s��(��) �Բ� ��å�Ѵ�..\n", self->name, self->owner_name);
    }

    typedef struct s_bag {
        void** arr;
        int len;
        int max;
    } t_bag;

    // bool ����
    typedef int t_bool;
    #define TRUE 1
    #define FALSE 0

    t_bool m(void* target, size_t size) { // �޸� �Ҵ��� ���ְ� �� �Ǿ����� Ȯ�����ִ� �Լ�
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

        if (!m(&bag.arr, sizeof(void*) * 10)) // �޸��Ҵ� �����ϸ� ����
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

    typedef struct s_walkable { // walk ����� �����ϰ� ������ �˷��ִ� ����ü, t_bag �� ����Ǵ� �͵��� ��� t_walkable �� �����ϰ� �ִٰ� �����ؾ� �Ѵ�.
        void (*walk)(void* raw_self); // ������ �Լ�
        void* object; // ������ ��ü, � �Լ��� ���������� �����ش�.
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
        t_walkable* walkable; // ������ �Լ�, ������ ��ü�� ��� �ִ�.

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
        p.address = (char*)"��絿";
        p.age = 31;
        p.name = (char*)"����";
        p.company = (char*)"LoadComplete";

        t_dog dog;
        dog.age = 5;
        dog.name = (char*)"�Ѳ�";
        dog.owner_name = (char*)"����";

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
                cout << "t1 const ��" << endl;
            }

            void t1() {
                cout << "t1 non-const ��" << endl;
            }

            const char& operator[](size_t position) const {
                cout << "const [] ȣ��" << endl;
                return pText[position];
            }

            char& operator[](size_t position) {
                cout << "non-const [] ȣ��" << endl;

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
                cout << "t �⺻ ������ ȣ��" << endl;
            }
        };

        class dt : public t {
        public:
            const int a;
            int b;

            dt() :a(20), b(20) {
                cout << "dt �⺻ ������ ȣ��" << endl;
            }
        };

        class tt {
        public:
            // ����� ������� ��� �ʱ�ȭ����Ʈ���� �ʱ�ȭ�ȴ�.
            t a;
            dt b;
            int c;

            tt() : a(), b(), c() { // b(), a(), c()�� �ص� a,b,c ������ �ʱ�ȭ�ȴ�.
                // ���� ���� ��� �̸��� �μ��� �ȳ־ �⺻ �����ڸ� ȣ���� ���� �ִ�.
                cout << "tt ��� �ʱ�ȭ ����Ʈ �� ������" << endl;
            }

            ~tt() {
                cout << "tt �ı��� ȣ��" << endl;
            }

            //tt(const t& in1, const int in2){
            //    // ��� �ʱ�ȭ ����Ʈ �ȽἭ in1�� �⺻ �����ڸ� ȣ���ϴ� ���
            //    cout << "��� �ʱ�ȭ ����Ʈ �Ⱦ� ������" << endl;
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
            // �̷��� �س����� �̰��� ��ӹ޴� Ŭ������ ���� �������� �Ʒ� �Լ����� ȣ��� �� �ۿ� ���⿡ 
            // = ����� ���� �����ڰ� ����� ������ ���̴�.
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
    // ���� ���� �� ���ø� �׽�Ʈ�� �ڵ�
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
    // ���� Ÿ�Ե��� �޴� �Լ� (int, char, unsigned, etc.)
}

template <class T,
    typename std::enable_if<std::is_class<T>::value, T>::type* = nullptr>
    void do_stuff(T& t) {
    // �Ϲ����� Ŭ�������� ����
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
//        cout << "enable_if true �ִ� ����" << endl;
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
        cout << "class ��" << endl;
    }

    template <typename T>
    typename enable_if_t<is_integral_v<T>, void>
        ffff(T a) {
        cout << "int ��" << endl;
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
        // sleep�� �Ȱɾ��µ�.. �� 5�� �����尡 ��� �۵��ұ�? 
        // cout ���� IO ���ͷ�Ʈ�� ȣ��Ǹ� IOCP���� �� ������� �۾� ���� �ƴ� ������� �Ǵ��ϴ� �� ����.
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
            cout << "�⺻ ������ ȣ��" << endl;
        }

        vt(const vt& _a) {
            cout << "���� ������ ȣ��" << endl;
        }

        vt(vt&& _a) {
            cout << "�̵� ������ ȣ��" << endl;
        }
    };

    //vector<vt> bb;
    //vt item;
    //cout << endl;

    //cout << "emplace_back�� ���" << endl;
    //bb.emplace_back(item);

    //cout << endl;
    //cout << "push_back�� ���" << endl;
    //bb.push_back(item);

    // ���� ��ũ https://shaeod.tistory.com/630
}


namespace template_meta_programming {
    template <int X, int Y>
    struct GCD {
        static const int value = GCD<Y, X% Y>::value;
    };

    template <int X>
    struct GCD<X, 0> {
        static const int value = X;
    };

    template <int N, int D = 1>
    struct Ratio {
        typedef Ratio<N, D> type;
        static const int num = N;  // ����
        static const int den = D;  // �и�
    };
    template <class R1, class R2>
    struct _Ratio_add {
        typedef Ratio<R1::num* R2::den + R2::num * R1::den, R1::den* R2::den> type;
    };

    template <class R1, class R2>
    struct Ratio_add : _Ratio_add<R1, R2>::type {};

    enum myE {
        aa = 30
    };

    //typedef Ratio<2, 4> rat;
    //typedef Ratio<4, 2> rat2;
    //typedef Ratio_add<rat, rat2> rat3;

    //std::cout << rat3::num << " / " << rat3::den << std::endl;

}


namespace lock_test {
    std::recursive_mutex g_num_mutex;
    constexpr int rountine_count = 100;
    static unsigned int g_num = 0;

    struct t {
        int id;

        void real_increment() {
            cout << this->id << "���� " << "lock ȹ�� �õ� ��" << '\n';
            g_num_mutex.lock();
            for (int i = 0; i < rountine_count; ++i) {
                ++g_num;
                std::cout << this->id << " => " << g_num << '\n';
            }
            g_num_mutex.unlock();
        }
    };
    void lock_increment(t* _t)
    {
        _t->real_increment();
    }

    void trylock_increment(t* _t)
    {
        int count = 0;
        do
        {
            // ���ؽ� ȹ�� ����
            if (g_num_mutex.try_lock())
            {
                ++g_num;
                count++;
                std::cout << _t->id << " => " << g_num << '\n';
                g_num_mutex.unlock();
            }
            // else ���ؽ� ȹ�� ����
        } while (count < rountine_count);
    };

    //t a;
    //a.id = 0;
    //t b;
    //b.id = 1;
    //t c;
    //c.id = 2;
    //std::thread t1(lock_increment, &a);
    //std::thread t2(lock_increment, &b);
    //std::thread t3(lock_increment, &c);
    //t1.join();
    //t2.join();
    //t3.join();

}

using namespace lock_test;

constexpr int tt(int n) {
    int result = 0;

    if (n <= 1)
        result = 1;
    else
        result = n * tt(n - 1);

    return result;
}

namespace const_expr {
    template<int n>
    struct constN {
        // constexpr�� ���� �Լ��� ��� ���ͷ����� Ȯ���ϴ� ���ø�
        constN() { std::cout << n << endl; }
    };

    //constexpr int rand_test() {
    //    // ��� ���� ���� ���� ��� �̷� ������ ������ ������ ���.
    //    return rand() % 655;
    //}
    //constexpr int a = tt(5);

    //constexpr int b = tt(4);
}

namespace string_view {
    // c++ 14����
    std::string FiveCharacterOnlyString(const std::string& str)
    {
        if (str.size() < 5) return str;
        return str.substr(0, 5);
    }

    // c++ 17 string_view
    // string_view�� ����ϸ� string ��ü�� ���簡 �߻����� �ʽ��ϴ�. 
    std::string_view FiveCharacterOnlyStringView(const std::string_view str)
    {
        if (str.size() < 5) return str;
        return str.substr(0, 5);
    }

    void UsedString(std::string& str)
    {
        // string ���� ��ü�� ���� ������ �̷������.
        str = "�Ǽ��� ������ ���� �õ�";
    }

    void UsedStringView(std::string_view str_v)
    {
        // string_view ���� ��ü�� ���� ������ �߻����� �ʴ´�.
        str_v = "�Ǽ��� ������ ���� �õ�";
    }

    void CallerOriginStringObject(std::string_view str_v, std::string& str)
    {
        std::cout << "[origindata] str_v = " << str_v.data() << " Size = " << str_v.size() << std::endl;
        str = "modify";
        // ���������Ͱ� ����Ǹ� string_view�� �����Ͱ� ����˴ϴ�.
        // ���ڿ� : "modify", size = 15
        // �����ִ� �� �������� ���� �����Ͱ� ��� �ֽ��ϴ�. 
        std::cout << "[origindata modify] str_v = " << str_v.data() << " Size = " << str_v.size() << std::endl;

        // original string -> modify l string �̷��� �� ����, �������� ������ ���ڿ� null�� �־ a�� ��������.
        std::cout << str_v.data()[6] << std::endl;
        std::cout << str_v.data()[7] << std::endl;
        std::cout << str_v.data()[8] << std::endl;
        std::cout << str_v.data()[9] << std::endl;
        std::cout << str_v.data()[10] << std::endl;
        std::cout << str_v.data()[11] << std::endl;
        std::cout << str_v.data()[12] << std::endl;
        std::cout << str_v.data()[13] << std::endl;
        std::cout << str_v.data()[14] << std::endl;

        // ���� string �����ʹ� size���� ��� �ٲ����.
        std::cout << str << std::endl;
        std::cout << str.size() << std::endl;
       
        // string_view ��ü�� ���� �����͸� ������ �� ������, �ٶ󺸰� �ִ� ���� �����Ͱ� �����Ǹ� string_view�� �����ȴ�.
        // �� ���� string_view �� �ʱ��� size�� �����Ǿ� �ֱ� ������ �������� ���� ������ �߻��� �� �ִ�.
        // �׷��⿡ string_view�� ��� ���߿��� ȣ���ϴ� ���忡�� ���� �����Ͱ� �������� ���� ���� ��������� �Ѵ�.

        // �̷��� �� ������ ���������� const ���ڿ� �����Ϳ� ����� �����ϴ� ������ basic_string_view�� ���� �����̴�.
        // �̷� ���� string ó�� ���ڿ� ���簡 �ƴ϶� ������ ������, ������ ������ �Ұ����ؼ� �б� �������θ� ����� �����ϴ�.
    }

    //// std::string val1 = "12345"
    //auto val1 = FiveCharacterOnlyString("123456789");
    //// std::string_view val2 = "12345"
    //auto val2 = FiveCharacterOnlyStringView("123456789");

    //cout << val1 << endl;
    //cout << val2 << endl;

    //UsedString(val1);
    //UsedStringView(val2);

    //cout << val1 << endl;
    //cout << val2 << endl;

    //std::string str("original string");
    //CallerOriginStringObject(str, str);

    //string a = "asd";
    //string b = "ddd";

    //std::string_view c = a;
    //cout << c << endl;

    ////c[0] = 'a'; // �Ҵ� �Ұ�
    //c = b;
    //cout << c << endl;

}

using namespace string_view;

int main()
{
    _CrtDumpMemoryLeaks();  
    return 0;
}
