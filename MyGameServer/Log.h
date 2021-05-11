#pragma once
#include<Windows.h>
#include<fstream>
#include<tchar.h>
#include<pchannel.h>
#define MAX_BUFFER_LENGTH 4096  // TODO: �̰� å�� ������ ��� �ϴ� ����ũ�μ���Ʈ �������� �̷� ������ �����ϴ� �ϴ� �̷��� ����. ���� Ȯ��.

/// <summary>
/// �α� Ŭ�����̴�. ����, �ܼ� ȭ��, ����� â 3���� ��µȴ�.
/// </summary>
class CLog {
public:
	static BOOL WriteLog(LPTSTR data, ...) { // TDOO: �� �κ� ���� �������� �Լ�, _vstprintf �� ���� ����� �����鼭 �� ����.
		SYSTEMTIME systemTime;
		TCHAR CurrentDate[32] = { 0, };
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

		FilePtr = _tfopen(CurrentDate, _T("a")); // TODO: �̰� _sntprintf �� ����� ������� ���� �� �ִٴµ�.. ã�ƺ���. �׸��� "a" �̰͸� �ִ°� �´����� Ȯ��
		if (!FilePtr)
			return FALSE;

		// ���Ͽ� ���
		_ftprintf(FilePtr, _T("[%s] %S\n"), CurrentDate, Log);

		// �ܼ� ȭ�鿡 ��� TODO: �´��� Ȯ�� �ʿ�
		_sntprintf(DebugLog, MAX_BUFFER_LENGTH, _T("[%s] %S\n"), CurrentDate, Log);

		fflush(FilePtr);

		fclose(FilePtr);

		// ����� â�� ��� TODO: �´��� Ȯ�� �ʿ�
		OutputDebugString(DebugLog);
		_tprintf(_T("%s"), DebugLog);

		return TRUE;
	}
};