#pragma warning(disable:4996)

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include "DLL_TEST.h"
#include <vector>
using namespace std;

class const_test {
public:
	int fu() const {
		cout << "const object" << endl;
		return 1;
	}
	int fu() {
		cout << "normal object" << endl;

		return 2;
	}
};

void tfunc(const_test& a) {
	a.fu();
}
int main() {
	const_test a;
	const const_test b;
	
	tfunc(a);
	return 0;
}