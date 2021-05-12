#pragma once
#include<Windows.h>
#include<fstream>
#include<tchar.h>
#include<pchannel.h>
#define MAX_BUFFER_LENGTH 4096  // TODO: 이거 책에 설명이 없어서 일단 마이크로소프트 예제에서 이런 식으로 설정하니 이렇게 쓰자. 추후 서버 돌려보면서 이 값 확인

/// <summary>
/// 로그 클래스이다. 파일, 콘솔 화면, 디버그 창 3곳에 출력된다.
/// </summary>
class CLog {
public:
	static BOOL WriteLog(LPTSTR data, ...) {
		SYSTEMTIME systemTime;
		TCHAR CurrentDate[32] = { 0, }; // 0은 문자열 종료 문자(NULL)라고 보면 된다.
		TCHAR CurrentFileName[MAX_PATH] = { 0, };
		FILE* FilePtr = NULL;
		TCHAR DebugLog[MAX_BUFFER_LENGTH] = { 0, };
	
		// 아래 변수는 variadic_list
		// variadic: Taking a arbitrarily many arguments. '임의의 갯수의 argument(전달인자, 함수와 메서드의 입력 값) 를 받는' 
		// 덤으로 Parameter 는 매개변수이며 함수와 메서드의 입력 변수 명이다.
		va_list ap; // 가변인자 함수에 사용된다. ap 는 argument pointer 를 의미.
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

		// 파일에 출력
		_ftprintf(FilePtr, _T("[%s] %s\n"), CurrentDate, Log);

		_sntprintf(DebugLog, MAX_BUFFER_LENGTH, _T("[%s] %\n"), CurrentDate, Log);

		fflush(FilePtr);

		fclose(FilePtr);

		// 디버그 창에 출력
		OutputDebugString(DebugLog);

		// 콘솔 창에 출력
		_tprintf(_T("%s"), DebugLog);

		return TRUE;
	}
};