#include<iostream>
#include<Windows.h>
#include <vector>
#include <assert.h>
#include <map>
#include <unordered_map>
#include <crtdbg.h>
#if _DEBUG
#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define malloc(s) _malloc_dbg(s, _NORMAL_BLOCK, __FILE__, __LINE__)
#endif
using namespace std;



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
        A(int& _a) {
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
}


using namespace RAII_SmartPointer;
int main()
{

    shared_ptr<test> a(new test());
    weak_ptr<test> b = a;

    cout << a.use_count() << endl;
    cout << b.use_count() << endl;

    cout << b.expired() << endl;
    (*(b.lock())).a = 150;
    a.reset();


    _CrtDumpMemoryLeaks();
    return 0;

}
