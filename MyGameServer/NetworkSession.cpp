#include "NetworkSession.h"
// WaitForSingleObject = time-out 시간이 지날때까지 기다리거나, 어떤 오브젝트가 signaled 될 때까지 기다린다. (blocking), timeout 짧게 하면 스레드 끝나기 전에 리턴한다.
// WAIT_OBJECT_0 해당 이벤트 오브젝트가 signaled 상태가 되었음을 알린다.
// WAIT_TIMEOUT 해당 이벤트 오브젝트 상태가 nonsignaled 인 상태로 time-out 되어버렸음을 알린다.
// WAIT_FAILED 해당 function 자체가 실패, 에러를 알기 위해서 GetLastError 호출이 필요하다.

// SetEvent = Event Object 의 상태를 Signaled 상태로 변환
// CreateEvent = EVent Object Handle 을 생성 
// CreateThread = Thread 생성, 최근에는 _begintreadex 가 더 권장된다고 한다.추후 시간되면 더 조사해본 후 적용.https://killsia.tistory.com/entry/CreateThread-beginthread-beginthreadex%EC%9D%98-%EC%B0%A8%EC%9D%B4

DWORD WINAPI ReliableUdpThreadCallback(LPVOID parameter) 
{
	CNetworkSession* Owner = (CNetworkSession*)parameter;
	Owner->ReliableUdpThreadCallback();

	return 0;
}

CNetworkSession::CNetworkSession(VOID) 
{
	memset(&mAcceptOverlapped, 0, sizeof(mAcceptOverlapped));
	memset(&mReadOverlapped, 0, sizeof(mReadBuffer));
	memset(&mWriteOverlapped, 0, sizeof(mWriteOverlapped));

	memset(&mReadBuffer, 0, sizeof(mReadBuffer));
	memset(&mUdpRemoteInfo, 0, sizeof(mUdpRemoteInfo));

	mSocket							= NULL;
	mReliableUdpThreadHandle		= NULL;			// 보낸 패킷을 받을 때까지 계속 전송해주는 ReliableUDP 를 사용하는 Thread 핸들.
	mReliableUdpThreadStartupEvent	= NULL;			// ReliableUDP를 사용하는 Thread의 시작을 알리는 이벤트.
	mReliableUdpThreadDestroyEvent	= NULL;			// ReliableUDP를 사용하는 Thread의 종료를 알리는 이벤트.
	mReliableUdpThreadWakeUpEvent	= NULL;			// ReliableUDP를 사용하는 Thread 깨울 때에 사용하는 이벤트.
	mReliableUdpWriteCompleteEvent	= NULL;			// 상대가 패킷을 받아서 더 이상 보낼 필요가 없을 때에 사용하는 이벤트.

	mIsReliableUdpSending			= FALSE;		// 현재 Reliable 하게 보내고 있는 data 가 있는지 확인.

	mAcceptOverlapped.IoType		= IO_ACCEPT;	// 여기서 이 값을 넣어 놓아서 나중에 IOCP 에서 어떤 IO 인지를 판단한다.
	mReadOverlapped.IoType			= IO_READ;		// 여기서 이 값을 넣어 놓아서 나중에 IOCP 에서 어떤 IO 인지를 판단한다.
	mWriteOverlapped.IoType			= IO_WRITE;		// 여기서 이 값을 넣어 놓아서 나중에 IOCP 에서 어떤 IO 인지를 판단한다.

	mAcceptOverlapped.Object		= this;
	mReadOverlapped.Object			= this;
	mWriteOverlapped.Object			= this;
}

CNetworkSession::~CNetworkSession(VOID) 
{

}

