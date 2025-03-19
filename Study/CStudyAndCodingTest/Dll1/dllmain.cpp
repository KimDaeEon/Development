#include <windows.h>
#include <iostream>

extern "C" __declspec(dllexport) void HelloFunction()
{
    std::cout << "Hello from DLL!" << std::endl;
}

int main()
{
    HelloFunction(); // 정적으로 링크된 함수 호출
    return 0;
}
