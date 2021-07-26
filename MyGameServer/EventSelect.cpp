#include "EventSelect.h"

DWORD WINAPI SelectThreadCallback(LPVOID parameter)
{
	CEventSelect* Owner = (CEventSelect*)parameter;
	Owner->SelectThreadCallback();

	return 0;
}

/// <summary>
/// ���� �̺�Ʈ�� �����ϴ� �����忡�� ����Ǵ� �Լ��̴�.
/// </summary>
/// <param name=""></param>
VOID CEventSelect::SelectThreadCallback(VOID)
{
	// ��Ʈ��ũ �̺�Ʈ�� �����ϴ� ����
	WSANETWORKEVENTS	NetworkEvents;
	DWORD				EventID = 0;
	DWORD				Result = 0;

	// ���� �̺�Ʈ�� ��Ʈ��ũ �̺�Ʈ 2���� ����
	HANDLE				ThreadEvents[2] = { mDestroyEventHandle, mSelectEventHandle };

	while (TRUE)
	{
		// �����尡 ���������� ���۵Ǿ��ٴ� ���� Begin �Լ��� �˷��ش�.
		SetEvent(mStartupEventHandle);

		// Ư�� �̺�Ʈ�� �߻��� ������ ��� ����Ѵ�.
		EventID = ::WaitForMultipleObjects(2, ThreadEvents, FALSE, INFINITE);
		switch (EventID)
		{
		// ������ ���� �̺�Ʈ�� �߻����� ���
		case WAIT_OBJECT_0:
			return;

		// ���� �̺�Ʈ�� �߻����� ���
		case WAIT_OBJECT_0 + 1:
			/*int WSAAPI WSAEnumNetworkEvents(
				SOCKET             s,
				WSAEVENT           hEventObject,		// �̺�Ʈ�� �����ϴ� �ڵ�
				LPWSANETWORKEVENTS lpNetworkEvents		// �߻��� �̺�Ʈ�� ��� ���� ������ (���� ����)
			);*/
			// mSelectEventHandle ���� ��Ʈ��ũ �̺�Ʈ�� �޾ƿ´�.
			EventID = WSAEnumNetworkEvents(mSocket, mSelectEventHandle, &NetworkEvents);

			// EventID �� ���� ������ ��� ������ ���� ���̹Ƿ� ������ ����, ���� ������ WSAGetLastError �� ���� Ȯ�� ����
			if (EventID == SOCKET_ERROR)
				return;
			else
			{
				// Connect �̺�Ʈ�� �߻����� ���
				if (NetworkEvents.lNetworkEvents & FD_CONNECT)
				{
					// Connect ���� �����Լ� ȣ��
					OnIoConnected();

					if (NetworkEvents.iErrorCode[FD_CONNECT_BIT])
						return;
				}
				// Write �̺�Ʈ�� �߻����� ���
				else if (NetworkEvents.lNetworkEvents & FD_WRITE)
				{
					// ���⼭�� ���� �ʴ´�. Write �̺�Ʈ�� ���� ������ ���� Write �� ���� ���������� �� �� �����Ƿ�..
					//OnIoWrote();
				}
				// Read �̺�Ʈ�� �߻����� ���
				else if (NetworkEvents.lNetworkEvents & FD_READ)
				{
					// Read ���� �����Լ� ȣ��
					OnIoRead();
				}
				// Disconnect �̺�Ʈ�� �߻����� ���
				else if (NetworkEvents.lNetworkEvents & FD_CLOSE)
				{
					// Disconnect ���� �����Լ� ȣ��
					OnIoDisconnected();

					return;
				}
			}
			break;
		default:
			return;
		}
	}
}

/// <summary>
/// WSAEventSelect �� ����ϴ� Ŭ������ �������̴�. ���� ������ �ƴ� Ŭ���̾�Ʈ���� �� Ŭ������ ���ȴ�.
/// </summary>
/// <param name=""></param>
CEventSelect::CEventSelect(VOID)
{
	mSelectEventHandle	= NULL;		// ���� �̺�Ʈ �ڵ� �ʱ�ȭ
	mDestroyEventHandle = NULL;		// ������ ���� �̺�Ʈ �ڵ� �ʱ�ȭ
	mSelectThreadHandle = NULL;		// ���� �̺�Ʈ �����ϴ� ������ �ڵ� �ʱ�ȭ
	mStartupEventHandle = NULL;		// ������ ������ üũ�ϴ� �̺�Ʈ �ڵ� �ʱ�ȭ

	mSocket				= NULL;		// ���� �ڵ� �ʱ�ȭ
}