VOID CNetworkSession::ReliableUdpThreadCallback(VOID)
{
	// 이런 형태로 전역 콜백 함수들을 클래스 내에서 관리하는 것이 유지 보수 및 구조적인 프로그래밍에 좋다고 하셨다. 염두하자.
	DWORD	EventID						= 0;			// ReliableUDPThread 의 시작과 종료를 담당하는 이벤트 ID
	HANDLE	ThreadEvents[2]				= { mReliableUdpThreadDestroyEvent, mReliableUdpThreadWakeUpEvent };	// 시작, 종료 이벤트

	CHAR	RemoteAddress[32]			= { 0, };		// 데이터 보낼 IP 주소
	USHORT	RemotePort					= 0;			// 데이터 보낼 주소의 포트
	BYTE	Data[MAX_BUFFER_LENGTH]		= { 0, };		// 보낼 데이터 담는 배열
	DWORD	DataLength					= 0;			// 보낼 데이터 길이
	VOID*	Object						= NULL;			// 오브젝트, 여기서는 쓰지 않는다.
	
	while (TRUE)
	{
		SetEvent(mReliableUdpThreadStartupEvent);		// ReliableUDPThread 가 시작했다는 것을 알리는 이벤트. UdpBind에서 WairForSingleObject 를 끝낸다.

		// 이벤트들이 발생할 때까지 무한 대기
		EventID = WaitForMultipleObjects(2, ThreadEvents, FALSE, INFINITE);

		switch (EventID)
		{
			// DestroyEvent 발생할 경우 종료.
		case WAIT_OBJECT_0:
			return;
		case WAIT_OBJECT_0 + 1:
NEXT_DATA:
			// Write 일 경우 1개의 보낼 데이터를 큐에서 꺼낸다.
			if (mReliableWriteQueue.Pop(&Object, Data, DataLength, RemoteAddress, RemotePort))
			{
RETRY:
				// WriteTo2 는 WSASendTo 로 실제 데이터를 보낸다. WriteTo 는 보낼 데이터를 큐에 넣는다.
				if (!WriteTo2(RemoteAddress, RemotePort, Data, DataLength))
					return;

				// 데이터가 있을 경우 위에서 처럼 데이터를 보내고, 기다린다. 받은 곳에서 SetEvent 호출하면 여기를 바로 넘어간다.
				// SetEvent 호출 안해도 10 밀리 세컨드(1000분의 1초)만큼만 기다리겠단 뜻. 즉 0.01초만 기다리고, 이를 넘으면 전송 실패로 간주하고 재전송한다.
				DWORD result = WaitForSingleObject(mReliableUdpWriteCompleteEvent, 10);

				if (result == WAIT_OBJECT_0)  // 해당 이벤트가 singaled 상태, 즉 스레드에 의해서 사용될 수 있는 상태이면 넘어간다.
					goto NEXT_DATA;
				else  // 해당 시간안에 처리가 안되면 재시도
					// TODO: 원래는 일정 이상 Retry 를 하면 데이터 손실로 인정하는 부분을 구현해야 한다.
					goto RETRY;
			}
			else
				// WriteTo 함수에서 현재 보내는 데이터가 있는지 없는지 확인하기 위한 FLAG 변수
				mIsReliableUdpSending = FALSE;	// 현재 큐에 더 이상 보낼 데이터가 없으므로 FALSE 로 설정

			break;
		}
	}
}

BOOL CNetworkSession::Begin(VOID) {
	CThreadSync Sync;

	if (mSocket)
		return FALSE;

	memset(mReadBuffer, 0, sizeof(mReadBuffer));
	memset(&mUdpRemoteInfo, 0, sizeof(mUdpRemoteInfo));

	mSocket							= NULL;
	mReliableUdpThreadHandle		= NULL;
	mReliableUdpThreadStartupEvent	= NULL;
	mReliableUdpThreadDestroyEvent	= NULL;
	mReliableUdpThreadWakeUpEvent	= NULL;
	mReliableUdpWriteCompleteEvent	= NULL;

	mIsReliableUdpSending			= FALSE;

	return TRUE;
}

BOOL CNetworkSession::End(VOID)
{
	CThreadSync Sync;

	if (!mSocket)
		return FALSE;

	closesocket(mSocket);
	mSocket = NULL;

	if (mReliableUdpThreadHandle)	// Reliable UDP 실행 중인 경우
	{
		SetEvent(mReliableUdpThreadDestroyEvent);	// 종료 이벤트 호출

		WaitForSingleObject(mReliableUdpThreadHandle, INFINITE);	// 이벤트 종료 기다린다.

		CloseHandle(mReliableUdpThreadHandle);		// 이벤트 종료 후 핸들 닫는다.
	}

	// 관련 이벤트들 모두 종료
	if (mReliableUdpThreadDestroyEvent)
		CloseHandle(mReliableUdpThreadDestroyEvent);

	if (mReliableUdpThreadStartupEvent)
		CloseHandle(mReliableUdpThreadStartupEvent);

	if (mReliableUdpThreadWakeUpEvent)
		CloseHandle(mReliableUdpThreadWakeUpEvent);

	if (mReliableUdpWriteCompleteEvent)
		CloseHandle(mReliableUdpWriteCompleteEvent);

	mReliableWriteQueue.End();

	return TRUE;
}

BOOL CNetworkSession::Listen(USHORT port, INT backLog)
{
	CThreadSync Sync;

	if (port <= 0 || backLog <= 0)
		return FALSE;

	if (!mSocket)
		return FALSE;

	SOCKADDR_IN ListenSocketInfo;

	ListenSocketInfo.sin_family				= AF_INET;
	ListenSocketInfo.sin_port				= htons(port);
	ListenSocketInfo.sin_addr.S_un.S_addr	= htonl(INADDR_ANY);

	if (bind(mSocket, (struct sockaddr*)&ListenSocketInfo, sizeof(SOCKADDR_IN)) == SOCKET_ERROR)
	{
		End();

		return FALSE;
	}

	if (listen(mSocket, backLog) == SOCKET_ERROR)
	{
		End();

		return FALSE;
	}

	LINGER Linger;
	Linger.l_onoff		= 1;
	Linger.l_linger		= 0;

	if (setsockopt(mSocket, SOL_SOCKET, SO_LINGER, (char*)&Linger, sizeof(LINGER)) == SOCKET_ERROR)
	{
		End();

		return FALSE;
	}

	return TRUE;
}

