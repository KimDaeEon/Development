#include "pch.h"
#include "Session.h"
#include "SocketUtils.h"
#include "Service.h"

// --------------------------------
//			   Session
// --------------------------------
Session::Session()
{
	_socket = SocketUtils::CreateSocket();
}

Session::~Session()
{
	SocketUtils::Close(_socket);
}

// 기본적으로 Send가 가장 생각할 것이 많고 구현이 어렵다.
// 고민해야할 것들
// 1) 버퍼 관리 
// 2) sendEvent 관리 (단일 or 복수, WSASend 중첩 여부)
void Session::Send(BYTE* buffer, int32 len)
{
	// TEMP
	SendEvent* sendEvent = myNew<SendEvent>();
	sendEvent->owner = shared_from_this();
	sendEvent->buffer.resize(len);
	::memcpy(sendEvent->buffer.data(), buffer, len);

	// WSASend는 thread-safe가 아니여서 여러 스레드에서 Send를 호출할 수 있기에 lock으로 안전함을 보장해줘야 한다.
	// 그렇다면 Recv는 thread-safe 하지 않냐가 궁금할 수 있는데, Recv도 thread-safe 하지 않다.
	// 그런데 Recv는 현재 코드에서 lock을 잡지 않는다. 한 세션에서 Recv를 여러 스레드에서 호출하지 않기 때문이다.
	// WSASend와 WSARecv의 thread-safe에 대한 설명은 아래 링크 참조
	// https://stackoverflow.com/questions/28770789/is-calling-wsasend-and-wsarecv-from-two-threads-safe-when-using-iocp
	WRITE_LOCK; 

	RegisterSend(sendEvent);
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

	OnDisconnected(); // 컨텐츠 코드에서 오버로딩되어 처리
	
	GetService()->ReleaseSession(GetSessionRef());
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
		ProcessSend(static_cast<SendEvent*>(iocpEvent), numOfBytes);
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
	wsaBuf.buf = reinterpret_cast<char*>(_recvBuffer);
	wsaBuf.len = len32(_recvBuffer);

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

void Session::RegisterSend(SendEvent* sendEvent)
{
	if (IsConnected() == false)
	{
		return;
	}

	WSABUF wsaBuf;
	wsaBuf.buf = (char*)sendEvent->buffer.data();
	wsaBuf.len = (ULONG)sendEvent->buffer.size();

	DWORD numOfBytes = 0;
	if (SOCKET_ERROR == ::WSASend(_socket, &wsaBuf, 1, &numOfBytes, 0, sendEvent, nullptr))
	{
		int32 errorCode = ::WSAGetLastError();
		if (errorCode != WSA_IO_PENDING)
		{
			HandleError(errorCode);
			sendEvent->owner = nullptr;
			myDelete(sendEvent);
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

	// 컨텐츠 코드에서 오버로딩
	OnRecv(_recvBuffer, numOfBytes);

	// 수신 등록
	RegisterRecv();
}

void Session::ProcessSend(SendEvent* sendEvent, int32 numOfBytes)
{
	sendEvent->owner = nullptr;
	myDelete(sendEvent);

	if (numOfBytes == 0)
	{
		Disconnect(L"Send 0");
		return;
	}

	// 컨텐츠 코드에서 오버로딩
	OnSend(numOfBytes);
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
