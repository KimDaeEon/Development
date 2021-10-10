#pragma warning(disable:4996)

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include "DLL_TEST.h"
using namespace std;

int main() {
	myclass a(20,30);
	
	while (true) {
		test();
		a.f();
		char key = getch();
		if (key == 'q') {
			break;
		}
	}

	return 0;
}