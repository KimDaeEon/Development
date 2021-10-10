#include "pch.h"
#include "DLL_TEST.h"
#include <iostream>


using namespace std;

void test() {
	cout << "C++ test 하이영" << endl;
}

int test2(int a, int b) {
	cout << "C++ test2 하이영" << endl;
	return a + b;
}

myclass::myclass(int _a = 10, int _b = 20) {
	a = _a;
	b = _b;
}

void myclass::f() {
	cout << a << endl;
	cout << b << endl;
}