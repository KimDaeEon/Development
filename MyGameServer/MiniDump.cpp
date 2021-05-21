#include"MiniDump.h"

typedef BOOL(WINAPI* MINIDUMPWRITEDUMP)( // Callback 함수 원형, 매번 이 형태로 함수 포인터 선언하기 귀찮으니 typedef 으로 이렇게 정의한 것이다.
	HANDLE hProcess,
	DWORD dwPid,
	HANDLE hFile,
	MINIDUMP_TYPE DumpType,
	CONST PMINIDUMP_EXCEPTION_INFORMATION ExceptionParam, // 미니 덤프를 일으킨 client 의 exception 정보를 가리킨다. NULL 이면 exception 정보가 미니 덤프 파일에 담기지 않는다.
	CONST PMINIDUMP_USER_STREAM_INFORMATION UserStreamPrarm,  // 사용자 Data 가 담긴 스트림 리스트를 담은 것, NULL 이면 사용자 정의 정보가 미니 덤프에 포함되지 않는다.
	CONST PMINIDUMP_CALLBACK_INFORMATION CallbackParam // 미니 덤프 정보를 받아 처리하는 콜백함수를 가리키는 포인터, NULL 이면 callback 실행되지 않는다. 
	);

LPTOP_LEVEL_EXCEPTION_FILTER PreviousExceptionFilter = NULL;

// UnhandledExceptionFilter 로 하면 함수 이름이 충돌이 난다. 주의.
LONG WINAPI UnHandledExceptionFilter(struct _EXCEPTION_POINTERS* exceptionInfo) {  // WINAPI = __stdcall 호출 규약이다. 호출된 함수가 스택을 정리한다. 기본은 __cdecl (C declaration, C 에서 기존에 쓰던 호출 규약, 호출한 함수가 스택 정리)
	HMODULE DllHandle = NULL;

	// Windows 2000 이전에는 따로 DBGHELp를 배포해서 설정해 주어야 한다.
	DllHandle = LoadLibrary(_T("DBGHELP.DLL"));

	if (DllHandle) {
		// 덤프를 받아 파일로 만드는 과정
		// DBGHELP.DLL 에서 MiniDumpWriteDump 함수 주소(포인터)를 불러와서 Dump 라고 정의, 이를 통해 덤프 파일을 만든다.
		MINIDUMPWRITEDUMP Dump = (MINIDUMPWRITEDUMP)GetProcAddress(DllHandle, "MiniDumpWriteDump"); // 요게 함수 주소를 가져오는 것이다. Proc 이 Process 가 아니라 Procedure 이다.
		
		if (Dump) {
			TCHAR DumpPath[MAX_PATH] = { 0, };
			SYSTEMTIME SystemTime;

			GetLocalTime(&SystemTime);

			_sntprintf(DumpPath, MAX_PATH, _T("%d-%d-%d %d_%d_%d.dmp"),
				SystemTime.wYear, // w 는 WORD 를 의미. 이 자료형이 WORD 이다.
				SystemTime.wMonth,
				SystemTime.wDay,
				SystemTime.wHour,
				SystemTime.wMinute,
				SystemTime.wSecond);

			HANDLE FileHandle = CreateFile(
				DumpPath,
				GENERIC_WRITE, // dwDesiredAccess, write 하기로 설정, dwShareMode 와 모순된 것은 설정 불가.
				FILE_SHARE_WRITE, // dwShareMode, 다른 프로세스도 이 파일을 열 수 있도록 한다.
				NULL, // lpSecurityAttributes, 보안 관련 특성
				CREATE_ALWAYS, // dwCreationDisposition, 파일 생성 시 처리 방식, 항상 파일 새로 생성하며 기존 이름 있고 덮어쓸 수 있으면 덮어쓴다.
				FILE_ATTRIBUTE_NORMAL, // dwFlagsAndAttributes, 파일 특성
				NULL // hTemplateFile, 파일 특성에 보조적 정보를 주는 템플릿 파일 핸들 정보를 입력한다고 한다. 기존에 존재하는 파일을 열려고 하면 이 패러미터는 무시.
			); 

			if (FileHandle != INVALID_HANDLE_VALUE) {
				_MINIDUMP_EXCEPTION_INFORMATION MiniDumpExceptionInfo;

				MiniDumpExceptionInfo.ThreadId			= GetCurrentThreadId();  // DWORD, 현재 exception 을 throw 하는 스레드의 ID 를 받아오는 함수
				MiniDumpExceptionInfo.ExceptionPointers = exceptionInfo; // PEXCEPTION_POINTERS, 컴퓨터에 독립적인 exception 정보를 저장하는 포인터라고 한다. (컴퓨터 종류에 상관없이 처리되는 형식인 것 같다.)
				MiniDumpExceptionInfo.ClientPointers	= NULL; // TODO: BOOPL, ExceptionPointers 가 가리키는 메모리가 디버그되는 쪽이라면 True, 디버그 프로세서 쪽이라면 False 라고 한다.. 잘 이해가 안된다. 추후 다시 확인.
				
				BOOL Success = Dump( // MiniDumpWriteDump 를 맨 위에 작성한 포인터로 받아와서 요렇게 바꾼 것이다.
					GetCurrentProcess(), // 현재 프로세스에 대한 pseudo handle 을 리턴, 앞의 GetProcAddress 랑 아예 다른 것이니 헷갈리지 말자.
					GetCurrentProcessId(), // calling process 에 대한 id 리턴
					FileHandle,
					MiniDumpNormal,
					&MiniDumpExceptionInfo, // 위에서 입력했던 미니 덤프 exception 정보들 입력
					NULL, // 사용자 정의 정보 없음.
					NULL // callback 없음.
				);

				if (Success) {
					CloseHandle(FileHandle);

					return EXCEPTION_EXECUTE_HANDLER; // 예외 발생 시 __except 블럭의 코드가 실행되라는 뜻이다.
					// EXCEPTION_CONTINUE_EXCEPTION 은 예외 발생 시 __except 실행하지 않고 예외 발생 지점에서 재실행
					// EXCEPTION_CONTINUE_SEARCH 은 상위 __except 문을 실행, 하위 __except 문의 코드는 실행되지 않는다.
				}
			} // if FileHandle != INVALID_HANDLE_VALUE

			CloseHandle(FileHandle);
		} // if Dump
	} // if DllHandle

	return EXCEPTION_CONTINUE_SEARCH;  // 위의 설명과 같이 상위 __except 문의 코드를 실행하라는 뜻이다.
}

