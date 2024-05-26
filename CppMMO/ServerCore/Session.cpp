#include "pch.h"
#include "Session.h"
#include "SocketUtils.h"
#include "Service.h"

// --------------------------------
//			   Session
// --------------------------------
Session::Session() : _recvBuffer(BUFFER_SIZE)
{
	_socket = SocketUtils::CreateSocket();
}

Session::~Session()
{
	SocketUtils::Close(_socket);
}

void Session::Send(SendBufferRef sendBuffer)
{
	if(IsConnected() == false)
	{
		return;
	}
	
	bool isRegisterSend = false;

	// WSASend는 thread-safe가 아니여서 여러 스레드에서 Send를 호출할 수 있기에 lock으로 안전함을 보장해줘야 한다.
	// 그렇다면 Recv는 thread-safe 하지 않냐가 궁금할 수 있는데, Recv도 thread-safe 하지 않다.
	// 그런데 Recv는 현재 코드에서 lock을 잡지 않는다. 한 세션에서 Recv를 여러 스레드에서 호출하지 않기 때문이다.
	// WSASend와 WSARecv의 thread-safe에 대한 설명은 아래 링크 참조
	// https://stackoverflow.com/questions/28770789/is-calling-wsasend-and-wsarecv-from-two-threads-safe-when-using-iocp
	{
		// WRITE_LOCK을 걸어주면서 _sendQueue에 데이터를 넣어준다.
		WRITE_LOCK;

		_sendQueue.push(sendBuffer);

		// 처음 Send를 할 수 있는 녀석은 Send까지 처리한다. isSendRegistered를 획득하지 못한 녀석들은 _sendQueue에 push만 해준다.
		if (_isSendRegistered.exchange(true) == false)
		{
			isRegisterSend = true;
		}
	}

	// 이렇게 RegisterSend를 바깥으로 빼줘서 WRITE_LOCK 부분이 너무 길어지지 않게 한다.
	if (isRegisterSend)
	{
		RegisterSend();
	}
}


void Session::RegisterSend()
{
	if (IsConnected() == false)
	{
		return;
	}

	_sendEvent.Init();
	_sendEvent.owner = shared_from_this();

	// 보낼 데이터를 sendEvent에 등록
	{
		// _sendQueue에 여러 스레드가 접근하는 상황 고려해서 lock을 걸어준다.
		WRITE_LOCK;

		int32 writeSize = 0;
		while (_sendQueue.empty() == false)
		{
			SendBufferRef sendBuffer = _sendQueue.front();
			writeSize += sendBuffer->WriteSize();
			// TODO: 너무 많은 데이터가 들어올 때에 예외 처리 코드 추가 필요

			_sendQueue.pop();
			_sendEvent.sendBuffers.push_back(sendBuffer);
		}
	}

	// Scatter-Gather (흩어져 있는 WSABUF 데이터 모아서 보내기)
	myVector<WSABUF> wsaBufs;
	wsaBufs.reserve(_sendEvent.sendBuffers.size());

	for (SendBufferRef sendBuffer : _sendEvent.sendBuffers)
	{
		WSABUF wsaBuf;
		wsaBuf.buf = reinterpret_cast<char*>(sendBuffer->Buffer());
		wsaBuf.len = static_cast<ULONG>(sendBuffer->WriteSize());
		wsaBufs.push_back(wsaBuf);
	}


	DWORD numOfBytes = 0;
	if (SOCKET_ERROR == ::WSASend(_socket, wsaBufs.data(), static_cast<DWORD>(wsaBufs.size()), &numOfBytes, 0, &_sendEvent, nullptr))
	{
		int32 errorCode = ::WSAGetLastError();
		if (errorCode != WSA_IO_PENDING)
		{
			HandleError(errorCode);
			_sendEvent.owner = nullptr;
			_sendEvent.sendBuffers.clear();
			_isSendRegistered.store(false);
		}
	}
}

bool Session::Connect()
{
	return RegisterConnect();
}

