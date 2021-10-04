#include "stdafx.h"
#include "Iocp.h"

DWORD WINAPI WorkerThreadCallback(LPVOID parameter)
{
	CIocp* Owner = (CIocp*)parameter;
	Owner->WorkerThreadCallback();

	return 0;
}

/// <summary>
/// IOCP 생성자이다. 
/// </summary>
/// <param name=""></param>
CIocp::CIocp(VOID)
{
	mIocpHandle			= NULL; // IOCP 를 관리하는 핸들, IOCP 생성 및 소켓 핸들의 IOCP 등록 역할을 한다.
	mWorkerThreadCount	= 0;	// 사용할 WorkerThread 개수

	mStartupEventHandle = NULL;	// 시작을 관리하는 이벤트 핸들
}

CIocp::~CIocp(VOID)
{
}

/// <summary>
/// IOCP 시작 함수이다.
/// </summary>
/// <param name=""></param>
/// <returns></returns>
BOOL CIocp::Begin(VOID)
{
	// IOCP 핸들 초기화
	mIocpHandle = NULL;

	// 서버 시스템의 정보를 가져온다. 가져온 정보에서 CPU 정보를 사용한다.
	SYSTEM_INFO SystemInfo;
	GetSystemInfo(&SystemInfo);

	// CPU 개수의 2배 만큼의 WorkerThread를 만들 것이다. 이게 경험적으로 좋다고 평가받는 수치이다.
	// 만약 서버가 속도가 좀 느려도 응답을 많이 받아야 한다면 WorkerThread 를 늘릴 수도 있다고 한다.
	mWorkerThreadCount = SystemInfo.dwNumberOfProcessors * 2;

	/*HANDLE WINAPI CreateIoCompletionPort(
		_In_     HANDLE    FileHandle,					// IOCP 에 등록할 핸들 값
		_In_opt_ HANDLE    ExistingCompletionPort,		// 등록할 때에 미리 생성되어 있는 IOCP 핸들 값을 넣어준다. 생성시에는 NULL을 입력한다.
															NULL 아니면 NumberOfConcurrentThreads 변수 값은 무시된다.	
		_In_     ULONG_PTR CompletionKey,				// 등록할해당 핸들 값의 키 (대부분은 연결 개체를 입력)
		_In_     DWORD     NumberOfConcurrentThreads	// 시스템에서 IOC용으로 할당할 스레드 개수, 0일 경우 자동으로 CPU 갯수로 설정된다.
	);*/
	// IOCP 핸들 생성, 메인 IOCP 핸들이므로 모든 파라미터 값이 0이나 NULL이다. 앞으로 추가될 때에는 다른 패러미터들이 들어간다.
	mIocpHandle = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);

	// IOCP 핸들 생성 실패했으면 종료한다.
	if (!mIocpHandle)
		return FALSE;

	// 시작을 관라하는 이벤트 생성
	mStartupEventHandle = CreateEvent(0, FALSE, FALSE, 0);
	if (mStartupEventHandle == NULL)
	{
		End();

		return FALSE;
	}

	// CPU 개수의 2배만큼의 WorkerThread 를 생성하고, mWorkerThreadVector 를 통해 관리한다.
	for (DWORD i = 0; i < mWorkerThreadCount; i++)
	{
		HANDLE WorkerThread = CreateThread(NULL, 0, ::WorkerThreadCallback, this, 0, NULL);
		mWorkerThreadVector.push_back(WorkerThread);

		// 하나의 스레드가 생성이 완료되기를 기다린다.
		WaitForSingleObject(mStartupEventHandle, INFINITE);
	}

	return TRUE;
}

/// <summary>
/// IOCP 종료 함수이다.
/// </summary>
/// <param name=""></param>
/// <returns></returns>
BOOL CIocp::End(VOID)
{
	// IOCP 를 이용해서 각 WorkerThread마다 종료를 요청한다.
	for (DWORD i = 0; i < mWorkerThreadVector.size(); i++)

		/*BOOL WINAPI PostQueuedCompletionStatus(				// IOCP 에 강제적으로 신호를 보내는 함수
			_In_     HANDLE       CompletionPort,				// 신호를 보낼 IOCP 핸들
			_In_     DWORD        dwNumberOfBytesTransferred,	// GetQueuedCompletionStatus 에서 dwNumberOfBytesTransferred 파라미터가 받는 값
			_In_     ULONG_PTR    dwCompletionKey,				// GetQueuedCompletionStatus 에서 dwCompletionKey 파라미터가 받는 값
			_In_opt_ LPOVERLAPPED lpOverlapped					// GetQueuedCompletionStatus 에서 lpOverlapped 파라미터가 받는 값
		);*/
		PostQueuedCompletionStatus(mIocpHandle, 0, 0, NULL);
	
	for (DWORD i = 0; i < mWorkerThreadVector.size(); i++)
	{
		// 각 WorkerThraed 가 종료되기를 기다린 후 핸들을 닫는다.
		WaitForSingleObject(mWorkerThreadVector[i], INFINITE);
		
		CloseHandle(mWorkerThreadVector[i]);
	}

	// IOCP 핸들 종료
	if (mIocpHandle)
		CloseHandle(mIocpHandle);

	// WorkerThread 를 관리하는 벡터 초기화
	mWorkerThreadVector.clear();

	// 시작 관리하는 이벤트 종료
	if (mStartupEventHandle)
		CloseHandle(mStartupEventHandle);

	return TRUE;
}

