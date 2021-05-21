#include"MiniDump.h"

typedef BOOL(WINAPI* MINIDUMPWRITEDUMP)( // Callback �Լ� ����, �Ź� �� ���·� �Լ� ������ �����ϱ� �������� typedef ���� �̷��� ������ ���̴�.
	HANDLE hProcess,
	DWORD dwPid,
	HANDLE hFile,
	MINIDUMP_TYPE DumpType,
	CONST PMINIDUMP_EXCEPTION_INFORMATION ExceptionParam, // �̴� ������ ����Ų client �� exception ������ ����Ų��. NULL �̸� exception ������ �̴� ���� ���Ͽ� ����� �ʴ´�.
	CONST PMINIDUMP_USER_STREAM_INFORMATION UserStreamPrarm,  // ����� Data �� ��� ��Ʈ�� ����Ʈ�� ���� ��, NULL �̸� ����� ���� ������ �̴� ������ ���Ե��� �ʴ´�.
	CONST PMINIDUMP_CALLBACK_INFORMATION CallbackParam // �̴� ���� ������ �޾� ó���ϴ� �ݹ��Լ��� ����Ű�� ������, NULL �̸� callback ������� �ʴ´�. 
	);

LPTOP_LEVEL_EXCEPTION_FILTER PreviousExceptionFilter = NULL;

// UnhandledExceptionFilter �� �ϸ� �Լ� �̸��� �浹�� ����. ����.
LONG WINAPI UnHandledExceptionFilter(struct _EXCEPTION_POINTERS* exceptionInfo) {  // WINAPI = __stdcall ȣ�� �Ծ��̴�. ȣ��� �Լ��� ������ �����Ѵ�. �⺻�� __cdecl (C declaration, C ���� ������ ���� ȣ�� �Ծ�, ȣ���� �Լ��� ���� ����)
	HMODULE DllHandle = NULL;

	// Windows 2000 �������� ���� DBGHELp�� �����ؼ� ������ �־�� �Ѵ�.
	DllHandle = LoadLibrary(_T("DBGHELP.DLL"));

	if (DllHandle) {
		// ������ �޾� ���Ϸ� ����� ����
		// DBGHELP.DLL ���� MiniDumpWriteDump �Լ� �ּ�(������)�� �ҷ��ͼ� Dump ��� ����, �̸� ���� ���� ������ �����.
		MINIDUMPWRITEDUMP Dump = (MINIDUMPWRITEDUMP)GetProcAddress(DllHandle, "MiniDumpWriteDump"); // ��� �Լ� �ּҸ� �������� ���̴�. Proc �� Process �� �ƴ϶� Procedure �̴�.
		
		if (Dump) {
			TCHAR DumpPath[MAX_PATH] = { 0, };
			SYSTEMTIME SystemTime;

			GetLocalTime(&SystemTime);

			_sntprintf(DumpPath, MAX_PATH, _T("%d-%d-%d %d_%d_%d.dmp"),
				SystemTime.wYear, // w �� WORD �� �ǹ�. �� �ڷ����� WORD �̴�.
				SystemTime.wMonth,
				SystemTime.wDay,
				SystemTime.wHour,
				SystemTime.wMinute,
				SystemTime.wSecond);

			HANDLE FileHandle = CreateFile(
				DumpPath,
				GENERIC_WRITE, // dwDesiredAccess, write �ϱ�� ����, dwShareMode �� ����� ���� ���� �Ұ�.
				FILE_SHARE_WRITE, // dwShareMode, �ٸ� ���μ����� �� ������ �� �� �ֵ��� �Ѵ�.
				NULL, // lpSecurityAttributes, ���� ���� Ư��
				CREATE_ALWAYS, // dwCreationDisposition, ���� ���� �� ó�� ���, �׻� ���� ���� �����ϸ� ���� �̸� �ְ� ��� �� ������ �����.
				FILE_ATTRIBUTE_NORMAL, // dwFlagsAndAttributes, ���� Ư��
				NULL // hTemplateFile, ���� Ư���� ������ ������ �ִ� ���ø� ���� �ڵ� ������ �Է��Ѵٰ� �Ѵ�. ������ �����ϴ� ������ ������ �ϸ� �� �з����ʹ� ����.
			); 

			if (FileHandle != INVALID_HANDLE_VALUE) {
				_MINIDUMP_EXCEPTION_INFORMATION MiniDumpExceptionInfo;

				MiniDumpExceptionInfo.ThreadId			= GetCurrentThreadId();  // DWORD, ���� exception �� throw �ϴ� �������� ID �� �޾ƿ��� �Լ�
				MiniDumpExceptionInfo.ExceptionPointers = exceptionInfo; // PEXCEPTION_POINTERS, ��ǻ�Ϳ� �������� exception ������ �����ϴ� �����Ͷ�� �Ѵ�. (��ǻ�� ������ ������� ó���Ǵ� ������ �� ����.)
				MiniDumpExceptionInfo.ClientPointers	= NULL; // TODO: BOOPL, ExceptionPointers �� ����Ű�� �޸𸮰� ����׵Ǵ� ���̶�� True, ����� ���μ��� ���̶�� False ��� �Ѵ�.. �� ���ذ� �ȵȴ�. ���� �ٽ� Ȯ��.
				
				BOOL Success = Dump( // MiniDumpWriteDump �� �� ���� �ۼ��� �����ͷ� �޾ƿͼ� �䷸�� �ٲ� ���̴�.
					GetCurrentProcess(), // ���� ���μ����� ���� pseudo handle �� ����, ���� GetProcAddress �� �ƿ� �ٸ� ���̴� �򰥸��� ����.
					GetCurrentProcessId(), // calling process �� ���� id ����
					FileHandle,
					MiniDumpNormal,
					&MiniDumpExceptionInfo, // ������ �Է��ߴ� �̴� ���� exception ������ �Է�
					NULL, // ����� ���� ���� ����.
					NULL // callback ����.
				);

				if (Success) {
					CloseHandle(FileHandle);

					return EXCEPTION_EXECUTE_HANDLER; // ���� �߻� �� __except ���� �ڵ尡 ����Ƕ�� ���̴�.
					// EXCEPTION_CONTINUE_EXCEPTION �� ���� �߻� �� __except �������� �ʰ� ���� �߻� �������� �����
					// EXCEPTION_CONTINUE_SEARCH �� ���� __except ���� ����, ���� __except ���� �ڵ�� ������� �ʴ´�.
				}
			} // if FileHandle != INVALID_HANDLE_VALUE

			CloseHandle(FileHandle);
		} // if Dump
	} // if DllHandle

	return EXCEPTION_CONTINUE_SEARCH;  // ���� ����� ���� ���� __except ���� �ڵ带 �����϶�� ���̴�.
}

