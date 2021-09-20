#include<iostream>
#include<Windows.h>
#include <vector>
#include <assert.h>
#include <map>
#include <unordered_map>
using namespace std;


#pragma region Decorator
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

#pragma endregion 


#pragma region Signleton
namespace singletone {
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
#pragma endregion



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
#pragma endregion




template <class T, int ALLOC_BLOCK_SIZE = 50>
class CMemoryPool
{
public:
    static VOID* operator new(size_t allocLength) { // TODO: å������ std::size_t ���µ� �̷��� �ᵵ ���̰� ���ٰ� ���Ҵ�. Ȥ�� �𸣴� �ϴ� üũ.

        assert(sizeof(T) == allocLength); // allocLength �� sizeof(T) �� �ٸ��� ����.
        assert(sizeof(T) >= sizeof(UCHAR*)); // sizeof(T) �� 4����Ʈ ���� ������ ����. �޸� Ǯ�� �̻��� ũ��� �Ҵ��� �� ���̴�.

        // ���� �� �̻� �Ҵ��� �� �ִ� ������ ���� ��� ���� �޸� ���� �߰� �Ҵ�
        if (!mFreePointer)
            allockBlock();

        UCHAR* returnPointer = mFreePointer;

        // mFreePointer �� �� �ִ� ���� ���� �޸� ���� ���� �ּҸ� ����Ű�� �ִ�. �Ʒ��� ���� ������ �ϸ� ���� �޸� ���� ���� �ּҸ� ����Ű�� �ȴ�.
        // ���̳ʸ� ��(�ּ� ��)�� �����ϴ� retinerpret_cast �� �Ἥ ��� UCHAR* ���� �����ϸ鼭 ���� �Ҵ� ������ ���� �ּҸ� mFreePointer �� ����Ű���� �ϴ� ���̴�.
        mFreePointer = *reinterpret_cast<UCHAR**>(returnPointer);

        return returnPointer;
    }

    static VOID operator delete(VOID* deletePointer) { // ���α׷� ���� �ÿ��� �ڵ����� �޸𸮰� ��������, ��� ���� �ø� �޸𸮴� ���� ���� ���� ���� ���̶�..
        *reinterpret_cast<UCHAR**>(deletePointer) = mFreePointer; // ��ȯ�Ǵ� �޸� �� ���� ���� mFreePointer �� ���� �޸� ������ ����
        mFreePointer = static_cast<UCHAR*>(deletePointer); // mFreePointer �� ��ȯ�� �޸� ���� ����Ű���� ����, �̷��� �����ϸ� mFreePointer �� deletePointer �� �־ ���� ���� ó���� �ʿ䰡 ����!! 
    }

private:
    static VOID allockBlock() {
        mFreePointer = new UCHAR[sizeof(T) * ALLOC_BLOCK_SIZE]; // T �� Ÿ���� ũ�⸸ŭ 50���� �Ҵ�

        UCHAR** current = reinterpret_cast<UCHAR**>(mFreePointer); // �Ҵ��� �޸� ���� �����͸� current �� �Ҵ�
        UCHAR* next = mFreePointer; // ����� �Ҵ�� ù �޸� ���� �ּ�, �Ʒ� ���꿡�� ���� ���� �ּҸ� ����Ű�� �뵵�� ���ȴ�.

        for (INT i = 0; i < ALLOC_BLOCK_SIZE - 1; ++i) {
            next += sizeof(T); // ���� �� �ּҸ�ŭ �̵�.
            *current = next; // current �� ���� ���� �ּҸ� ����Ű���� �ش� ���� �Ҵ�.
            current = reinterpret_cast<UCHAR**>(next); // UCHAR* ������ �����ϸ鼭 �޸� ���� �� ù 4����Ʈ�� ���� ���� �ּ� ���� �Ҵ��ϱ� ���� UCHAR ** ������ ��ȯ.
        }

        *current = 0; // ������ ���� ���� ���� ���� �����Ƿ� ù 4����Ʈ�� NULL �� �Ҵ�.
    }

private:
    static UCHAR* mFreePointer;

protected:
    ~CMemoryPool() { // �ı��� ��� �����ϰ� ����

    }
};

template <class T, int ALLOCK_BLOCK_SIZE>
UCHAR* CMemoryPool<T, ALLOCK_BLOCK_SIZE>::mFreePointer; // Ŭ���� ���� ����� �̷� ������ �ʱ�ȭ�� ���Ѿ� �Ѵ�. ���� �Է����� ������ �ڵ����� 0�� �ȴ�. (BSS)

class CDataMP : public CMemoryPool<CDataMP> {
public:
    BYTE a[20];
    BYTE b[20];

};




#pragma region RvalueReference

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

#pragma endregion


#pragma region
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

#pragma endregion
int main()
{
  
    
    return 0;

}
