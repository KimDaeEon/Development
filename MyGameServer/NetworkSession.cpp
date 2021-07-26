#include "NetworkSession.h"
// WaitForSingleObject = time-out �ð��� ���������� ��ٸ��ų�, � ������Ʈ�� signaled �� ������ ��ٸ���. (blocking), timeout ª�� �ϸ� ������ ������ ���� �����Ѵ�.
// WAIT_OBJECT_0 �ش� �̺�Ʈ ������Ʈ�� signaled ���°� �Ǿ����� �˸���.
// WAIT_TIMEOUT �ش� �̺�Ʈ ������Ʈ ���°� nonsignaled �� ���·� time-out �Ǿ�������� �˸���.
// WAIT_FAILED �ش� function ��ü�� ����, ������ �˱� ���ؼ� GetLastError ȣ���� �ʿ��ϴ�.

// SetEvent = Event Object �� ���¸� Signaled ���·� ��ȯ
// CreateEvent = EVent Object Handle �� ���� 
// CreateThread = Thread ����, �ֱٿ��� _begintreadex �� �� ����ȴٰ� �Ѵ�.���� �ð��Ǹ� �� �����غ� �� ����.https://killsia.tistory.com/entry/CreateThread-beginthread-beginthreadex%EC%9D%98-%EC%B0%A8%EC%9D%B4

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
	mReliableUdpThreadHandle		= NULL;			// ���� ��Ŷ�� ���� ������ ��� �������ִ� ReliableUDP �� ����ϴ� Thread �ڵ�.
	mReliableUdpThreadStartupEvent	= NULL;			// ReliableUDP�� ����ϴ� Thread�� ������ �˸��� �̺�Ʈ.
	mReliableUdpThreadDestroyEvent	= NULL;			// ReliableUDP�� ����ϴ� Thread�� ���Ḧ �˸��� �̺�Ʈ.
	mReliableUdpThreadWakeUpEvent	= NULL;			// ReliableUDP�� ����ϴ� Thread ���� ���� ����ϴ� �̺�Ʈ.
	mReliableUdpWriteCompleteEvent	= NULL;			// ��밡 ��Ŷ�� �޾Ƽ� �� �̻� ���� �ʿ䰡 ���� ���� ����ϴ� �̺�Ʈ.

	mIsReliableUdpSending			= FALSE;		// ���� Reliable �ϰ� ������ �ִ� data �� �ִ��� Ȯ��.

	mAcceptOverlapped.IoType		= IO_ACCEPT;	// ���⼭ �� ���� �־� ���Ƽ� ���߿� IOCP ���� � IO ������ �Ǵ��Ѵ�.
	mReadOverlapped.IoType			= IO_READ;		// ���⼭ �� ���� �־� ���Ƽ� ���߿� IOCP ���� � IO ������ �Ǵ��Ѵ�.
	mWriteOverlapped.IoType			= IO_WRITE;		// ���⼭ �� ���� �־� ���Ƽ� ���߿� IOCP ���� � IO ������ �Ǵ��Ѵ�.

	mAcceptOverlapped.Object		= this;
	mReadOverlapped.Object			= this;
	mWriteOverlapped.Object			= this;
}

CNetworkSession::~CNetworkSession(VOID) 
{

}