void Session::Disconnect(const WCHAR* cause)
{
	// exchange 했는데 false가 리턴된다면 이전에 들고 있던 값이 false란 것이고,
	// 이는 이미 Disconnect 처리가 된 것을 의미하므로 함수를 종료한다.
	if (_connected.exchange(false) == false)
	{
		return;
	}

	// TEMP
	wcout << "Disconnect : " << cause << endl;

	RegisterDisconnect();
}

HANDLE Session::GetHandle()
{
	return reinterpret_cast<HANDLE>(_socket);
}

void Session::Dispatch(IocpEvent* iocpEvent, int32 numOfBytes)
{
	switch (iocpEvent->eventType)
	{
	case EventType::Connect:
		ProcessConnect();
		break;

	case EventType::Disconnect:
		ProcessDisconnect();
		break;

	case EventType::Recv:
		ProcessRecv(numOfBytes);
		break;
		
	case EventType::Send:
		ProcessSend(numOfBytes);
		break;
		
	default:
		ASSERT_CRASH("INVALID_EVENT_TYPE");
		break;
	}
}

bool Session::RegisterConnect()
{
	if (IsConnected())
	{
		return false;
	}

	if (GetService()->GetServiceType() != ServiceType::Client)
	{
		return false;
	}

	if (SocketUtils::BindAnyAddress(_socket, 0/*남는 포트 중 아무거나*/) == false)
	{
		return false;
	}

	_connectEvent.Init();
	_connectEvent.owner = shared_from_this();

	DWORD numOfBytes = 0;
	SOCKADDR_IN sockAddr = GetService()->GetNetAddress().GetSockAddr();
	if (false == SocketUtils::ConnectEx(_socket, reinterpret_cast<SOCKADDR*>(&sockAddr),
		sizeof(sockAddr), nullptr, 0, &numOfBytes, &_connectEvent))
	{
		int32 errorCode = ::WSAGetLastError();
		if (errorCode != WSA_IO_PENDING)
		{
			_connectEvent.owner = nullptr;
			return false;
		}
	}

	return true;
}

bool Session::RegisterDisconnect()
{
	_disconnectEvent.Init();
	_disconnectEvent.owner = shared_from_this(); 

	// DisconnectEx를 쓰는 이유는 아래처럼 TF_REUSE_SOCKET 옵션을 넣어서 
	// 해당 소켓이 다시 사용될 수 있도록 해주기 때문이다. 
	if (false == SocketUtils::DisconnectEx(_socket, &_disconnectEvent, TF_REUSE_SOCKET, 0))
	{
		int32 errorCode = ::WSAGetLastError();
		if (errorCode != WSA_IO_PENDING)
		{
			_disconnectEvent.owner = nullptr;
			return false;
		}
	}

	return true;
}

void Session::RegisterRecv()
{
	if (IsConnected() == false)
	{
		return;
	}

	_recvEvent.Init();
	_recvEvent.owner = shared_from_this(); // 이를 통해서 WSARecv 호출해둔 상황에서 Session 유지

	WSABUF wsaBuf;
	wsaBuf.buf = reinterpret_cast<char*>(_recvBuffer.WritePos());
	wsaBuf.len = _recvBuffer.FreeSize();

	DWORD numOfBytes = 0;
	DWORD flags = 0;

	if (SOCKET_ERROR == ::WSARecv(_socket, &wsaBuf, 1, OUT & numOfBytes, OUT & flags, &_recvEvent, nullptr))
	{
		int32 errorCode = ::WSAGetLastError();
		if (errorCode != WSA_IO_PENDING)
		{
			HandleError(errorCode);
			_recvEvent.owner = nullptr; // 이거 안해주면 session 삭제가 안되어서 메모리 릭 생긴다.
		}
	}
}

void Session::ProcessConnect()
{
	_connectEvent.owner = nullptr; // 붙는 세션이 바뀔 수 있으므로 초기화해준다.
	 
	_connected.store(true);

	// 세션 등록
	GetService()->AddSession(GetSessionRef());

	// 컨텐츠 코드에서 오버로딩
	OnConnected();

	// 수신 등록
	RegisterRecv();
}

