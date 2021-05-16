#pragma once
#include<Windows.h>

class CCrypt {
public:
	static BOOL Encrypt(BYTE* source, BYTE* destination, DWORD length);
	static BOOL Decrypt(BYTE* source, BYTE* destination, DWORD length);
};