VOID CNetworkSession::ReliableUdpThreadCallback(VOID)
{
	// �̷� ���·� ���� �ݹ� �Լ����� Ŭ���� ������ �����ϴ� ���� ���� ���� �� �������� ���α׷��ֿ� ���ٰ� �ϼ̴�. ��������.
	DWORD	EventID						= 0;			// ReliableUDPThread �� ���۰� ���Ḧ ����ϴ� �̺�Ʈ ID
	HANDLE	ThreadEvents[2]				= { mReliableUdpThreadDestroyEvent, mReliableUdpThreadWakeUpEvent };	// ����, ���� �̺�Ʈ

	CHAR	RemoteAddress[32]			= { 0, };		// ������ ���� IP �ּ�
	USHORT	RemotePort					= 0;			// ������ ���� �ּ��� ��Ʈ
	BYTE	Data[MAX_BUFFER_LENGTH]		= { 0, };		// ���� ������ ��� �迭
	DWORD	DataLength					= 0;			// ���� ������ ����
	VOID*	Object						= NULL;			// ������Ʈ, ���⼭�� ���� �ʴ´�.
	
	while (TRUE)
	{
		SetEvent(mReliableUdpThreadStartupEvent);		// ReliableUDPThread �� �����ߴٴ� ���� �˸��� �̺�Ʈ. UdpBind���� WairForSingleObject �� ������.

		// �̺�Ʈ���� �߻��� ������ ���� ���
		EventID = WaitForMultipleObjects(2, ThreadEvents, FALSE, INFINITE);

		switch (EventID)
		{
			// DestroyEvent �߻��� ��� ����.
		case WAIT_OBJECT_0:
			return;
		case WAIT_OBJECT_0 + 1:
NEXT_DATA:
			// Write �� ��� 1���� ���� �����͸� ť���� ������.
			if (mReliableWriteQueue.Pop(&Object, Data, DataLength, RemoteAddress, RemotePort))
			{
RETRY:
				// WriteTo2 �� WSASendTo �� ���� �����͸� ������. WriteTo �� ���� �����͸� ť�� �ִ´�.
				if (!WriteTo2(RemoteAddress, RemotePort, Data, DataLength))
					return;

				// �����Ͱ� ���� ��� ������ ó�� �����͸� ������, ��ٸ���. ���� ������ SetEvent ȣ���ϸ� ���⸦ �ٷ� �Ѿ��.
				// SetEvent ȣ�� ���ص� 10 �и� ������(1000���� 1��)��ŭ�� ��ٸ��ڴ� ��. �� 0.01�ʸ� ��ٸ���, �̸� ������ ���� ���з� �����ϰ� �������Ѵ�.
				DWORD result = WaitForSingleObject(mReliableUdpWriteCompleteEvent, 10);

				if (result == WAIT_OBJECT_0)  // �ش� �̺�Ʈ�� singaled ����, �� �����忡 ���ؼ� ���� �� �ִ� �����̸� �Ѿ��.
					goto NEXT_DATA;
				else  // �ش� �ð��ȿ� ó���� �ȵǸ� ��õ�
					// TODO: ������ ���� �̻� Retry �� �ϸ� ������ �սǷ� �����ϴ� �κ��� �����ؾ� �Ѵ�.
					goto RETRY;
			}
			else
				// WriteTo �Լ����� ���� ������ �����Ͱ� �ִ��� ������ Ȯ���ϱ� ���� FLAG ����
				mIsReliableUdpSending = FALSE;	// ���� ť�� �� �̻� ���� �����Ͱ� �����Ƿ� FALSE �� ����

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

	if (mReliableUdpThreadHandle)	// Reliable UDP ���� ���� ���
	{
		SetEvent(mReliableUdpThreadDestroyEvent);	// ���� �̺�Ʈ ȣ��

		WaitForSingleObject(mReliableUdpThreadHandle, INFINITE);	// �̺�Ʈ ���� ��ٸ���.

		CloseHandle(mReliableUdpThreadHandle);		// �̺�Ʈ ���� �� �ڵ� �ݴ´�.
	}

	// ���� �̺�Ʈ�� ��� ����
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
/// Accept ������ ó���ϴ� �Լ�.
/// </summary>
/// <param name="listenSocket"></param>
/// <returns></returns>
BOOL CNetworkSession::Accept(SOCKET listenSocket)
{
	CThreadSync Sync;

	if (!listenSocket)	// Listen �ǰ� ���� ������ ������� �ʴ´�.
		return FALSE;

	if (mSocket)
		return FALSE;

	mSocket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED); // WSA = Windows Sockets API
	/*SOCKET WSAAPI WSASocketW(
		int                 af,					// Address Family, AF_INET �� IPv4 �ּҸ� ����Ѵٴ� �ǹ�.
		int                 type,				// SOCK_STREAM �� TCP ���� ���Ǵ� ���� ����̴�.
		int                 protocol,			// �׷��� ���� IPPROTO_TCP �� �´�.
		LPWSAPROTOCOL_INFOW lpProtocolInfo,		// TODO: ���Ŀ� Ȯ��, NULL �� �ƴϸ� ���� �Էµ� WSAPROTOCOL_INFO �� ���δٰ� �Ѵ�.
		GROUP               g,					// TODO: ���� Ȯ��, socket group id, 0 �̸� group operation �� �������� �ʴ´ٰ� �Ѵ�.
		DWORD               dwFlags				// ��Ĺ Ư���� �����ϴ� flag, ���⼭�� Overlapped I/O �� �����ϴ� ������ ����.
	);*/

	if (mSocket == INVALID_SOCKET)
	{
		End();

		return FALSE;
	}

	//TODO: �Ʒ� ���� ���� �ڵ�� �� �м��� ���� �ٽ� Ȯ��. 
	// BOOL NoDelay = TRUE; 
	//setsockopt(mSocket, IPPROTO_TCP, TCP_NODELAY, (const char FAR *)&NoDelay, sizeof(NoDelay));
	// ���� �ɼ��� setsockopt �� �����ϰ�, getsockopt �� �����ͼ� Ȯ���Ѵ�.

	/*BOOL AcceptEx(
		SOCKET       sListenSocket,				// listen �ǰ� �ִ� ����
		SOCKET       sAcceptSocket,				// aceept ���� ����
		PVOID        lpOutputBuffer,			// accept �� read �� ���� ����� ���� �ּ� 
		DWORD        dwReceiveDataLength,		// read �� �� ����� ������ ũ�� (0�� ��� read ���� �ʴ´�)
		DWORD        dwLocalAddressLength,		// ���� �ּ� ���� ����. ����ϴ� ���� �������� �ּ� �ִ� ���� ���� 16����Ʈ �� Ŀ���Ѵ�.
		DWORD        dwRemoteAddressLength,		// ����Ʈ �ּ� ���� ����. ����ϴ� ���� �������� �ּ� �ִ� ���� ���� 16����Ʈ �� Ŀ���Ѵ�.
		LPDWORD      lpdwBytesReceived,			// ���� ����Ʈ ũ��, (Read �� ũ��)
		LPOVERLAPPED lpOverlapped				// accept ���� ����� overlapped ����ü
	);*/
	// while ���� ����� WSAAccept �� �ϳ��� Accept �� �Ϸ�Ǳ� ���� �ٸ� ���ῡ ���� Accept �� �� �� ����.
	// AcceptEx �� AcceptEx �� �ϳ��� ������ �̸� �����, Accept ��û�� ���� ���� ������ ������ Ȱ���ϴ� ����̴�.
	// �̷� ���� WSAAceept �ʹ� �ٸ��� ���ڱ� ������ ������ ��쿡�� ��� �� �� �ִ�.
	// ���� WSAAccept �� ������ ����ϰ�, �ʱ� �ޱ⸦ �� �� �õ��Ͽ� ���¸� �������ϴµ�, AcceptEx �� �̸� �� ���� ���� �� �� �ִ�.
	// ���⼭�� ũ�⸦ 0���� �ؼ� �ʱ� �ޱ⸦ �������� �ʵ��� �Ͽ���.
	if (!AcceptEx(listenSocket,
		mSocket,
		mReadBuffer,				// ���۴� ����������, �Ʒ����� ũ�⸦ 0���� �ؼ� ���۸����� �ʴ´�.
		0,							// ũ�⸦ 0���� �ؼ� �����͸� ���� �ʰ� �Ͽ���.
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
/// IOCP�� ����� ���� �ʱ� �ޱ� ������ ó���ϴ� �Լ�.
/// </summary>
/// <param name=""></param>
/// <returns></returns>
BOOL CNetworkSession::InitializeReadForIocp(VOID)
{
	CThreadSync Sync;

	if (!mSocket)
		return FALSE;

	WSABUF	WsaBuf;								// buf �� len ���� �����Ǿ� �ִ�.
	DWORD	ReadBytes	= 0;					// WSARecv ���� �޾ƿ� ������ ���� ����ȴ�.
	DWORD	ReadFlag	= 0;					// WSARecv �۵� ����� �ٲٴ� flag
	
	WsaBuf.buf			= (CHAR*)mReadBuffer;	// ��� ���� ���� �Ҵ�
	WsaBuf.len			= MAX_BUFFER_LENGTH;	// �ִ� ���� ���� �Ҵ�	

	// TODO: dwBufferCount �� WSABUF ����ü ������ ����Ű�µ�, �̰� ���� ���۸� Ȱ���� Scatter/Gather ��� ����� ���� ���̶�� �Ѵ�. ���� ����.
	/*int WSAAPI WSARecv(
		SOCKET                             s,						// �����͸� �޾ƿ� ���� �ڵ�.			
		LPWSABUF                           lpBuffers,				// WSABUF ����ü�� ����Ű�� ������.
		DWORD                              dwBufferCount,			// lpBuffers �� ����Ű�� WSABUF ����ü ����
		LPDWORD                            lpNumberOfBytesRecvd,	// ���Ͽ��� �о�� ������ ũ��
		LPDWORD                            lpFlags,					// WSARecv �۵� ����� �ٲٴ� flag ������ ����Ű�� ������
		LPWSAOVERLAPPED                    lpOverlapped,			// Overlapped ����ü�� ����Ű�� ������
		LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine		// completion routine �� ���õ� ������ ����Ű�� ������, Callback ����� ���� �̺�Ʈ�� ������ ���� ����ϳ��� �Ѵ�.
	);*/

	INT	ReturnValue = WSARecv(mSocket,
		&WsaBuf,
		1,			// WSABUF �� �̿��� ��� 1 �̻��� ���ڸ� �����Ѵ�.
		&ReadBytes,
		&ReadFlag,
		&mReadOverlapped.Overlapped,
		NULL);

	// SOCKET_ERROR ������ WSA_IO_PENDING(���� I/O�� ���� ������) �̰ų� WSAEWOULDBLOCK(���� �۾� ���� ��) �̸� ���� ����
	if (ReturnValue == SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING && WSAGetLastError() != WSAEWOULDBLOCK)
	{
		End();

		return FALSE;
	}

	return TRUE;
}

/// <summary>
/// WSARecv �� ���� �޾ƿ� �����Ͱ� ����ִ� mReadBuffer ���� �����͸� �����ؼ� �о�´�. 
/// ���� �о�� �����͸� �������� � �۾��� �ؾ� ���� �Ǵ��Ѵ�. �̶� ������ EventSelect �� Read �� �ٸ���.
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
/// IOCP �� �ƴ�, �̺�Ʈ ��Ŀ��� ����� Read �Լ�.
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

	// TODO: �� �Ȱ��� �κ��� 2�� �־� ����������?
	if (!mSocket)
		return FALSE;

	WSABUF	WsaBuf;
	DWORD	ReadBytes	= 0;		// IOCP ������ �Ƚ�����, ���⼭�� �����͸� �о�� ���� ����Ѵ�.
	DWORD	ReadFlag	= 0;
	
	WsaBuf.buf			= (CHAR*)mReadBuffer;
	WsaBuf.len			= MAX_BUFFER_LENGTH;
	
	// EventSelect ����� ����� ������ WSARecv �� IOCP�� ���� �ٸ���.
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

	// IOCP �� �ٸ��� �����Ͱ� �����ߴٴ� ��ȣ�� ������ ���⼭ �ٷ� ���� ������ �����Ѵ�.
	memcpy(data, mReadBuffer, ReadBytes);
	dataLength = ReadBytes;

	return TRUE;
}

/// <summary>
/// data �� �ִ� �����͸� WSASend �� ������. ���� ���ۿ� ���� �������� �����͸� ���⿡ write ��� ǥ���Ͻ� �� ����.
/// Write �� ���� ������ �����͸� WSASend �� ���� ������ ������ƾ� �ϴµ�, �� �����͸� ť�� �־���� �Ϸ�Ǹ� �����ϴ� ������ �̰��� �����Ѵ�.
/// PacketSession Ŭ�������� ���� ������ ó���Ѵ�.
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

	WsaBuf.buf			= (CHAR*)data;			// �����͸� ���� ������ �Է�.
	WsaBuf.len			= dataLength;

	// WSARecv �� ����ϴ�. Scatter/Gather �� ���� ��Ƽ ���۸� ����ϴ� ������ ����ϴ�. �׷��Ƿ� �ڼ��� ������ ����.
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
/// WSAConnect �� ���� �����ϴ� �Լ�. ���� ���� ��ſ����� �̿��� �� �ִ�.
/// </summary>
/// <param name="address"></param>
/// <param name="port"></param>
/// <returns></returns>
BOOL CNetworkSession::Connect(LPSTR address, USHORT port)
{
	CThreadSync Sync;				// ����ȭ �뵵

	if (!address || port <= 0)
		return FALSE;

	if (!mSocket)
		return FALSE;

	SOCKADDR_IN RemoteAddressInfo;	// ������ IP �ּ� ����

	RemoteAddressInfo.sin_family			= AF_INET;
	RemoteAddressInfo.sin_port				= htons(port);	// host to network short, Big-endian ���� ���� ex) 0x1234 �� ��� 12, 34 �̷� ������ �ּҰ��� �������� ����Ʈ ���ĵȴ�.
	RemoteAddressInfo.sin_addr.S_un.S_addr	= inet_addr(address); // ���ڿ� �ּҸ� IP ���Ŀ� �´� ulong ���� �����Ѵ�. ex) "127.0.0.1" �̶� ���ڿ��� 0x7f 00 00 01 �̷� ������

	if (WSAConnect(mSocket, (LPSOCKADDR)&RemoteAddressInfo, sizeof(SOCKADDR_IN), NULL, NULL, NULL, NULL) == SOCKET_ERROR)
	{	
		// WSAConnect �� �Ϲ� Connect ���� ������ �ɼ��� ���� �� �ִ�.
		/*int WSAAPI WSAConnect(
			SOCKET         s,				// �����ϱ� ���� ������ ��Ĺ �ڵ�
			const sockaddr * name,			// �����ϱ� ���� �ּ� ����
			int            namelen,			// �����ϱ� ���� ���� ����, �̰� IPv6 �� ��� ���� ũ�� ���̰� �־ �׷���.
			LPWSABUF       lpCallerData,	// TODO: ���Ŀ� MSDN remark �ڼ��� �о��
			LPWSABUF       lpCalleeData,	// TODO: ���Ŀ� MSDN remark �ڼ��� �о��
			LPQOS          lpSQOS,			// QOS = Quality Of Service, ���� ǰ���� �ǹ��Ѵ�. TODO: ���Ŀ� MSDN remark �ڼ��� �о�� 
			LPQOS          lpGQOS			// �̷��� ��� ����, ������ NULL �̾�� �Ѵ�.
		);*/

		// ���� �������� ������(WOULDBLOCK)�̸� �������� �Ǵ�.
		if (WSAGetLastError() != WSAEWOULDBLOCK)
		{
			End();

			return FALSE;
		}
	}

	return TRUE;
}

/// <summary>
/// UDP �� ����ϱ� ���� bind �Լ�.
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

	// UDP �� ���� ����, IPPROTO_UDP �׸��� SOCK_DGRAM �κ��� TCP �� �ٸ� ���� �� �� �ִ�.
	mSocket = WSASocket(AF_INET, SOCK_DGRAM, IPPROTO_UDP, NULL, 0, WSA_FLAG_OVERLAPPED);

	if (mSocket == INVALID_SOCKET)
		return FALSE;

	// ������ �ּҷ� Bind �Ѵ�. ������ Bind �� �ּҷ� ��Ŷ�� ���� ���̴�.
	if (bind(mSocket, (struct sockaddr*)&RemoteAddressInfo, sizeof(SOCKADDR_IN)) == SOCKET_ERROR)
	{
		End();

		return FALSE;
	}

	// ���� Event ������Ʈ ���� �� �Ҵ�
	mReliableUdpThreadDestroyEvent = CreateEvent(0, FALSE, FALSE, 0);
	if (mReliableUdpThreadDestroyEvent == NULL)
	{
		End();

		return FALSE;
	}

	// ���� Event ������Ʈ ���� �� �Ҵ�
	mReliableUdpThreadStartupEvent = CreateEvent(0, FALSE, FALSE, 0);
	if (mReliableUdpThreadStartupEvent == NULL)
	{
		End();

		return FALSE;
	}

	// TODO: �̰� ť IOCP ��⿭�� �ִٰ� ����� ���� ���ϴ� �ǰ�? Ȯ��
	// WakeUp Event ������Ʈ ���� �� �Ҵ�
	mReliableUdpThreadWakeUpEvent = CreateEvent(0, FALSE, FALSE, 0);
	if (mReliableUdpThreadWakeUpEvent == NULL)
	{
		End();

		return FALSE;
	}

	// Write Event ������Ʈ�� �����ؼ� �Ҵ�
	mReliableUdpWriteCompleteEvent = CreateEvent(0, FALSE, FALSE, 0);
	if (mReliableUdpWriteCompleteEvent == NULL)
	{
		End();

		return FALSE;
	}

	// Reliable UDP ���� ����� ť�� �ʱ�ȭ
	if (!mReliableWriteQueue.Begin())
	{
		End();

		return FALSE;
	}

	// Reliable UDP �� ������ ����
	DWORD ReliableUdpThreadID = 0;	// ThreadID �޾ƿ��� ����
	mReliableUdpThreadHandle = CreateThread(NULL, 0, ::ReliableUdpThreadCallback, this, 0, &ReliableUdpThreadID);

	// Startup �̺�Ʈ ��ü signal �� ������ ���� ��� (Event ��ü�� Signal �� ������ ���) 
	// Signal �� Ư�� �̺�Ʈ�� �߻��ߴٴ� ���� �˸��� ���̴�. ���ͷ�Ʈ��� �θ��⵵ �Ѵ�.
	// �� ���⼭�� �����尡 �����ǰ� �ݹ鿡�� StartupEvent �� SetEvent �Լ��� singal ��Ű�� return TRUE �ϴ� ���̴�.
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
/// IOCP �� ����ϴ� UDP �ʱ� �ޱ� �Լ�. 
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
		LPWSABUF                           lpBuffers,				// ���� �����͸� ������ ����
		DWORD                              dwBufferCount,			// ���� ���۸� ����� �� ����ϴ� ���� ����
		LPDWORD                            lpNumberOfBytesRecvd,	// ���� ���Ͽ��� ���� ������ ũ��
		LPDWORD                            lpFlags,
		sockaddr * lpFrom,											// �����͸� ���� ����� �ּ� ����, WSARecv �� ������.
		LPINT                              lpFromlen,				// ��� �ּ� ������ ����, WSARecv �� ������.
		LPWSAOVERLAPPED                    lpOverlapped,			
		LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine		// Callback ����� ���� �̺�Ʈ ����� ������ �� ���
	);*/

	INT		ReturnValue = WSARecvFrom(mSocket,
		&WsaBuf,
		1,
		&ReadBytes,
		&ReadFlag,
		(SOCKADDR*)&mUdpRemoteInfo,		// WSARecv �� �ٸ� �Ķ���ʹ�. �����͸� ���� ����� �ּ� ����. �̰��� ���� �����ϴ� ���� TCP �� �ٸ���.
		&RemoteAddressInfoSize,			// WSARecv �� �ٸ� �Ķ���ʹ�. �ּ� ���� ���� 
										// TCP ���� ���� listen �ϴٰ� Accept �ϰ� ���� ����Ǵ� ��Ʈ ���� ����� �����Ǵµ�, 
										// UDP �� �׷� �� ��� �� ������ �����س��� ����� �ٷ� ����ϴ� ���̴�.
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
/// IOCP ����ϴ� UDP ���� ������ ���� �����͸� Ȯ���ϴ� �Լ�.
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

	// WSARecvFrom ���� �޾ƿ� �����͸� data �� ����.
	memcpy(data, mReadBuffer, dataLength);

	// �����͸� ���� IP �ּ� Ȯ��
	strcpy(remoteAddress, inet_ntoa(mUdpRemoteInfo.sin_addr));
	// �����͸� ���� ��Ʈ Ȯ��
	remotePort = ntohs(mUdpRemoteInfo.sin_port);

	USHORT Ack = 0;
	// mReadBuffer ���� �� �� 2����Ʈ�� �����ؼ� Ȯ��
	memcpy(&Ack, mReadBuffer, sizeof(USHORT));

	if (Ack == 9999) // ���� 2����Ʈ�� 9999�� ��� �����͸� �� �޾Ƽ� ������ '����(Ack) ��Ŷ'���� �ν�
	{
		// ���� ��Ŷ�� �޾����Ƿ� Complete �̺�Ʈ�� ���� ���� �����͸� ����
		SetEvent(mReliableUdpWriteCompleteEvent);

		return FALSE;
	}
	else
	{
		// ���ο� �����͸� ���� ���� ���
		Ack = 9999; 

		// �����͸� ���� �ּҷ� Ack 2����Ʈ�� 9999�� ����, ��밡 ������ �������� �� �޾Ҵٰ� �˸��� ���̴�.
		WriteTo2(remoteAddress, remotePort, (BYTE*)&Ack, sizeof(USHORT));
	}

	return TRUE;
}

/// <summary>
/// EventSelect ����� ����� ���� ReadFrom �Լ�. WSARecvFrom �� ����Ѵ�.
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

	if (!mSocket) // TODO: �̰� �Ȱ��� mSocket üũ�� 2�� �� �Ͻôµ�.. �ǵ��� ���ΰ�? �ٸ� �͵� �̷�����.. �Ǽ� �ƴ��� Ȯ���غ���.
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

	// IOCP �� �ٸ��� �ٷ� ����� ���´�. �׷��� �����͸� �޾� Ȯ���Ѵ�.
	memcpy(data, mReadBuffer, ReadBytes);	// CopyMemory �� ����.
	dataLength = ReadBytes;

	// �ּ� ������ �����Ѵ�.
	// TODO: �ּҰ� �ٲ� ���� �ֳ�? �� �Ź� �����ұ�..
	strcpy(remoteAddress, inet_ntoa(mUdpRemoteInfo.sin_addr));
	remotePort = ntohs(mUdpRemoteInfo.sin_port);

	// IOCP �� ���� Reliable UDP �� �������ִ� �ڵ��̴�.
	USHORT Ack = 0;
	memcpy(&Ack, mReadBuffer, sizeof(USHORT));

	// IOCP �� ReadFrom�� �����ϰ� 9999�� ���� ó���� �Ѵ�.
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
/// �����(Client)���� ȣ���ϴ� WriteTo �Լ�. �����͸� ť�� �־� ���� ������ �Ѵ�.
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

	// ť�� ������ �Է�
	if (!mReliableWriteQueue.Push(this, data, dataLength, remoteAddress, remotePort))
		return FALSE;

	// ���� ������ �����Ͱ� ���� ���
	if (!mIsReliableUdpSending)
	{
		// ������ �����Ͱ� �ִٰ� �÷��׸� �ٲٰ�, ReliableUDPThread �� �����.
		mIsReliableUdpSending = TRUE;
		SetEvent(mReliableUdpThreadWakeUpEvent);
	}

	return TRUE;
}

/// <summary>
/// ������ WSAsendTo �� �Ἥ UDP �� �����͸� ������ �Լ��̴�.
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

	// �����͸� ���� �ּ� �Է�
	RemoteAddressInfo.sin_family			= AF_INET;
	RemoteAddressInfo.sin_addr.S_un.S_addr	= inet_addr(remoteAddress);
	RemoteAddressInfo.sin_port				= htons(remotePort);

	// WSASendTo �Լ��� �̿��ؼ� ������ ����
	INT			ReturnValue = WSASendTo(mSocket,
		&WsaBuf,
		1,
		&WriteBytes,
		WriteFlag,
		(SOCKADDR*)&RemoteAddressInfo,		// ���� �ּ� ����
		RemoteAddressInfoSize,				// ���� �ּ� ���� ����, UDP �� �� �з����͸� ����.
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