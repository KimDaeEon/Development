#pragma once
#include <iostream>
#include <utility>
#include <WerApi.h>
#include <windows.h>
#include <iostream>

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

namespace VTable
{
	class Base
	{
	public:
		virtual void Func1() { std::cout << "Base::Func1" << std::endl; }
		virtual void Func2(int a = 10) { std::cout << "Base::Func2" << std::endl; }
	};

	class DerivedOverride : public Base
	{
	public:
		void Func1() override { std::cout << "DerivedOverride::Func1" << std::endl; }
		void Func2(int a = 10) override { std::cout << "DerivedOverride::Func2" << std::endl; }
	};

	class DerivedOnlyOneOverride : public Base
	{
		void Func1() override { std::cout << "DerivedOverride::Func1" << std::endl; }
	};

	void Func1Func2(Base* obj, const char* name)
	{
		std::cout << "class name = " << name << std::endl;

		// obj->Func1(Func2) 의 주소는 동적으로 정해지는 것이라서 컴파일러에서 이를 확인하는 것을 허용하지 않는다.
		// 여기에 디버그를 찍고 obj 내부의 __vfptr의 주소를 보면 override 된 경우 자식 클래스의 가상 함수 주소를,
		// override 되지 않은 경우 부모 클래스의 가상 함수 주소를 가지고 있는 것을 확인할 수 있다.
		obj->Func1();
		obj->Func2();

		std::cout << endl;
	}

	static void Test()
	{
		Base base;
		DerivedOverride derivedOverride;
		DerivedOnlyOneOverride derivedOnlyOneOverride;

		Func1Func2(&base, "Base");
		Func1Func2(&derivedOverride, "DerivedOverride");
		Func1Func2(&derivedOnlyOneOverride, "DerivedOnlyOneOverride");
	}

}


namespace WindowsViaCpp
{
	namespace SEH_VEH_Test
	{
		// 예외 처리 순서
		// VEH -> SEH -> SetUnhandledExecptionFilter 로 등록한 함수
		// 위 처리 과정 중에 EXCEPTION_COTNIUE_EXECUTION이 되면 VCH 실행
		int* ptr = nullptr;

		// Vectored Exception Handler (VEH)
		LONG WINAPI VectoredExceptionHandler1(EXCEPTION_POINTERS* pExceptionInfo)
		{
			std::cout << "[VectoredExceptionHandler1] 예외 발생! 코드: " << std::hex << pExceptionInfo->ExceptionRecord->ExceptionCode << std::endl;
			return EXCEPTION_CONTINUE_SEARCH; // 다음 핸들러로 전달
		}

		LONG WINAPI VectoredExceptionHandler2(EXCEPTION_POINTERS* pExceptionInfo)
		{
			std::cout << "[VectoredExceptionHandler2] 예외 발생! 코드: " << std::hex << pExceptionInfo->ExceptionRecord->ExceptionCode << std::endl;
			return EXCEPTION_CONTINUE_SEARCH; // 다음 핸들러로 전달
		}

		// Vectored Continue Handler (VCH)
		// VEH나 SEH 에서 EXCEPTION_CONTINUE_EXECUTION을 리턴하면 이 함수가 호출된다.
		// 주로 로그 작성을 위해서 사용된다고 한다.
		LONG WINAPI VectoredContinueHandler1(EXCEPTION_POINTERS* pExceptionInfo)
		{
			std::cout << "[VectoredContinueHandler1] 예외 발생! 코드: " << std::hex << pExceptionInfo->ExceptionRecord->ExceptionCode << std::endl;
			return EXCEPTION_CONTINUE_SEARCH; // 다음 핸들러로 전달
		}

		LONG WINAPI VectoredContinueHandler2(EXCEPTION_POINTERS* pExceptionInfo)
		{
			std::cout << "[VectoredContinueHandler2] 예외 발생! 코드: " << std::hex << pExceptionInfo->ExceptionRecord->ExceptionCode << std::endl;
			return EXCEPTION_CONTINUE_SEARCH; // 다음 핸들러로 전달
		}

		// SEH 필터 함수
		LONG WINAPI SehFilter(EXCEPTION_POINTERS* pExceptionInfo)
		{
			std::cout << "[SEH Filter] 예외 처리 시작" << std::endl;
			return EXCEPTION_CONTINUE_SEARCH;
		}

		// Unhandled Exception Filter (UEH)
		LONG WINAPI UnhandledExceptionHandler(EXCEPTION_POINTERS* pExceptionInfo)
		{
			std::cout << "[UnhandledExceptionHandler] 마지막 예외 처리! 코드: " << std::hex << pExceptionInfo->ExceptionRecord->ExceptionCode << std::endl;
			ptr = (int*)malloc(sizeof(int));
			return EXCEPTION_EXECUTE_HANDLER;
		}

		void Test()
		{
			// 벡터 예외 핸들러 등록 (가장 먼저 실행됨)
			PVOID veh1 = AddVectoredExceptionHandler(0, VectoredExceptionHandler1);
			PVOID veh2 = AddVectoredExceptionHandler(0, VectoredExceptionHandler2);

			// 벡터 계속 핸들러 등록 (VEH 이후 실행됨)
			PVOID vch1 = AddVectoredContinueHandler(0, VectoredContinueHandler1);
			PVOID vch2 = AddVectoredContinueHandler(0, VectoredContinueHandler2);

			// SetUnhandledExceptionFilter 등록 (SEH가 처리하지 못한 예외를 처리)
			SetUnhandledExceptionFilter(UnhandledExceptionHandler);

			std::cout << "모든 핸들러 등록 완료, 예외를 강제로 발생시킵니다." << std::endl;

			__try
			{
				*ptr = 42;  // 강제로 접근 예외 발생
			}
			__except (SehFilter(GetExceptionInformation()))
			{
				std::cout << "[SEH Handler] 예외가 SEH에서 처리됨." << std::endl;
			}

			// 핸들러 제거
			RemoveVectoredExceptionHandler(veh1);
			RemoveVectoredExceptionHandler(veh2);
			RemoveVectoredExceptionHandler(vch1);
			RemoveVectoredExceptionHandler(vch2);
			delete ptr;

			std::cout << "핸들러 제거 완료." << std::endl;
		}
	}

	namespace EXPLICIT_DYNAMIC_LINKING
	{
		typedef void (*HelloFunctionPtr)(); // 함수 포인터 선언

		int Test()
		{
			HMODULE hModule = LoadLibrary(L"Dll1.dll"); // DLL 로드
			if (!hModule)
			{
				std::cerr << "Failed to load DLL!" << std::endl;
				return 1;
			}

			// 아래처럼 윈도우는 심벌이름으로 문자열을 사용하는 경향이 있으므로 알아두면 좋음
			HelloFunctionPtr HelloFunction = (HelloFunctionPtr)GetProcAddress(hModule, "HelloFunction");
			if (!HelloFunction)
			{
				std::cerr << "Failed to get function address!" << std::endl;
				FreeLibrary(hModule);
				return 1;
			}

			HelloFunction(); // 동적 호출

			FreeLibrary(hModule); // DLL 해제
			
			return 1;
		}

	}
}
