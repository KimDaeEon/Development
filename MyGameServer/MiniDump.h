#pragma once
#include<Windows.h>
#include<minidumpapiset.h>
#include<tchar.h>
class CMiniDump {
public:
	static BOOL Begin(VOID);
	static BOOL End(VOID);
};