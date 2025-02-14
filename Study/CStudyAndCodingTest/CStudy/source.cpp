#include <iostream>

class Base
{
public:
    Base()
    {
        std::cout << "Base ������ ����" << std::endl;
        virtualFunction(); // Base�� virtualFunction()�� ȣ���
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
        std::cout << "Derived ������ ����" << std::endl;
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