/// <summary>
/// Accept 과정을 처리하는 함수.
/// </summary>
/// <param name="listenSocket"></param>
/// <returns></returns>
BOOL CNetworkSession::Accept(SOCKET listenSocket)
{
	CThreadSync Sync;

	if (!listenSocket)	// Listen 되고 있지 않으면 실행되지 않는다.
		return FALSE;

	if (mSocket)
		return FALSE;

	mSocket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED); // WSA = Windows Sockets API
	/*SOCKET WSAAPI WSASocketW(
		int                 af,					// Address Family, AF_INET 은 IPv4 주소를 사용한다는 의미.
		int                 type,				// SOCK_STREAM 이 TCP 에서 사용되는 전송 방식이다.
		int                 protocol,			// 그래서 여기 IPPROTO_TCP 가 온다.
		LPWSAPROTOCOL_INFOW lpProtocolInfo,		// TODO: 차후에 확인, NULL 이 아니면 여기 입력된 WSAPROTOCOL_INFO 로 묶인다고 한다.
		GROUP               g,					// TODO: 차후 확인, socket group id, 0 이면 group operation 은 행해지지 않는다고 한다.
		DWORD               dwFlags				// 소캣 특성을 지정하는 flag, 여기서는 Overlapped I/O 를 지원하는 소켓을 생성.
	);*/

	if (mSocket == INVALID_SOCKET)
	{
		End();

		return FALSE;
	}

	//TODO: 아래 내용 여기 코드들 다 분석할 때에 다시 확인. 
	// BOOL NoDelay = TRUE; 
	//setsockopt(mSocket, IPPROTO_TCP, TCP_NODELAY, (const char FAR *)&NoDelay, sizeof(NoDelay));
	// 소켓 옵션은 setsockopt 로 세팅하고, getsockopt 로 꺼내와서 확인한다.

	/*BOOL AcceptEx(
		SOCKET       sListenSocket,				// listen 되고 있는 소켓
		SOCKET       sAcceptSocket,				// aceept 받을 소켓
		PVOID        lpOutputBuffer,			// accept 후 read 할 때에 사용할 버퍼 주소 
		DWORD        dwReceiveDataLength,		// read 할 때 사용할 버퍼의 크기 (0일 경우 read 하지 않는다)
		DWORD        dwLocalAddressLength,		// 로컬 주소 정보 길이. 사용하는 전송 프로토콜 주소 최대 길이 보다 16바이트 더 커야한다.
		DWORD        dwRemoteAddressLength,		// 리모트 주소 정보 길이. 사용하는 전송 프로토콜 주소 최대 길이 보다 16바이트 더 커야한다.
		LPDWORD      lpdwBytesReceived,			// 받은 바이트 크기, (Read 된 크기)
		LPOVERLAPPED lpOverlapped				// accept 에서 사용할 overlapped 구조체
	);*/
	// while 문을 사용한 WSAAccept 는 하나의 Accept 가 완료되기 전에 다른 연결에 대한 Accept 를 할 수 없다.
	// AcceptEx 는 AcceptEx 당 하나의 소켓을 미리 만들고, Accept 요청이 있을 때에 만들어둔 소켓을 활용하는 방식이다.
	// 이로 인해 WSAAceept 와는 다르게 갑자기 접속이 몰리는 경우에도 대비를 할 수 있다.
	// 또한 WSAAccept 는 연결을 허락하고, 초기 받기를 한 번 시도하여 상태를 마무리하는데, AcceptEx 는 이를 한 번에 진행 할 수 있다.
	// 여기서는 크기를 0으로 해서 초기 받기를 진행하지 않도록 하였다.
	if (!AcceptEx(listenSocket,
		mSocket,
		mReadBuffer,				// 버퍼는 정의했지만, 아래에서 크기를 0으로 해서 버퍼링하지 않는다.
		0,							// 크기를 0으로 해서 데이터를 받지 않게 하였다.
		sizeof(sockaddr_in) + 16,
		sizeof(sockaddr_in) + 16,
		NULL,
		&mAcceptOverlapped.Overlapped)) 
	{
		if (WSAGetLastError() != ERROR_IO_PENDING && WSAGetLastError() != WSAEWOULDBLOCK)
		{
			End();

			return FALSE;
		}
	}

	return TRUE;
}

