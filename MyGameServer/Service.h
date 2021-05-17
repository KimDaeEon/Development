#pragma once
#include<Windows.h>
#include<tchar.h>
#include <stdio.h>

class CService {
public:
	CService(VOID);
	virtual ~CService(VOID); // OnStopped 와 짝을 이루기 위해서 virtual 로 선언

	BOOL Install(LPCTSTR serviceName);
	BOOL Uninstall(LPCTSTR serviceName);

	BOOL Begin(LPTSTR serviceName);
	BOOL End(VOID);
	
	VOID RunCallback(DWORD argumentCount, LPTSTR* argument);
	VOID CtrlHandlerCallback(DWORD opCode);

	virtual VOID OnStarted(VOID) = 0;
	virtual VOID OnStopped(VOID) = 0;

private:
	
	SERVICE_STATUS mServiceStatus;
	SERVICE_STATUS_HANDLE mServiceStatusHandle;

	TCHAR mServiceName[MAX_PATH];
};

