#include <iostream>

class Base
{
public:
    Base()
    {
        std::cout << "Base 생성자 실행" << std::endl;
        virtualFunction(); // Base의 virtualFunction()이 호출됨
    }

    virtual void virtualFunction()
    {
        std::cout << "Base::virtualFunction" << std::endl;
    }
};

class Derived : public Base
{
public:
    Derived()
    {
        std::cout << "Derived 생성자 실행" << std::endl;
        virtualFunction();
    }

    void virtualFunction() override
    {
        std::cout << "Derived::virtualFunction" << std::endl;
    }
};

int main()
{
    Derived d;
    return 0;
}