BOOL CMiniDump::Begin(VOID) {
	
	// TOOD: ���� ��� 1, 2, 4, 8 �־�鼭 �غôµ� ���̸� �� �𸣰ڴ�.. ���Ŀ� �ٽ� Ȯ��
	SetErrorMode(SEM_FAILCRITICALERRORS); // �ý����� Ư�� �ɰ��� ������ �ڵ��� ��, ���μ����� ������ �ڵ��� �� ��带 ����
	// SEM_FAILCRITICALERRORS �� �ý����� �ɰ��� ����(Abort, Retry, Fail)�� ���� ���� �޼��� �ڽ� �� ����, ������ ȣ���� ���μ������� ������.
	// ã�ƺ��� �ɰ��� ������ ���� ������ �ָ��� �� �ϴ�. ���� SEM �� Set Error Mode �� �ǹ��ϴ� �� ����.
	// https://stackoverflow.com/questions/17853625/what-does-sem-failcriticalerrors-prevent 
	// https://docs.microsoft.com/en-us/windows/win32/api/errhandlingapi/nf-errhandlingapi-seterrormode SetErrorMode �Լ�

	PreviousExceptionFilter = SetUnhandledExceptionFilter(UnHandledExceptionFilter); 
	// �������� top-level exception handler(�ֻ��� ���� ���� ó����, ���� ���� ����Ǵ� ���� ó����� ���� �ȴ�.) �� ���� ������ UnhandledExceptionFilter �� ���� �ִ�.
	// ������ Exception �� ���ؼ� ó���ϴ� �Լ��� �ִµ�, �װ��� ���� ������ �Լ��� �ٲٵ��� �������ִ� ���̶�� ���� �ȴ�.
	// MSDN �� ����� �ÿ��� �̰� ȣ�� �ȵȴٰ� �� �־ �غôµ�, ȣ���� �ȵȴ�. ������ �غ��� �翬�ѵ�, ������ ����� �ÿ��� � ������ �ִ��� �˱� ������ ���� �̰��� �۵���Ű�� �ʴ� �� ����.

	return true;
}

BOOL CMiniDump::End(VOID) {
	SetUnhandledExceptionFilter(PreviousExceptionFilter);

	return true;
}