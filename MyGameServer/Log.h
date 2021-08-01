#pragma once

/// <summary>
/// 로그 클래스이다. 파일, 콘솔 화면, 디버그 창 3곳에 출력된다.
/// </summary>
class CLog 
{
public:
	static BOOL WriteLog(LPCTSTR data, ...) { // TODO: 책에서는 LPTSTR 로 적혀 있어서 그렇게 적었었는데, 사용해보니 LPCTSTR 이 맞고 오타를 치신 것 같다. 이 변수를 바꿀 일도 없고.. 혹시 모르니 추후에 확인.
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
		// vstprintf: formatted printing with va_list to a string (with TCHAR), TCHAR 형을 담는 va_list 를 문자열에 프린트하는 것.
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

	static BOOL	WriteLogNoDate(LPTSTR data, ...)
	{
		SYSTEMTIME	SystemTime;
		TCHAR		CurrentDate[32] = { 0, };
		TCHAR		CurrentFileName[MAX_PATH] = { 0, };
		FILE* FilePtr = NULL;
		TCHAR		DebugLog[MAX_BUFFER_LENGTH] = { 0, };

		va_list		ap;
		TCHAR		Log[MAX_BUFFER_LENGTH] = { 0, };

		va_start(ap, data);
		_vstprintf(Log, data, ap);
		va_end(ap);

		GetLocalTime(&SystemTime);
		_sntprintf(CurrentDate, 32, _T("%d-%d-%d %d:%d:%d"),
			SystemTime.wYear,
			SystemTime.wMonth,
			SystemTime.wDay,
			SystemTime.wHour,
			SystemTime.wMinute,
			SystemTime.wSecond);

		_sntprintf(CurrentFileName, MAX_PATH, _T("LOG_%d-%d-%d %d.log"),
			SystemTime.wYear,
			SystemTime.wMonth,
			SystemTime.wDay,
			SystemTime.wHour);

		FilePtr = _tfopen(CurrentFileName, _T("a"));
		if (!FilePtr)
			return FALSE;

		_ftprintf(FilePtr, _T("%s"), Log);
		_sntprintf(DebugLog, MAX_BUFFER_LENGTH, _T("%s"), Log);

		fflush(FilePtr);

		fclose(FilePtr);

		OutputDebugString(DebugLog);
		_tprintf(_T("%s"), DebugLog);

		return TRUE;
	}
};