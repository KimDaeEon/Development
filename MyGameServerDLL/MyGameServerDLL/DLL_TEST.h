#pragma once

#define MYTESTLIBRARY_EXPORTS
 
#ifdef MYTESTLIBRARY_EXPORTS
#define MYTESTLIBRARY_API __declspec(dllexport)
#else
#define MYTESTLIBRARY_API __declspec(dllimport)
#endif


extern "C" {
	MYTESTLIBRARY_API void test();

	MYTESTLIBRARY_API int test2(int a, int b);


	class MYTESTLIBRARY_API myclass {
	public:
		myclass(int a, int b);
		void f();

	private:
		int a;
		int b;
	};

	MYTESTLIBRARY_API myclass* createMyClass(int a, int b) {
		myclass* new_instance = new myclass(a, b);
		return new_instance;
	}

	MYTESTLIBRARY_API bool deleteMyClass(myclass* deleted) {
		delete deleted;
		return true;
	}



}
