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

	// �ּ� ��ĥ ��쿡 ���� �ȶߴ� �� ���� ���� ��ġ
	if (SocketUtils::SetReuseAddress(_listenSocket, true) == false)
	{
		return false;
	}

	// Linger �ɼ��� ���ش�.
	// �ɼ��� ���ָ� ��� Ŀ�ο��� �������� �����ϰ�, ������ ���ۿ� �����ִ� ��� �����͸� �����ϴ� �Ϲ����� ������ ���� ���ᰡ �Ͼ��.
	// Linger �ɼ��� Ű�� Ŀ�� �۾��� ���������� '���(Linger)'�ϴ� ���̴�.
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
		_acceptEvents.push_back(acceptEvent); // ���Ŀ� �޸� ������ �ϱ� ���ؼ� �����صд�.
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
		session->GetRecvBuffer(), 0 /*ù ��°�� ������ ����Ʈ ��Ʈ���� ũ��, 0�̸� ���� �����ϴ��� �����۾� X*/,
		sizeof(SOCKADDR_IN) + 16, sizeof(SOCKADDR_IN) + 16,
		OUT & bytesReceived, static_cast<LPOVERLAPPED>(acceptEvent)))
	{
		const int32 errorCode = ::WSAGetLastError();
		if (errorCode != WSA_IO_PENDING)
		{
			// IO_PENDING�� �ƴϸ� ���� ������ ���� ���̶� �ϴ� �ٽ� Accept�� �ɾ��ش�. 
			RegisterAccept(acceptEvent);
		}
	}
}

// Iocp �۾��� �������� �ٽ� ����� ���´�.
void Listener::Dispatch(IocpEvent* iocpEvent, int32 numOfBytes)
{
	ASSERT_CRASH(iocpEvent->eventType == EventType::Accept);

	AcceptEvent* acceptEvent = static_cast<AcceptEvent*>(iocpEvent);
	ProcessAccept(acceptEvent);
}

// Dispatch�� ������ ���⼭ ó���� �ȴ�.
void Listener::ProcessAccept(AcceptEvent* acceptEvent)
{
	// �̷��� ���� ���ؼ� AcceptEvent�� Session�� ����ִ� ���̴�.
	SessionRef session = acceptEvent->session;

	// Listen Socket�� �ɼ��� �Ȱ��� �����ش�.
	if (false == SocketUtils::SetUpdateAcceptSocket(session->GetSocket(), _listenSocket))
	{
		// �������� ������ ��� �̷��� ����� ����� ���� Accept �۾��� ����ȴ�.
		// ���� �ڵ� �������� ���� �Ŀ� RegisterAccept�� ���� ������ �����Ѵ�.
		RegisterAccept(acceptEvent);
		return;
	}

	// ������ ���� ���
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

