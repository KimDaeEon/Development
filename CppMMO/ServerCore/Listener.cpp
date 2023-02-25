#include "pch.h"
#include "Listener.h"
#include "SocketUtils.h"
#include "IocpEvent.h"
#include "Session.h"
#include "Service.h"

Listener::~Listener()
{
	SocketUtils::Close(_listenSocket);

	for (AcceptEvent* acceptEvent : _acceptEvents)
	{
		// TODO
		myDelete(acceptEvent);
	}
}

bool Listener::StartAccept(ServerServiceRef service)
{
	_service = service;
	if (_service == nullptr)
	{
		return false;
	}

	_listenSocket = SocketUtils::CreateSocket();
	if (_listenSocket == INVALID_SOCKET)
	{
		return false;
	}

	if (service->GetIocpCore()->Register(shared_from_this()) == false)
	{
		return false;
	}

	// 주소 겹칠 경우에 서버 안뜨는 것 막기 위한 조치
	if (SocketUtils::SetReuseAddress(_listenSocket, true) == false)
	{
		return false;
	}

	// Linger 옵션은 꺼준다.
	// 옵션을 꺼주면 즉시 커널에서 유저모드로 복귀하고, 소켓은 버퍼에 남아있는 모든 데이터를 전송하는 일반적인 소켓의 정상 종료가 일어난다.
	// Linger 옵션을 키면 커널 작업이 끝날때까지 '대기(Linger)'하는 것이다.
	// https://blog.naver.com/bringmelove1/119145244
	if (SocketUtils::SetLinger(_listenSocket, 0, 0) == false)
	{
		return false;
	}

	if (SocketUtils::Bind(_listenSocket, _service->GetNetAddress()) == false)
	{
		return false;
	}

	if (SocketUtils::Listen(_listenSocket) == false)
	{
		return false;
	}

	const int32 acceptCount = _service->GetMaxSessionCount();
	for (int32 i = 0; i < acceptCount; i++)
	{
		AcceptEvent* acceptEvent = myNew<AcceptEvent>();
		acceptEvent->owner = shared_from_this();
		_acceptEvents.push_back(acceptEvent); // 추후에 메모리 해제를 하기 위해서 저장해둔다.
		RegisterAccept(acceptEvent);
	}

	return true;
}

void Listener::CloseSocket()
{
	SocketUtils::Close(_listenSocket);
}

HANDLE Listener::GetHandle()
{
	return reinterpret_cast<HANDLE>(_listenSocket);
}

void Listener::RegisterAccept(AcceptEvent* acceptEvent)
{
	SessionRef session = _service->CreateSession();

	acceptEvent->Init();
	acceptEvent->session = session;

	DWORD bytesReceived = 0;
	if (false == SocketUtils::AcceptEx(_listenSocket, session->GetSocket(),
		session->GetRecvBuffer(), 0 /*첫 번째로 수신할 바이트 스트림의 크기, 0이면 연결 수락하더라도 수신작업 X*/,
		sizeof(SOCKADDR_IN) + 16, sizeof(SOCKADDR_IN) + 16,
		OUT & bytesReceived, static_cast<LPOVERLAPPED>(acceptEvent)))
	{
		const int32 errorCode = ::WSAGetLastError();
		if (errorCode != WSA_IO_PENDING)
		{
			// IO_PENDING이 아니면 뭔가 문제가 생긴 것이라서 일단 다시 Accept를 걸어준다. 
			RegisterAccept(acceptEvent);
		}
	}
}

// Iocp 작업이 끝났으면 다시 여기로 들어온다.
void Listener::Dispatch(IocpEvent* iocpEvent, int32 numOfBytes)
{
	ASSERT_CRASH(iocpEvent->eventType == EventType::Accept);

	AcceptEvent* acceptEvent = static_cast<AcceptEvent*>(iocpEvent);
	ProcessAccept(acceptEvent);
}

// Dispatch가 끝나면 여기서 처리가 된다.
void Listener::ProcessAccept(AcceptEvent* acceptEvent)
{
	// 이렇게 쓰기 위해서 AcceptEvent에 Session이 들어있는 것이다.
	SessionRef session = acceptEvent->session;

	// Listen Socket과 옵션을 똑같이 맞춰준다.
	if (false == SocketUtils::SetUpdateAcceptSocket(session->GetSocket(), _listenSocket))
	{
		// 실패했을 때에도 계속 이렇게 등록을 해줘야 다음 Accept 작업이 실행된다.
		// 다음 코드 블럭에서도 실패 후에 RegisterAccept가 같은 이유로 존재한다.
		RegisterAccept(acceptEvent);
		return;
	}

	// 문제가 생긴 경우
	SOCKADDR_IN sockAddress;
	int32 sizeOfSockAddr = sizeof(sockAddress);
	if (SOCKET_ERROR == ::getpeername(session->GetSocket(), OUT reinterpret_cast<SOCKADDR*>(&sockAddress), (&sizeOfSockAddr)))
	{
		RegisterAccept(acceptEvent);
		return;
	}

	session->SetNetworkAddress(NetworkAddress(sockAddress));
	cout << "Client Connected!" << endl;
	session->ProcessConnect();
	RegisterAccept(acceptEvent);
}

