#include "stdafx.h"
#include "Iocp.h"

DWORD WINAPI WorkerThreadCallback(LPVOID parameter)
{
	CIocp* Owner = (CIocp*)parameter;
	Owner->WorkerThreadCallback();

	return 0;
}

/// <summary>
/// IOCP �������̴�. 
/// </summary>
/// <param name=""></param>
CIocp::CIocp(VOID)
{
	mIocpHandle			= NULL; // IOCP �� �����ϴ� �ڵ�, IOCP ���� �� ���� �ڵ��� IOCP ��� ������ �Ѵ�.
	mWorkerThreadCount	= 0;	// ����� WorkerThread ����

	mStartupEventHandle = NULL;	// ������ �����ϴ� �̺�Ʈ �ڵ�
}

CIocp::~CIocp(VOID)
{
}

/// <summary>
/// IOCP ���� �Լ��̴�.
/// </summary>
/// <param name=""></param>
/// <returns></returns>
BOOL CIocp::Begin(VOID)
{
	// IOCP �ڵ� �ʱ�ȭ
	mIocpHandle = NULL;

	// ���� �ý����� ������ �����´�. ������ �������� CPU ������ ����Ѵ�.
	SYSTEM_INFO SystemInfo;
	GetSystemInfo(&SystemInfo);

	// CPU ������ 2�� ��ŭ�� WorkerThread�� ���� ���̴�. �̰� ���������� ���ٰ� �򰡹޴� ��ġ�̴�.
	// ���� ������ �ӵ��� �� ������ ������ ���� �޾ƾ� �Ѵٸ� WorkerThread �� �ø� ���� �ִٰ� �Ѵ�.
	mWorkerThreadCount = SystemInfo.dwNumberOfProcessors * 2;

	/*HANDLE WINAPI CreateIoCompletionPort(
		_In_     HANDLE    FileHandle,					// IOCP �� ����� �ڵ� ��
		_In_opt_ HANDLE    ExistingCompletionPort,		// ����� ���� �̸� �����Ǿ� �ִ� IOCP �ڵ� ���� �־��ش�. �����ÿ��� NULL�� �Է��Ѵ�.
															NULL �ƴϸ� NumberOfConcurrentThreads ���� ���� ���õȴ�.	
		_In_     ULONG_PTR CompletionKey,				// ������ش� �ڵ� ���� Ű (��κ��� ���� ��ü�� �Է�)
		_In_     DWORD     NumberOfConcurrentThreads	// �ý��ۿ��� IOC������ �Ҵ��� ������ ����, 0�� ��� �ڵ����� CPU ������ �����ȴ�.
	);*/
	// IOCP �ڵ� ����, ���� IOCP �ڵ��̹Ƿ� ��� �Ķ���� ���� 0�̳� NULL�̴�. ������ �߰��� ������ �ٸ� �з����͵��� ����.
	mIocpHandle = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);

	// IOCP �ڵ� ���� ���������� �����Ѵ�.
	if (!mIocpHandle)
		return FALSE;

	// ������ �����ϴ� �̺�Ʈ ����
	mStartupEventHandle = CreateEvent(0, FALSE, FALSE, 0);
	if (mStartupEventHandle == NULL)
	{
		End();

		return FALSE;
	}

	// CPU ������ 2�踸ŭ�� WorkerThread �� �����ϰ�, mWorkerThreadVector �� ���� �����Ѵ�.
	for (DWORD i = 0; i < mWorkerThreadCount; i++)
	{
		HANDLE WorkerThread = CreateThread(NULL, 0, ::WorkerThreadCallback, this, 0, NULL);
		mWorkerThreadVector.push_back(WorkerThread);

		// �ϳ��� �����尡 ������ �Ϸ�Ǳ⸦ ��ٸ���.
		WaitForSingleObject(mStartupEventHandle, INFINITE);
	}

	return TRUE;
}

/// <summary>
/// IOCP ���� �Լ��̴�.
/// </summary>
/// <param name=""></param>
/// <returns></returns>
BOOL CIocp::End(VOID)
{
	// IOCP �� �̿��ؼ� �� WorkerThread���� ���Ḧ ��û�Ѵ�.
	for (DWORD i = 0; i < mWorkerThreadVector.size(); i++)

		/*BOOL WINAPI PostQueuedCompletionStatus(				// IOCP �� ���������� ��ȣ�� ������ �Լ�
			_In_     HANDLE       CompletionPort,				// ��ȣ�� ���� IOCP �ڵ�
			_In_     DWORD        dwNumberOfBytesTransferred,	// GetQueuedCompletionStatus ���� dwNumberOfBytesTransferred �Ķ���Ͱ� �޴� ��
			_In_     ULONG_PTR    dwCompletionKey,				// GetQueuedCompletionStatus ���� dwCompletionKey �Ķ���Ͱ� �޴� ��
			_In_opt_ LPOVERLAPPED lpOverlapped					// GetQueuedCompletionStatus ���� lpOverlapped �Ķ���Ͱ� �޴� ��
		);*/
		PostQueuedCompletionStatus(mIocpHandle, 0, 0, NULL);
	
	for (DWORD i = 0; i < mWorkerThreadVector.size(); i++)
	{
		// �� WorkerThraed �� ����Ǳ⸦ ��ٸ� �� �ڵ��� �ݴ´�.
		WaitForSingleObject(mWorkerThreadVector[i], INFINITE);
		
		CloseHandle(mWorkerThreadVector[i]);
	}

	// IOCP �ڵ� ����
	if (mIocpHandle)
		CloseHandle(mIocpHandle);

	// WorkerThread �� �����ϴ� ���� �ʱ�ȭ
	mWorkerThreadVector.clear();

	// ���� �����ϴ� �̺�Ʈ ����
	if (mStartupEventHandle)
		CloseHandle(mStartupEventHandle);

	return TRUE;
}