/// <summary>
/// IOCP를 사용할 때에 초기 받기 과정을 처리하는 함수.
/// </summary>
/// <param name=""></param>
/// <returns></returns>
BOOL CNetworkSession::InitializeReadForIocp(VOID)
{
	CThreadSync Sync;

	if (!mSocket)
		return FALSE;

	WSABUF	WsaBuf;								// buf 와 len 으로 구성되어 있다.
	DWORD	ReadBytes	= 0;					// WSARecv 에서 받아온 데이터 수가 저장된다.
	DWORD	ReadFlag	= 0;					// WSARecv 작동 방식을 바꾸는 flag
	
	WsaBuf.buf			= (CHAR*)mReadBuffer;	// 멤버 리드 버퍼 할당
	WsaBuf.len			= MAX_BUFFER_LENGTH;	// 최대 버퍼 길이 할당	

	// TODO: dwBufferCount 가 WSABUF 구조체 갯수를 가리키는데, 이게 다중 버퍼를 활용한 Scatter/Gather 라는 기술을 위한 것이라고 한다. 추후 조사.
	/*int WSAAPI WSARecv(
		SOCKET                             s,						// 데이터를 받아올 소켓 핸들.			
		LPWSABUF                           lpBuffers,				// WSABUF 구조체를 가리키는 포인터.
		DWORD                              dwBufferCount,			// lpBuffers 가 가리키는 WSABUF 구조체 갯수
		LPDWORD                            lpNumberOfBytesRecvd,	// 소켓에서 읽어온 데이터 크기
		LPDWORD                            lpFlags,					// WSARecv 작동 방식을 바꾸는 flag 정보를 가리키는 포인터
		LPWSAOVERLAPPED                    lpOverlapped,			// Overlapped 구조체를 가리키는 포인터
		LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine		// completion routine 과 관련된 정보를 가리키는 포인터, Callback 방식의 소켓 이벤트를 구성할 때에 사용하나독 한다.
	);*/

	INT	ReturnValue = WSARecv(mSocket,
		&WsaBuf,
		1,			// WSABUF 를 이용할 경우 1 이상의 숫자를 설정한다.
		&ReadBytes,
		&ReadFlag,
		&mReadOverlapped.Overlapped,
		NULL);

	// SOCKET_ERROR 이지만 WSA_IO_PENDING(소켓 I/O가 아직 진행중) 이거나 WSAEWOULDBLOCK(아직 작업 진행 중) 이면 정상 진행
	if (ReturnValue == SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING && WSAGetLastError() != WSAEWOULDBLOCK)
	{
		End();

		return FALSE;
	}

	return TRUE;
}

/// <summary>
/// WSARecv 를 통해 받아온 데이터가 들어있는 mReadBuffer 에서 데이터를 복사해서 읽어온다. 
/// 이후 읽어온 데이터를 바탕으로 어떤 작업을 해야 할지 판단한다. 이란 과정이 EventSelect 의 Read 와 다르다.
/// </summary>
/// <param name="data"></param>
/// <param name="dataLength"></param>
/// <returns></returns>
BOOL CNetworkSession::ReadForIocp(BYTE* data, DWORD& dataLength)
{
	CThreadSync Sync;

	if (!mSocket)
		return FALSE;

	if (!data || dataLength <= 0)
		return FALSE;

	memcpy(data, mReadBuffer, dataLength);

	return TRUE;
}

/// <summary>
/// IOCP 가 아닌, 이벤트 방식에서 사용할 Read 함수.
/// </summary>
/// <param name="data"></param>
/// <param name="dataLength"></param>
/// <returns></returns>
BOOL CNetworkSession::ReadForEventSelect(BYTE* data, DWORD& dataLength)
{
	CThreadSync Sync;

	if (!mSocket)
		return FALSE;

	if (!data)
		return FALSE;

	// TODO: 왜 똑같은 부분을 2개 넣어 놓으셨을까?
	if (!mSocket)
		return FALSE;

	WSABUF	WsaBuf;
	DWORD	ReadBytes	= 0;		// IOCP 에서는 안썼지만, 여기서는 데이터를 읽어올 때에 사용한다.
	DWORD	ReadFlag	= 0;
	
	WsaBuf.buf			= (CHAR*)mReadBuffer;
	WsaBuf.len			= MAX_BUFFER_LENGTH;
	
	// EventSelect 방식을 사용할 때에는 WSARecv 가 IOCP일 때와 다르다.
	INT		ReturnValue = WSARecv(mSocket,
		&WsaBuf,
		1,
		&ReadBytes,
		&ReadFlag,
		&mReadOverlapped.Overlapped,
		NULL);
	
	if (ReturnValue == SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING && WSAGetLastError() != WSAEWOULDBLOCK)
	{
		End();

		return FALSE;
	}

	// IOCP 와 다르게 데이터가 도착했다는 신호를 받으면 여기서 바로 받은 내용을 복사한다.
	memcpy(data, mReadBuffer, ReadBytes);
	dataLength = ReadBytes;

	return TRUE;
}

