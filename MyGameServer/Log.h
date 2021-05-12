#pragma once
#include<Windows.h>
#include<fstream>
#include<tchar.h>
#include<pchannel.h>
#define MAX_BUFFER_LENGTH 4096  // TODO: �̰� å�� ������ ��� �ϴ� ����ũ�μ���Ʈ �������� �̷� ������ �����ϴ� �̷��� ����. ���� ���� �������鼭 �� �� Ȯ��

/// <summary>
/// �α� Ŭ�����̴�. ����, �ܼ� ȭ��, ����� â 3���� ��µȴ�.
/// </summary>
class CLog {
public:
	static BOOL WriteLog(LPTSTR data, ...) {
		SYSTEMTIME systemTime;
		TCHAR CurrentDate[32] = { 0, }; // 0�� ���ڿ� ���� ����(NULL)��� ���� �ȴ�.
		TCHAR CurrentFileName[MAX_PATH] = { 0, };
		FILE* FilePtr = NULL;
		TCHAR DebugLog[MAX_BUFFER_LENGTH] = { 0, };
	
		// �Ʒ� ������ variadic_list
		// variadic: Taking a arbitrarily many arguments. '������ ������ argument(��������, �Լ��� �޼����� �Է� ��) �� �޴�' 
		// ������ Parameter �� �Ű������̸� �Լ��� �޼����� �Է� ���� ���̴�.
		va_list ap; // �������� �Լ��� ���ȴ�. ap �� argument pointer �� �ǹ�.
		TCHAR Log[MAX_BUFFER_LENGTH] = { 0, };

		va_start(ap, data);
		_vstprintf(Log, data, ap);
		va_end(ap);

		GetLocalTime(&systemTime);
		_sntprintf(CurrentDate, 32, _T("%d-%d-%d %d:%d:%d"),
			systemTime.wYear,
			systemTime.wMonth,
			systemTime.wDay,
			systemTime.wHour,
			systemTime.wMinute,
			systemTime.wSecond);
		
		_sntprintf(CurrentFileName, MAX_PATH, _T("LOG_%d-%d-%d %d.log"),
			systemTime.wYear,
			systemTime.wMonth,
			systemTime.wDay,
			systemTime.wHour);

		FilePtr = _tfopen(CurrentFileName, _T("a"));
		if (!FilePtr)
			return FALSE;

		// ���Ͽ� ���
		_ftprintf(FilePtr, _T("[%s] %s\n"), CurrentDate, Log);

		_sntprintf(DebugLog, MAX_BUFFER_LENGTH, _T("[%s] %\n"), CurrentDate, Log);

		fflush(FilePtr);

		fclose(FilePtr);

		// ����� â�� ���
		OutputDebugString(DebugLog);

		// �ܼ� â�� ���
		_tprintf(_T("%s"), DebugLog);

		return TRUE;
	}
};