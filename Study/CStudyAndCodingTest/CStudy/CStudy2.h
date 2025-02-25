#pragma once
#include <iostream>
#include <utility>

namespace RVOTest
{
	using namespace std;

	class Example
	{
	public:
		int a = 10;
		Example(int a = 10) : a(a) { std::cout << "기본 생성자 호출\n"; }
		Example(const Example& other) : a(other.a) { std::cout << "복사 생성자 호출\n"; }
		Example(const Example&& other) noexcept : a(other.a) { std::cout << "복사 생성자 호출\n"; }
	};

	Example CreateExample()
	{
		auto b = Example(); // RVO 적용 가능
		auto c = b; // 복사 생성자 호출, CreateExample()의 코드가 그대로 옮겨지는지 확인하기 위함
		c.a = 30;
		return c;
	}

	void Test()
	{
		// 실행해보면 CreateExample 내용이 그대로 실행되고, obj에 할당하기 위해 호출해야 할 복사 생성자는 안불리는 것을 알 수 있음
		Example obj = CreateExample(); 

		cout << obj.a << endl;
	}


}
