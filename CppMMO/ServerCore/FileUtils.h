#pragma once
#include <vector>
#include "Types.h"

// ----------------------------
//			FileUtils
// ----------------------------

class FileUtils
{
public:
	static myVector<BYTE>		ReadFile(const WCHAR* path);
	static myString				Convert(string str);
};