/// <summary>
/// data 에 있는 데이터를 WSASend 로 보낸다. 소켓 버퍼에 보낼 데이터의 포인터를 쓰기에 write 라고 표현하신 것 같다.
/// Write 할 때의 데이터 포인터를 WSASend 가 끝날 때까지 살려놓아야 하는데, 이 포인터를 큐에 넣어놓고 완료되면 삭제하는 식으로 이것을 진행한다.
/// PacketSession 클래스에서 위의 과정을 처리한다.
/// </summary>
/// <param name="data"></param>
/// <param name="dataLength"></param>
/// <returns></returns>
BOOL CNetworkSession::Write(BYTE* data, DWORD dataLength)
{
	CThreadSync Sync;

	if (!mSocket)
		return FALSE;

	if (!data || dataLength <= 0)
		return FALSE;

	WSABUF	WsaBuf;
	DWORD	WriteBytes	= 0;
	DWORD	WriteFlag	= 0;

	WsaBuf.buf			= (CHAR*)data;			// 데이터를 보낼 포인터 입력.
	WsaBuf.len			= dataLength;

	// WSARecv 와 비슷하다. Scatter/Gather 를 위해 멀티 버퍼를 사용하는 점까지 비슷하다. 그러므로 자세한 설명은 생략.
	INT		ReturnValue = WSASend(mSocket,
		&WsaBuf,
		1,
		&WriteBytes,
		WriteFlag,
		&mWriteOverlapped.Overlapped,
		NULL);

	if (ReturnValue == SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING && WSAGetLastError() != WSAEWOULDBLOCK)
	{
		End();

		return FALSE;
	}

	return TRUE;
}

/// <summary>
/// WSAConnect 를 통해 접속하는 함수. 서버 간의 통신에서도 이용할 수 있다.
/// </summary>
/// <param name="address"></param>
/// <param name="port"></param>
/// <returns></returns>
BOOL CNetworkSession::Connect(LPSTR address, USHORT port)
{
	CThreadSync Sync;				// 동기화 용도

	if (!address || port <= 0)
		return FALSE;

	if (!mSocket)
		return FALSE;

	SOCKADDR_IN RemoteAddressInfo;	// 접속할 IP 주소 정보

	RemoteAddressInfo.sin_family			= AF_INET;
	RemoteAddressInfo.sin_port				= htons(port);	// host to network short, Big-endian 으로 변경 ex) 0x1234 의 경우 12, 34 이런 순서로 주소값을 기준으로 바이트 정렬된다.
	RemoteAddressInfo.sin_addr.S_un.S_addr	= inet_addr(address); // 문자열 주소를 IP 형식에 맞는 ulong 으로 변경한다. ex) "127.0.0.1" 이란 문자열이 0x7f 00 00 01 이런 식으로

	if (WSAConnect(mSocket, (LPSOCKADDR)&RemoteAddressInfo, sizeof(SOCKADDR_IN), NULL, NULL, NULL, NULL) == SOCKET_ERROR)
	{	
		// WSAConnect 는 일반 Connect 보다 복잡한 옵션을 넣을 수 있다.
		/*int WSAAPI WSAConnect(
			SOCKET         s,				// 접속하기 위해 생성된 소캣 핸들
			const sockaddr * name,			// 접속하기 위한 주소 정보
			int            namelen,			// 접속하기 위한 정보 길이, 이게 IPv6 인 경우 정보 크기 차이가 있어서 그렇다.
			LPWSABUF       lpCallerData,	// TODO: 차후에 MSDN remark 자세히 읽어보기
			LPWSABUF       lpCalleeData,	// TODO: 차후에 MSDN remark 자세히 읽어보기
			LPQOS          lpSQOS,			// QOS = Quality Of Service, 서비스 품질을 의미한다. TODO: 차후에 MSDN remark 자세히 읽어보기 
			LPQOS          lpGQOS			// 미래에 사용 예정, 무조건 NULL 이어야 한다.
		);*/

		// 연결 실패지만 대기상태(WOULDBLOCK)이면 성공으로 판단.
		if (WSAGetLastError() != WSAEWOULDBLOCK)
		{
			End();

			return FALSE;
		}
	}

	return TRUE;
}