/// <summary>
/// IOCP 에 소켓이나 파일 핸들을 등록하는 함수이다.
/// 여기서 등록하지 않으면 IOCP 에서 신호를 찾아낼 수 없다.
/// </summary>
/// <param name="socket"></param>
/// <param name="completionKey"></param>
/// <returns></returns>
BOOL CIocp::RegisterSocketToIocp(SOCKET socket, ULONG_PTR completionKey)
{
	// 소켓 핸들이 없거나 completionKey 값이 없을 경우에는 등록되지 않는다.
	if (!socket || !completionKey)
		return FALSE;

	// Begin 에서 모습과 다르게 파라미터가 들어가 있다. mIocpHandle 에 소켓 핸들 및 키를 등록한다.
	mIocpHandle = CreateIoCompletionPort((HANDLE)socket, mIocpHandle, completionKey, 0);

	if (!mIocpHandle)
		return FALSE;

	return TRUE;
}

/// <summary>
/// IOCP 워커 스레드에서 작동되는 가장 핵심 함수이다.
/// </summary>
/// <param name=""></param>
VOID CIocp::WorkerThreadCallback(VOID)
{
	BOOL			Successed				= FALSE;	// 성공, 실패 여부 판단하는 변수
	DWORD			NumberOfByteTransfered	= 0;		// 몇 바이트 IO가 발생했는지 확인하는 변수 	
	VOID*			CompletionKey			= NULL;		// 키 값 변수
	OVERLAPPED*		Overlapped				= NULL;		// Overlapped 가리키는 포인터
	OVERLAPPED_EX*	OverlappedEx			= NULL;		// OverlappedEx 가리키는 포인터
	VOID*			Object					= NULL;		// 소켓 핸들에 매칭되는 객체 포인터 값

	while (TRUE)
	{
		// WorkerThread 가 시작했다고 알린다. 이를 통해 Begin 에서 다음 스레드 생성 루틴으로 넘어간다.
		SetEvent(mStartupEventHandle);

		/*BOOL GetQueuedCompletionStatus(
			HANDLE       CompletionPort,
			LPDWORD      lpNumberOfBytesTransferred,
			PULONG_PTR   lpCompletionKey,
			LPOVERLAPPED * lpOverlapped,
			DWORD        dwMilliseconds			 
		);*/
		// PostQueuedCompletionStatus 나 IOCP에 등록된 소켓 또는 파일의 Read, Write 관련 함수의 상태를 받아오는 함수
		Successed = GetQueuedCompletionStatus(	// True 일 때에 성공이다.
			mIocpHandle,						// IOCP 핸들 값, 이것을 통해서 PostQueuedCompletionStatus 도 함께 작동한다.
			&NumberOfByteTransfered,			// IO 된 데이터 크기
			(LPDWORD)&CompletionKey,			// 발생한 IO 의 키 값
			&Overlapped,						// 발생한 IO 의 Overlapped 값
			INFINITE);							// IO 가 발생하지 않았을 때의 대기 시간, 여기서는 무한대로 계속 기다린다.

		// 키가 NULL 일 경우 종료, End 에서 PostQueuedCompletionStatus 로 키를 NULL 로 넣어준 것으로 인해 동작한다.
		// 또는 코딩의 문제이거나 데이터의 문제일 수도 있다. 여하간 CompletionKey 값이 없다는 것은 문제 있는 상황이므로 종료.
		if (!CompletionKey)
			return;

		// Overlapped 포인터를 받아 OverlappedEx의 포인터를 확인한다. OverlappedEx 의 맨 처음 멤버가 Overlapped 라서 이게 가능하다.
		// CNetworkSession 에서 WSARecv, WSASend 등을 할 때에 넘겨주는 OVERLAPPED_EX 를 그대로 받는다.
		OverlappedEx = (OVERLAPPED_EX*)Overlapped;

		// CNetwrokSession 에서 Object 에 this를 입력해주어 어떤 개체인지 확인할 수 있게 하였다.
		Object = OverlappedEx->Object;

		// 결과가 실패 또는 성공이지만 IO 된 바이트 크기가 0인 경우는,
		// 클라이언트가 AcceptEx 를 통해서 Accept 한 경우와, 클라이언트 접속이 종료된 경우이다.
		if (!Successed || (Successed && !NumberOfByteTransfered))
		{
			if (OverlappedEx->IoType == IO_ACCEPT)
				// AcceotEx 를 통해서 Accept 한 경우
				OnIoConnected(Object);
			else
				// 클라이언트 접속이 종료된 경우
				OnIoDisconnected(Object);
			
			// 다시 처음으로 돌아간다.
			continue;
		}

		// 앞에서 AcceptEx, WSARecv, WSASend 에서 OverlappedEx 종류마다 미리 IO_READ, IO_WRITE 를 넣어주었다. 
		switch (OverlappedEx->IoType)
		{
		case IO_READ:
			// 데이터를 받은 경우 (WSARecv, WSARecvFrom)
			OnIoRead(Object, NumberOfByteTransfered);
			break;

		case IO_WRITE:
			// 데이터를 보낼 경우 (WSASend, WSASendTo)
			OnIoWrote(Object, NumberOfByteTransfered);
			break;
		}
	}
}