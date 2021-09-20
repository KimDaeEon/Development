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
            cout << "test 생성됨" << endl;
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
    static VOID* operator new(size_t allocLength) { // TODO: 책에서는 std::size_t 였는데 이렇게 써도 차이가 없다고 보았다. 혹시 모르니 일단 체크.

        assert(sizeof(T) == allocLength); // allocLength 와 sizeof(T) 가 다르면 중지.
        assert(sizeof(T) >= sizeof(UCHAR*)); // sizeof(T) 가 4바이트 보다 작으면 중지. 메모리 풀이 이상한 크기로 할당이 된 것이다.

        // 만약 더 이상 할당할 수 있는 공간이 없을 경우 새로 메모리 블럭을 추가 할당
        if (!mFreePointer)
            allockBlock();

        UCHAR* returnPointer = mFreePointer;

        // mFreePointer 에 들어가 있는 값은 다음 메모리 블럭의 시작 주소를 가리키고 있다. 아래와 같이 연산을 하면 다음 메모리 블럭의 시작 주소를 가리키게 된다.
        // 바이너리 값(주소 값)을 유지하는 retinerpret_cast 를 써서 계속 UCHAR* 형을 유지하면서 다음 할당 가능한 블럭의 주소를 mFreePointer 가 가리키도록 하는 것이다.
        mFreePointer = *reinterpret_cast<UCHAR**>(returnPointer);

        return returnPointer;
    }

    static VOID operator delete(VOID* deletePointer) { // 프로그램 종료 시에는 자동으로 메모리가 내려가고, 사실 여기 올린 메모리는 내릴 일이 거의 없는 것이라서..
        *reinterpret_cast<UCHAR**>(deletePointer) = mFreePointer; // 반환되는 메모리 블럭 기준 현재 mFreePointer 를 다음 메모리 블럭으로 설정
        mFreePointer = static_cast<UCHAR*>(deletePointer); // mFreePointer 가 반환된 메모리 블럭을 가리키도록 설정, 이렇게 구현하면 mFreePointer 를 deletePointer 에 넣어도 딱히 예외 처리할 필요가 없다!! 
    }

private:
    static VOID allockBlock() {
        mFreePointer = new UCHAR[sizeof(T) * ALLOC_BLOCK_SIZE]; // T 형 타입의 크기만큼 50개씩 할당

        UCHAR** current = reinterpret_cast<UCHAR**>(mFreePointer); // 할당한 메모리 블럭의 포인터를 current 에 할당
        UCHAR* next = mFreePointer; // 현재는 할당된 첫 메모리 블럭의 주소, 아래 연산에서 다음 블럭의 주소를 가리키는 용도로 사용된다.

        for (INT i = 0; i < ALLOC_BLOCK_SIZE - 1; ++i) {
            next += sizeof(T); // 다음 블럭 주소만큼 이동.
            *current = next; // current 가 다음 블럭의 주소를 가리키도록 해당 값을 할당.
            current = reinterpret_cast<UCHAR**>(next); // UCHAR* 형식을 유지하면서 메모리 블럭의 맨 첫 4바이트에 다음 블럭의 주소 값을 할당하기 위해 UCHAR ** 형으로 변환.
        }

        *current = 0; // 마지막 블럭의 경우는 다음 블럭이 없으므로 첫 4바이트를 NULL 로 할당.
    }

private:
    static UCHAR* mFreePointer;

protected:
    ~CMemoryPool() { // 파괴자 상속 가능하게 구성

    }
};

template <class T, int ALLOCK_BLOCK_SIZE>
UCHAR* CMemoryPool<T, ALLOCK_BLOCK_SIZE>::mFreePointer; // 클래스 정적 멤버는 이런 식으로 초기화를 시켜야 한다. 값을 입력하지 않으면 자동으로 0이 된다. (BSS)

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
T* Factory(U&& arg) { // 다른 클래스를 생성해서 리턴해주는 함수
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

#pragma endregion
int main()
{
  
    
    return 0;

}