/// <summary>
/// UDP 를 사용하기 위한 bind 함수.
/// </summary>
/// <param name="port"></param>
/// <returns></returns>
BOOL CNetworkSession::UdpBind(USHORT port)
{
	CThreadSync Sync;

	if (mSocket)
		return FALSE;

	SOCKADDR_IN RemoteAddressInfo;

	RemoteAddressInfo.sin_family			= AF_INET;
	RemoteAddressInfo.sin_port				= htons(port);
	RemoteAddressInfo.sin_addr.S_un.S_addr	= htonl(INADDR_ANY);

	// UDP 용 소켓 생성, IPPROTO_UDP 그리고 SOCK_DGRAM 부분이 TCP 와 다른 것을 알 수 있다.
	mSocket = WSASocket(AF_INET, SOCK_DGRAM, IPPROTO_UDP, NULL, 0, WSA_FLAG_OVERLAPPED);

	if (mSocket == INVALID_SOCKET)
		return FALSE;

	// 설정된 주소로 Bind 한다. 앞으로 Bind 된 주소로 패킷을 보낼 것이다.
	if (bind(mSocket, (struct sockaddr*)&RemoteAddressInfo, sizeof(SOCKADDR_IN)) == SOCKET_ERROR)
	{
		End();

		return FALSE;
	}

	// 종료 Event 오브젝트 생성 및 할당
	mReliableUdpThreadDestroyEvent = CreateEvent(0, FALSE, FALSE, 0);
	if (mReliableUdpThreadDestroyEvent == NULL)
	{
		End();

		return FALSE;
	}

	// 시작 Event 오브젝트 생성 및 할당
	mReliableUdpThreadStartupEvent = CreateEvent(0, FALSE, FALSE, 0);
	if (mReliableUdpThreadStartupEvent == NULL)
	{
		End();

		return FALSE;
	}

	// TODO: 이게 큐 IOCP 대기열에 있다가 깨어나는 것을 말하는 건가? 확인
	// WakeUp Event 오브젝트 생성 및 할당
	mReliableUdpThreadWakeUpEvent = CreateEvent(0, FALSE, FALSE, 0);
	if (mReliableUdpThreadWakeUpEvent == NULL)
	{
		End();

		return FALSE;
	}

	// Write Event 오브젝트를 생성해서 할당
	mReliableUdpWriteCompleteEvent = CreateEvent(0, FALSE, FALSE, 0);
	if (mReliableUdpWriteCompleteEvent == NULL)
	{
		End();

		return FALSE;
	}

	// Reliable UDP 에서 사용할 큐를 초기화
	if (!mReliableWriteQueue.Begin())
	{
		End();

		return FALSE;
	}

	// Reliable UDP 용 스레드 생성
	DWORD ReliableUdpThreadID = 0;	// ThreadID 받아오는 변수
	mReliableUdpThreadHandle = CreateThread(NULL, 0, ::ReliableUdpThreadCallback, this, 0, &ReliableUdpThreadID);

	// Startup 이벤트 객체 signal 될 때까지 무한 대기 (Event 객체가 Signal 할 때까지 대기) 
	// Signal 은 특정 이벤트가 발생했다는 것을 알리는 것이다. 인터럽트라고 부르기도 한다.
	// 즉 여기서는 스레드가 생성되고 콜백에서 StartupEvent 를 SetEvent 함수로 singal 시키면 return TRUE 하는 것이다.
	WaitForSingleObject(mReliableUdpThreadStartupEvent, INFINITE);

	return TRUE;
}

BOOL CNetworkSession::TcpBind(VOID)
{
	CThreadSync Sync;

	if (mSocket)
		return FALSE;

	mSocket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);

	if (mSocket == INVALID_SOCKET)
		return FALSE;

	return TRUE;
}

BOOL CNetworkSession::GetLocalIP(WCHAR* pIP)
{
	CThreadSync Sync;

	if (!mSocket)
		return FALSE;

	CHAR	Name[256] = { 0, };
	gethostname(Name, sizeof(Name));

	PHOSTENT host = gethostbyname(Name);
	if (host)
	{
		if (MultiByteToWideChar(CP_ACP, 0, inet_ntoa(*(struct in_addr*)*host->h_addr_list), -1, pIP, 32) > 0)
			return TRUE;
	}

	return FALSE;
}

USHORT CNetworkSession::GetLocalPort(VOID)
{
	CThreadSync Sync;

	if (!mSocket)
		return 0;

	SOCKADDR_IN Addr;
	ZeroMemory(&Addr, sizeof(Addr));
	
	INT AddrLength = sizeof(Addr);
	if (getsockname(mSocket, (sockaddr*)&Addr, &AddrLength) != SOCKET_ERROR)
		return ntohs(Addr.sin_port);

	return 0;
}

/// <summary>
/// IOCP 를 사용하는 UDP 초기 받기 함수. 
/// </summary>
/// <param name=""></param>
/// <returns></returns>
BOOL CNetworkSession::InitializeReadFromForIocp(VOID)
{
	CThreadSync Sync;

	if (!mSocket)
		return FALSE;

	WSABUF	WsaBuf;
	DWORD	ReadBytes				= 0;
	DWORD	ReadFlag				= 0;
	INT		RemoteAddressInfoSize	= sizeof(mUdpRemoteInfo);

	WsaBuf.buf	= (CHAR*)mReadBuffer;
	WsaBuf.len	= MAX_BUFFER_LENGTH;

	/*int WSAAPI WSARecvFrom(
		SOCKET                             s,
		LPWSABUF                           lpBuffers,				// 받은 데이터를 저장할 버퍼
		DWORD                              dwBufferCount,			// 원형 버퍼를 사용할 때 사용하는 버퍼 개수
		LPDWORD                            lpNumberOfBytesRecvd,	// 동시 소켓에서 받은 데이터 크기
		LPDWORD                            lpFlags,
		sockaddr * lpFrom,											// 데이터를 받은 상대의 주소 정보, WSARecv 와 차이점.
		LPINT                              lpFromlen,				// 상대 주소 정보의 길이, WSARecv 와 차이점.
		LPWSAOVERLAPPED                    lpOverlapped,			
		LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine		// Callback 방식의 소켓 이벤트 방식을 구성할 때 사용
	);*/

	INT		ReturnValue = WSARecvFrom(mSocket,
		&WsaBuf,
		1,
		&ReadBytes,
		&ReadFlag,
		(SOCKADDR*)&mUdpRemoteInfo,		// WSARecv 와 다른 파라미터다. 데이터를 보낸 상대의 주소 정보. 이것을 따로 저장하는 것이 TCP 와 다르다.
		&RemoteAddressInfoSize,			// WSARecv 와 다른 파라미터다. 주소 정보 길이 
										// TCP 같은 경우는 listen 하다가 Accept 하고 나서 연결되는 포트 따로 생기고 관리되는데, 
										// UDP 는 그런 거 없어서 이 정보를 저장해놓고 여기로 바로 통신하는 것이다.
		&mReadOverlapped.Overlapped,
		NULL);

	if (ReturnValue == SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING && WSAGetLastError() != WSAEWOULDBLOCK)
	{
		End();
	
		return FALSE;
	}

	return TRUE;
}