BOOL CMiniDump::Begin(VOID) {
	
	// TOOD: 에러 모드 1, 2, 4, 8 넣어가면서 해봤는데 차이를 잘 모르겠다.. 추후에 다시 확인
	SetErrorMode(SEM_FAILCRITICALERRORS); // 시스템이 특정 심각한 에러를 핸들할 지, 프로세스가 에러를 핸들할 지 모드를 설정
	// SEM_FAILCRITICALERRORS 는 시스템이 심각한 에러(Abort, Retry, Fail)가 있을 때에 메세지 박스 안 띄우고, 에러를 호출한 프로세스에게 보낸다.
	// 찾아보니 심각한 에러에 대한 기준이 애매한 듯 하다. 또한 SEM 은 Set Error Mode 를 의미하는 것 같다.
	// https://stackoverflow.com/questions/17853625/what-does-sem-failcriticalerrors-prevent 
	// https://docs.microsoft.com/en-us/windows/win32/api/errhandlingapi/nf-errhandlingapi-seterrormode SetErrorMode 함수

	PreviousExceptionFilter = SetUnhandledExceptionFilter(UnHandledExceptionFilter); 
	// 스레드의 top-level exception handler(최상위 수준 예외 처리기, 가장 먼저 적용되는 예외 처리기라 보면 된다.) 를 내가 정의한 UnhandledExceptionFilter 로 쓰여 있다.
	// 기존에 Exception 에 대해서 처리하던 함수가 있는데, 그것을 내가 정의한 함수로 바꾸도록 세팅해주는 것이라고 보면 된다.
	// MSDN 에 디버깅 시에는 이거 호출 안된다고 써 있어서 해봤는데, 호출이 안된다. 생각을 해보면 당연한데, 어차피 디버깅 시에는 어떤 문제가 있는지 알기 때문에 굳이 이것을 작동시키지 않는 것 같다.

	return true;
}

BOOL CMiniDump::End(VOID) {
	SetUnhandledExceptionFilter(PreviousExceptionFilter);

	return true;
}