void Session::ProcessDisconnect()
{
	_disconnectEvent.owner = nullptr;

	// 컨텐츠 코드에서 오버로딩되어 처리
	// 아래 코드들이 Disconnect()에 있으면 Send 중에 접속이 끊길 시에 _sessions를 건드리게 된다.
	OnDisconnected(); 
	GetService()->ReleaseSession(GetSessionRef());
}

void Session::ProecessDisconnect()
{
}

void Session::ProcessRecv(int32 numOfBytes)
{
	_recvEvent.owner = nullptr; // ref count 감소시킨다. 메모리 릭 방지용

	if (numOfBytes == 0)
	{
		Disconnect(L"Recv 0"); // 데이터를 받지 않으면 연결을 끊는다.
		return;
	}

	// writePos를 당겨준다.
	if (false == _recvBuffer.OnWrite(numOfBytes))
	{
		Disconnect(L"OnWrite Overflow"); // 이런 상황이 생기면 안된다.
		return;
	}

	int32 dataSize = _recvBuffer.DataSize(); // 쌓여서 처리되어야할 데이터 크기

	// 컨텐츠 코드에서 재정의
	int32 processedLen = OnRecv(_recvBuffer.ReadPos(), numOfBytes);
	if (processedLen < 0 || dataSize < processedLen || _recvBuffer.OnRead(processedLen) == false)
	{
		Disconnect(L"OnRead Overflow");
		return;
	}

	// 데이터 읽기작업이 끝났으므로 버퍼 정렬을 실시
	_recvBuffer.Relocate();

	// 수신 등록
	RegisterRecv();
}

void Session::ProcessSend(int32 numOfBytes)
{
	_sendEvent.owner = nullptr;
	_sendEvent.sendBuffers.clear();

	if (numOfBytes == 0)
	{
		Disconnect(L"Send 0");
		return;
	}

	// 컨텐츠 코드에서 오버로딩
	OnSend(numOfBytes);

	WRITE_LOCK;
	if (_sendQueue.empty()) // TODO: 이거 LockFreeQueue로 바꾸고 위에 WRITE_LOCK 지우는 것 고려해보기
	{
		_isSendRegistered.store(false);
	}
	// sendQueue에 데이터가 추가되었다면 해당 스레드에서 다시 Send를 등록해준다.
	else
	{
		RegisterSend();
	}
}

void Session::HandleError(int32 errorCode)
{
	switch (errorCode)
	{
		// WSA 에러 관련해서는 자세한 내용은 아래 문서 참조
		// https://learn.microsoft.com/ko-kr/windows/win32/winsock/windows-sockets-error-codes-2
	case WSAECONNRESET: // 원격 호스트에 의한 연결 종료
	case WSAECONNABORTED: // 내 호스트에 의한 연결 종료
		Disconnect(L"HandleError");
		break;

	default: // 아직까지 고려되지 않은 에러들은 로그를 찍는다.
		// TODO: Log
		cout << "Handle Error : " << errorCode << endl;
		break;
	}
}


// ---------------------------------
//			PacketSession
// ---------------------------------
PacketSession::PacketSession()
{
}

PacketSession::~PacketSession()
{
}

// [size(2)][id(2)][data ...][size(2)][id(2)][data ...] <- 요런 구조의 데이터를 분리 조립한다.
int32 PacketSession::OnRecv(BYTE* buffer, int32 len)
{
	int32 processedLen = 0;

	while (true)
	{
		int32 dataSize = len - processedLen;
		// 헤더를 파싱할 수 있을 때까지 대기
		if (dataSize < sizeof(PacketHeader))
		{
			break;
		}

		PacketHeader header = *(reinterpret_cast<PacketHeader*>(&buffer[processedLen]));
		// 헤더에 기록된 패킷 크기의 데이터를 파싱할 수 있을 때까지 대기
		if (dataSize < header.size)
		{
			break;
		}

		// 헤더와 데이터 모두 분리 및 조립이 가능한 상태이므로 조립 시작
		OnRecvPacket(&buffer[processedLen], header.size);

		processedLen += header.size;
	}

	return processedLen;
}