CEventSelect::~CEventSelect(VOID)
{
}

/// <summary>
/// �̺�Ʈ ����Ʈ ��ü ���� �Լ��̴�.
/// ������ ���� CNetwrokSession�� GetSocket �� ���� �޾ƿ��� ��Ĺ �ڵ��� �޾ƿ´�.
/// </summary>
/// <param name="socket"></param>
/// <returns></returns>
BOOL CEventSelect::Begin(SOCKET socket)
{
	if (!socket)
		return FALSE;

	// mSocket �� NULL �� �ƴ϶� ����, �̹� ����ǰ� �ִ� ������ �ִ� ���̹Ƿ� �������� �ʴ´�.
	if (mSocket)
		return FALSE;

	// �޾ƿ� ������ mSocket �� �Ҵ�, �� ������ ������ mSocket �� NULL �� �ƴϸ� �������� �ʴ� ���̴�.
	mSocket = socket;

	// ���� �̺�Ʈ�� �����ϴ� �̺�Ʈ�� ����
	mSelectEventHandle = WSACreateEvent();
	if (mSelectEventHandle == WSA_INVALID_EVENT)
	{
		End();

		return FALSE;
	}

	// ������ ���� �̺�Ʈ ����
	mDestroyEventHandle = CreateEvent(0, FALSE, FALSE, 0);
	if (mDestroyEventHandle == NULL)
	{
		End();

		return FALSE;
	}

	// ������ ���� �̺�Ʈ ����
	mStartupEventHandle = CreateEvent(0, FALSE, FALSE, 0);
	if (mStartupEventHandle == NULL)
	{
		End();

		return FALSE;
	}

	/*int WSAAPI WSAEventSelect(		// �� �Լ��� �̺�Ʈ ��ü�� � ��Ʈ��ũ �̺�Ʈ���� ���������� ���ϴ� �Լ��̴�. 0�� ���ϵ� ��� �����̴�.
		SOCKET   s,
		WSAEVENT hEventObject,			// � �̺�Ʈ��� ������ �����Ǵ� �̺�Ʈ �ڵ�
		long     lNetworkEvents			// � �̺�Ʈ���� ������ �� ���ϴ� �÷��� ��Ʈ�� ���� ����
	);*/
	// Connect, Read, Write, Close ���� �̺�Ʈ�� �����ϴ� ������ ����
	DWORD Result = WSAEventSelect(socket, mSelectEventHandle, FD_CONNECT | FD_READ | FD_WRITE | FD_CLOSE);
	if (Result == SOCKET_ERROR)
	{
		End();

		return FALSE;
	}

	// ���� �̺�Ʈ���� ������ �����带 ����
	DWORD SelectThreadID = 0;
	mSelectThreadHandle = CreateThread(NULL, 0, ::SelectThreadCallback, this, 0, &SelectThreadID);

	if (!mSelectThreadHandle)
	{
		End();

		return FALSE;
	}

	// Thread�� �����ǰ� mStartupEventhandle �̺�Ʈ�� �߻��ϸ� �� �Լ��� ����ȴ�.
	WaitForSingleObject(mStartupEventHandle, INFINITE);

	return TRUE;
}

/// <summary>
/// �̺�Ʈ ����Ʈ ���� �Լ��̴�.
/// </summary>
/// <param name=""></param>
/// <returns></returns>
BOOL CEventSelect::End(VOID)
{
	// mSocket �� ������ �������� �ʴ´�.
	if (!mSocket)
		return FALSE;

	// �����尡 ���� ���̸� �����Ų��.
	if (mSelectThreadHandle)
	{
		// ������ �����ϴ� �̺�Ʈ �߻���Ų��.
		SetEvent(mDestroyEventHandle);

		// �����尡 ������ ����� ������ ��ٸ���.
		WaitForSingleObject(mSelectThreadHandle, INFINITE);

		// ����Ǿ��� ��� ������ �ڵ��� �ʱ�ȭ�Ѵ�.
		CloseHandle(mSelectThreadHandle);
	}

	// �̺�Ʈ�� �����Ų��.
	if (mSelectEventHandle)
		CloseHandle(mSelectEventHandle);

	if (mDestroyEventHandle)
		CloseHandle(mDestroyEventHandle);

	if (mStartupEventHandle)
		CloseHandle(mStartupEventHandle);

	return TRUE;
}