/// <summary>
/// IOCP 사용하는 UDP 에서 실제로 받은 데이터를 확인하는 함수.
/// </summary>
/// <param name="remoteAddress"></param>
/// <param name="remotePort"></param>
/// <param name="data"></param>
/// <param name="dataLength"></param>
/// <returns></returns>
BOOL CNetworkSession::ReadFromForIocp(LPSTR remoteAddress, USHORT& remotePort, BYTE* data, DWORD& dataLength)
{
	CThreadSync Sync;

	if (!mSocket)
		return FALSE;

	if (!data || dataLength <= 0)
		return FALSE;

	// WSARecvFrom 에서 받아온 데이터를 data 에 복사.
	memcpy(data, mReadBuffer, dataLength);

	// 데이터를 보낸 IP 주소 확인
	strcpy(remoteAddress, inet_ntoa(mUdpRemoteInfo.sin_addr));
	// 데이터를 보낸 포트 확인
	remotePort = ntohs(mUdpRemoteInfo.sin_port);

	USHORT Ack = 0;
	// mReadBuffer 에서 맨 앞 2바이트를 복사해서 확인
	memcpy(&Ack, mReadBuffer, sizeof(USHORT));

	if (Ack == 9999) // 앞의 2바이트가 9999일 경우 데이터를 잘 받아서 보내는 '응답(Ack) 패킷'으로 인식
	{
		// 응답 패킷을 받았으므로 Complete 이벤트를 통해 다음 데이터를 전송
		SetEvent(mReliableUdpWriteCompleteEvent);

		return FALSE;
	}
	else
	{
		// 새로운 데이터를 내가 받은 경우
		Ack = 9999; 

		// 데이터를 보낸 주소로 Ack 2바이트를 9999로 전송, 상대가 데이터 보냈으니 잘 받았다고 알리는 것이다.
		WriteTo2(remoteAddress, remotePort, (BYTE*)&Ack, sizeof(USHORT));
	}

	return TRUE;
}

/// <summary>
/// EventSelect 방식을 사용할 때의 ReadFrom 함수. WSARecvFrom 을 사용한다.
/// </summary>
/// <param name="remoteAddress"></param>
/// <param name="remotePort"></param>
/// <param name="data"></param>
/// <param name="dataLength"></param>
/// <returns></returns>
BOOL CNetworkSession::ReadFromForEventSelect(LPSTR remoteAddress, USHORT& remotePort, BYTE* data, DWORD& dataLength)
{
	CThreadSync	Sync;

	if (!mSocket)
		return FALSE;
	
	if (!data)
		return FALSE;

	if (!mSocket) // TODO: 이게 똑같은 mSocket 체크를 2번 씩 하시는데.. 의도된 것인가? 다른 것도 이러던데.. 실수 아닌지 확인해보자.
		return FALSE;

	WSABUF	WsaBuf;
	DWORD	ReadBytes				= 0;
	DWORD	ReadFlag				= 0;
	INT		RemoteAddressInfoSize	= sizeof(mUdpRemoteInfo);

	WsaBuf.buf = (CHAR*)mReadBuffer;
	WsaBuf.len = MAX_BUFFER_LENGTH;

	INT ReturnValue = WSARecvFrom(mSocket,
		&WsaBuf,
		1,
		&ReadBytes,
		&ReadFlag,
		(SOCKADDR*)&mUdpRemoteInfo,
		&RemoteAddressInfoSize,
		&mReadOverlapped.Overlapped,
		NULL);

	if (ReturnValue == SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING && WSAGetLastError() != WSAEWOULDBLOCK)
	{
		End();

		return FALSE;
	}

	// IOCP 와 다르게 바로 결과가 나온다. 그래서 데이터를 받아 확인한다.
	memcpy(data, mReadBuffer, ReadBytes);	// CopyMemory 랑 같다.
	dataLength = ReadBytes;

	// 주소 정보를 복사한다.
	// TODO: 주소가 바뀔 일이 있나? 왜 매번 복사할까..
	strcpy(remoteAddress, inet_ntoa(mUdpRemoteInfo.sin_addr));
	remotePort = ntohs(mUdpRemoteInfo.sin_port);

	// IOCP 와 같은 Reliable UDP 를 보장해주는 코드이다.
	USHORT Ack = 0;
	memcpy(&Ack, mReadBuffer, sizeof(USHORT));

	// IOCP 의 ReadFrom과 동일하게 9999에 대한 처리를 한다.
	if (Ack == 9999)
	{
		SetEvent(mReliableUdpWriteCompleteEvent);

		return FALSE;
	}
	else
	{
		Ack = 9999;
		WriteTo2(remoteAddress, remotePort, (BYTE*)&Ack, sizeof(USHORT));
	}

	return TRUE;
}

