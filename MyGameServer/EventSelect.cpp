#include "EventSelect.h"

DWORD WINAPI SelectThreadCallback(LPVOID parameter)
{
	CEventSelect* Owner = (CEventSelect*)parameter;
	Owner->SelectThreadCallback();

	return 0;
}

/// <summary>
/// 소켓 이벤트를 관리하는 스레드에서 실행되는 함수이다.
/// </summary>
/// <param name=""></param>
VOID CEventSelect::SelectThreadCallback(VOID)
{
	// 네트워크 이벤트를 관리하는 변수
	WSANETWORKEVENTS	NetworkEvents;
	DWORD				EventID = 0;
	DWORD				Result = 0;

	// 종료 이벤트와 네트워크 이벤트 2개로 구분
	HANDLE				ThreadEvents[2] = { mDestroyEventHandle, mSelectEventHandle };

	while (TRUE)
	{
		// 스레드가 정상적으로 시작되었다는 것을 Begin 함수에 알려준다.
		SetEvent(mStartupEventHandle);

		// 특정 이벤트가 발생할 때까지 계속 대기한다.
		EventID = ::WaitForMultipleObjects(2, ThreadEvents, FALSE, INFINITE);
		switch (EventID)
		{
		// 스레드 종료 이벤트가 발생했을 경우
		case WAIT_OBJECT_0:
			return;

		// 소켓 이벤트가 발생했을 경우
		case WAIT_OBJECT_0 + 1:
			/*int WSAAPI WSAEnumNetworkEvents(
				SOCKET             s,
				WSAEVENT           hEventObject,		// 이벤트를 관리하는 핸들
				LPWSANETWORKEVENTS lpNetworkEvents		// 발생된 이벤트를 담는 변수 포인터 (에러 포함)
			);*/
			// mSelectEventHandle 에서 네트워크 이벤트를 받아온다.
			EventID = WSAEnumNetworkEvents(mSocket, mSelectEventHandle, &NetworkEvents);

			// EventID 가 소켓 에러인 경우 문제가 생긴 것이므로 스레드 종료, 에러 내용은 WSAGetLastError 를 통해 확인 가능
			if (EventID == SOCKET_ERROR)
				return;
			else
			{
				// Connect 이벤트가 발생했을 경우
				if (NetworkEvents.lNetworkEvents & FD_CONNECT)
				{
					// Connect 관련 가상함수 호출
					OnIoConnected();

					if (NetworkEvents.iErrorCode[FD_CONNECT_BIT])
						return;
				}
				// Write 이벤트가 발생했을 경우
				else if (NetworkEvents.lNetworkEvents & FD_WRITE)
				{
					// 여기서는 쓰지 않는다. Write 이벤트가 끝난 시점이 정말 Write 를 다한 시점인지는 알 수 없으므로..
					//OnIoWrote();
				}
				// Read 이벤트가 발생했을 경우
				else if (NetworkEvents.lNetworkEvents & FD_READ)
				{
					// Read 관련 가상함수 호출
					OnIoRead();
				}
				// Disconnect 이벤트가 발생했을 경우
				else if (NetworkEvents.lNetworkEvents & FD_CLOSE)
				{
					// Disconnect 관련 가상함수 호출
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
/// WSAEventSelect 를 사용하는 클래스의 생성자이다. 보통 서버가 아닌 클라이언트에서 이 클래스가 사용된다.
/// </summary>
/// <param name=""></param>
CEventSelect::CEventSelect(VOID)
{
	mSelectEventHandle	= NULL;		// 소켓 이벤트 핸들 초기화
	mDestroyEventHandle = NULL;		// 스레드 종료 이벤트 핸들 초기화
	mSelectThreadHandle = NULL;		// 소켓 이벤트 관리하는 스레드 핸들 초기화
	mStartupEventHandle = NULL;		// 스레드 시작을 체크하는 이벤트 핸들 초기화

	mSocket				= NULL;		// 소켓 핸들 초기화
}

CEventSelect::~CEventSelect(VOID)
{
}

/// <summary>
/// 이벤트 셀렉트 개체 시작 함수이다.
/// 시작할 때에 CNetwrokSession의 GetSocket 을 통해 받아오는 소캣 핸들을 받아온다.
/// </summary>
/// <param name="socket"></param>
/// <returns></returns>
BOOL CEventSelect::Begin(SOCKET socket)
{
	if (!socket)
		return FALSE;

	// mSocket 이 NULL 이 아니란 것은, 이미 실행되고 있는 소켓이 있는 것이므로 실행하지 않는다.
	if (mSocket)
		return FALSE;

	// 받아온 소켓을 mSocket 에 할당, 이 때문에 위에서 mSocket 이 NULL 이 아니면 실행하지 않는 것이다.
	mSocket = socket;

	// 소켓 이벤트를 관리하는 이벤트를 생성
	mSelectEventHandle = WSACreateEvent();
	if (mSelectEventHandle == WSA_INVALID_EVENT)
	{
		End();

		return FALSE;
	}

	// 스레드 종료 이벤트 생성
	mDestroyEventHandle = CreateEvent(0, FALSE, FALSE, 0);
	if (mDestroyEventHandle == NULL)
	{
		End();

		return FALSE;
	}

	// 스레드 시작 이벤트 생성
	mStartupEventHandle = CreateEvent(0, FALSE, FALSE, 0);
	if (mStartupEventHandle == NULL)
	{
		End();

		return FALSE;
	}

	/*int WSAAPI WSAEventSelect(		// 이 함수는 이벤트 객체가 어떤 네트워크 이벤트들을 관리할지를 정하는 함수이다. 0이 리턴될 경우 성공이다.
		SOCKET   s,
		WSAEVENT hEventObject,			// 어떤 이벤트들과 묶일지 결정되는 이벤트 핸들
		long     lNetworkEvents			// 어떤 이벤트들을 관리할 지 정하는 플래그 비트를 담은 변수
	);*/
	// Connect, Read, Write, Close 소켓 이벤트를 관리하는 것으로 설정
	DWORD Result = WSAEventSelect(socket, mSelectEventHandle, FD_CONNECT | FD_READ | FD_WRITE | FD_CLOSE);
	if (Result == SOCKET_ERROR)
	{
		End();

		return FALSE;
	}

	// 소켓 이벤트들을 관리할 스레드를 생성
	DWORD SelectThreadID = 0;
	mSelectThreadHandle = CreateThread(NULL, 0, ::SelectThreadCallback, this, 0, &SelectThreadID);

	if (!mSelectThreadHandle)
	{
		End();

		return FALSE;
	}

	// Thread가 생성되고 mStartupEventhandle 이벤트가 발생하면 이 함수가 종료된다.
	WaitForSingleObject(mStartupEventHandle, INFINITE);

	return TRUE;
}

/// <summary>
/// 이벤트 셀렉트 종료 함수이다.
/// </summary>
/// <param name=""></param>
/// <returns></returns>
BOOL CEventSelect::End(VOID)
{
	// mSocket 이 없으면 실행하지 않는다.
	if (!mSocket)
		return FALSE;

	// 스레드가 실행 중이면 종료시킨다.
	if (mSelectThreadHandle)
	{
		// 스레드 종료하는 이벤트 발생시킨다.
		SetEvent(mDestroyEventHandle);

		// 스레드가 완전히 종료될 때까지 기다린다.
		WaitForSingleObject(mSelectThreadHandle, INFINITE);

		// 종료되었을 경우 스레드 핸들을 초기화한다.
		CloseHandle(mSelectThreadHandle);
	}

	// 이벤트들 종료시킨다.
	if (mSelectEventHandle)
		CloseHandle(mSelectEventHandle);

	if (mDestroyEventHandle)
		CloseHandle(mDestroyEventHandle);

	if (mStartupEventHandle)
		CloseHandle(mStartupEventHandle);

	return TRUE;
}