/// <summary>
/// IOCP �� �����̳� ���� �ڵ��� ����ϴ� �Լ��̴�.
/// ���⼭ ������� ������ IOCP ���� ��ȣ�� ã�Ƴ� �� ����.
/// </summary>
/// <param name="socket"></param>
/// <param name="completionKey"></param>
/// <returns></returns>
BOOL CIocp::RegisterSocketToIocp(SOCKET socket, ULONG_PTR completionKey)
{
	// ���� �ڵ��� ���ų� completionKey ���� ���� ��쿡�� ��ϵ��� �ʴ´�.
	if (!socket || !completionKey)
		return FALSE;

	// Begin ���� ����� �ٸ��� �Ķ���Ͱ� �� �ִ�. mIocpHandle �� ���� �ڵ� �� Ű�� ����Ѵ�.
	mIocpHandle = CreateIoCompletionPort((HANDLE)socket, mIocpHandle, completionKey, 0);

	if (!mIocpHandle)
		return FALSE;

	return TRUE;
}

/// <summary>
/// IOCP ��Ŀ �����忡�� �۵��Ǵ� ���� �ٽ� �Լ��̴�.
/// </summary>
/// <param name=""></param>
VOID CIocp::WorkerThreadCallback(VOID)
{
	BOOL			Successed				= FALSE;	// ����, ���� ���� �Ǵ��ϴ� ����
	DWORD			NumberOfByteTransfered	= 0;		// �� ����Ʈ IO�� �߻��ߴ��� Ȯ���ϴ� ���� 	
	VOID*			CompletionKey			= NULL;		// Ű �� ����
	OVERLAPPED*		Overlapped				= NULL;		// Overlapped ����Ű�� ������
	OVERLAPPED_EX*	OverlappedEx			= NULL;		// OverlappedEx ����Ű�� ������
	VOID*			Object					= NULL;		// ���� �ڵ鿡 ��Ī�Ǵ� ��ü ������ ��

	while (TRUE)
	{
		// WorkerThread �� �����ߴٰ� �˸���. �̸� ���� Begin ���� ���� ������ ���� ��ƾ���� �Ѿ��.
		SetEvent(mStartupEventHandle);

		/*BOOL GetQueuedCompletionStatus(
			HANDLE       CompletionPort,
			LPDWORD      lpNumberOfBytesTransferred,
			PULONG_PTR   lpCompletionKey,
			LPOVERLAPPED * lpOverlapped,
			DWORD        dwMilliseconds			 
		);*/
		// PostQueuedCompletionStatus �� IOCP�� ��ϵ� ���� �Ǵ� ������ Read, Write ���� �Լ��� ���¸� �޾ƿ��� �Լ�
		Successed = GetQueuedCompletionStatus(	// True �� ���� �����̴�.
			mIocpHandle,						// IOCP �ڵ� ��, �̰��� ���ؼ� PostQueuedCompletionStatus �� �Բ� �۵��Ѵ�.
			&NumberOfByteTransfered,			// IO �� ������ ũ��
			(LPDWORD)&CompletionKey,			// �߻��� IO �� Ű ��
			&Overlapped,						// �߻��� IO �� Overlapped ��
			INFINITE);							// IO �� �߻����� �ʾ��� ���� ��� �ð�, ���⼭�� ���Ѵ�� ��� ��ٸ���.

		// Ű�� NULL �� ��� ����, End ���� PostQueuedCompletionStatus �� Ű�� NULL �� �־��� ������ ���� �����Ѵ�.
		// �Ǵ� �ڵ��� �����̰ų� �������� ������ ���� �ִ�. ���ϰ� CompletionKey ���� ���ٴ� ���� ���� �ִ� ��Ȳ�̹Ƿ� ����.
		if (!CompletionKey)
			return;

		// Overlapped �����͸� �޾� OverlappedEx�� �����͸� Ȯ���Ѵ�. OverlappedEx �� �� ó�� ����� Overlapped �� �̰� �����ϴ�.
		// CNetworkSession ���� WSARecv, WSASend ���� �� ���� �Ѱ��ִ� OVERLAPPED_EX �� �״�� �޴´�.
		OverlappedEx = (OVERLAPPED_EX*)Overlapped;

		// CNetwrokSession ���� Object �� this�� �Է����־� � ��ü���� Ȯ���� �� �ְ� �Ͽ���.
		Object = OverlappedEx->Object;

		// ����� ���� �Ǵ� ���������� IO �� ����Ʈ ũ�Ⱑ 0�� ����,
		// Ŭ���̾�Ʈ�� AcceptEx �� ���ؼ� Accept �� ����, Ŭ���̾�Ʈ ������ ����� ����̴�.
		if (!Successed || (Successed && !NumberOfByteTransfered))
		{
			if (OverlappedEx->IoType == IO_ACCEPT)
				// AcceotEx �� ���ؼ� Accept �� ���
				OnIoConnected(Object);
			else
				// Ŭ���̾�Ʈ ������ ����� ���
				OnIoDisconnected(Object);
			
			// �ٽ� ó������ ���ư���.
			continue;
		}

		// �տ��� AcceptEx, WSARecv, WSASend ���� OverlappedEx �������� �̸� IO_READ, IO_WRITE �� �־��־���. 
		switch (OverlappedEx->IoType)
		{
		case IO_READ:
			// �����͸� ���� ��� (WSARecv, WSARecvFrom)
			OnIoRead(Object, NumberOfByteTransfered);
			break;

		case IO_WRITE:
			// �����͸� ���� ��� (WSASend, WSASendTo)
			OnIoWrote(Object, NumberOfByteTransfered);
			break;
		}
	}
}