/// <summary>
/// 사용자(Client)들이 호출하는 WriteTo 함수. 데이터를 큐에 넣어 놓는 역할을 한다.
/// </summary>
/// <param name="remoteAddress"></param>
/// <param name="remotePort"></param>
/// <param name="data"></param>
/// <param name="dataLength"></param>
/// <returns></returns>
BOOL CNetworkSession::WriteTo(LPCSTR remoteAddress, USHORT remotePort, BYTE* data, DWORD dataLength)
{
	CThreadSync Sync;

	if (!mSocket)
		return FALSE;

	if (!remoteAddress || remotePort <= 0 || !data || dataLength <= 0)
		return FALSE;

	// 큐에 데이터 입력
	if (!mReliableWriteQueue.Push(this, data, dataLength, remoteAddress, remotePort))
		return FALSE;

	// 현재 보내는 데이터가 없는 경우
	if (!mIsReliableUdpSending)
	{
		// 보내는 데이터가 있다고 플래그를 바꾸고, ReliableUDPThread 를 깨운다.
		mIsReliableUdpSending = TRUE;
		SetEvent(mReliableUdpThreadWakeUpEvent);
	}

	return TRUE;
}

/// <summary>
/// 실제로 WSAsendTo 를 써서 UDP 로 데이터를 보내는 함수이다.
/// </summary>
/// <param name="remoteAddress"></param>
/// <param name="remotePort"></param>
/// <param name="data"></param>
/// <param name="dataLength"></param>
/// <returns></returns>
BOOL CNetworkSession::WriteTo2(LPSTR remoteAddress, USHORT remotePort, BYTE* data, DWORD dataLength)
{
	CThreadSync Sync;

	if (!mSocket)
		return FALSE;

	if (!remoteAddress || remotePort <= 0 || !data || dataLength <= 0)
		return FALSE;

	WSABUF		WsaBuf;
	DWORD		WriteBytes				= 0;
	DWORD		WriteFlag				= 0;

	SOCKADDR_IN RemoteAddressInfo;
	INT			RemoteAddressInfoSize	= sizeof(RemoteAddressInfo);
	
	WsaBuf.buf							= (CHAR*)data;
	WsaBuf.len							= dataLength;

	// 데이터를 보낼 주소 입력
	RemoteAddressInfo.sin_family			= AF_INET;
	RemoteAddressInfo.sin_addr.S_un.S_addr	= inet_addr(remoteAddress);
	RemoteAddressInfo.sin_port				= htons(remotePort);

	// WSASendTo 함수를 이용해서 데이터 전송
	INT			ReturnValue = WSASendTo(mSocket,
		&WsaBuf,
		1,
		&WriteBytes,
		WriteFlag,
		(SOCKADDR*)&RemoteAddressInfo,		// 보낼 주소 정보
		RemoteAddressInfoSize,				// 보낼 주소 정보 길이, UDP 라서 이 패러미터를 쓴다.
		&mWriteOverlapped.Overlapped,
		NULL);

	if (ReturnValue == SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING && WSAGetLastError() != WSAEWOULDBLOCK)
	{
		End();

		return FALSE;
	}

	return TRUE;
}

BOOL CNetworkSession::GetRemoteAddressAfterAccept(LPTSTR remoteAddress, USHORT& remotePort)
{
	CThreadSync Sync;

	if (!remoteAddress)
		return FALSE;

	sockaddr_in*	Local			= NULL;
	INT				LocalLength		= 0;

	sockaddr_in*	Remote			= NULL;
	INT				RemoteLength	= 0;

	GetAcceptExSockaddrs(mReadBuffer,
		0,
		sizeof(sockaddr_in) + 16,
		sizeof(sockaddr_in) + 16,
		(sockaddr**)&Local,
		&LocalLength,
		(sockaddr**)&Remote,
		&RemoteLength);

	CHAR TempRemoteAddress[32] = { 0, };
	strcpy(TempRemoteAddress, inet_ntoa(Remote->sin_addr));

	MultiByteToWideChar(CP_ACP,
		0,
		TempRemoteAddress,
		-1,
		remoteAddress,
		32);

	remotePort = ntohs(Remote->sin_port);

	return TRUE;
}

SOCKET CNetworkSession::GetSocket(VOID)
{
	CThreadSync Sync;

	return mSocket;
}