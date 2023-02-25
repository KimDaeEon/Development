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

// �⺻������ Send�� ���� ������ ���� ���� ������ ��ƴ�.
// ����ؾ��� �͵�
// 1) ���� ���� 
// 2) sendEvent ���� (���� or ����, WSASend ��ø ����)
void Session::Send(BYTE* buffer, int32 len)
{
	// TEMP
	SendEvent* sendEvent = myNew<SendEvent>();
	sendEvent->owner = shared_from_this();
	sendEvent->buffer.resize(len);
	::memcpy(sendEvent->buffer.data(), buffer, len);

	// WSASend�� thread-safe�� �ƴϿ��� ���� �����忡�� Send�� ȣ���� �� �ֱ⿡ lock���� �������� ��������� �Ѵ�.
	// �׷��ٸ� Recv�� thread-safe ���� �ʳİ� �ñ��� �� �ִµ�, Recv�� thread-safe ���� �ʴ�.
	// �׷��� Recv�� ���� �ڵ忡�� lock�� ���� �ʴ´�. �� ���ǿ��� Recv�� ���� �����忡�� ȣ������ �ʱ� �����̴�.
	// WSASend�� WSARecv�� thread-safe�� ���� ������ �Ʒ� ��ũ ����
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
	// exchange �ߴµ� false�� ���ϵȴٸ� ������ ��� �ִ� ���� false�� ���̰�,
	// �̴� �̹� Disconnect ó���� �� ���� �ǹ��ϹǷ� �Լ��� �����Ѵ�.
	if (_connected.exchange(false) == false)
	{
		return;
	}

	// TEMP
	wcout << "Disconnect : " << cause << endl;

	OnDisconnected(); // ������ �ڵ忡�� �����ε��Ǿ� ó��
	
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

	if (SocketUtils::BindAnyAddress(_socket, 0/*���� ��Ʈ �� �ƹ��ų�*/) == false)
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

	// DisconnectEx�� ���� ������ �Ʒ�ó�� TF_REUSE_SOCKET �ɼ��� �־ 
	// �ش� ������ �ٽ� ���� �� �ֵ��� ���ֱ� �����̴�. 
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
	_recvEvent.owner = shared_from_this(); // �̸� ���ؼ� WSARecv ȣ���ص� ��Ȳ���� Session ����

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
			_recvEvent.owner = nullptr; // �̰� �����ָ� session ������ �ȵǾ �޸� �� �����.
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
	_connectEvent.owner = nullptr; // �ٴ� ������ �ٲ� �� �����Ƿ� �ʱ�ȭ���ش�.
	 
	_connected.store(true);

	// ���� ���
	GetService()->AddSession(GetSessionRef());

	// ������ �ڵ忡�� �����ε�
	OnConnected();

	// ���� ���
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
	_recvEvent.owner = nullptr; // ref count ���ҽ�Ų��. �޸� �� ������

	if (numOfBytes == 0)
	{
		Disconnect(L"Recv 0"); // �����͸� ���� ������ ������ ���´�.
		return;
	}

	// ������ �ڵ忡�� �����ε�
	OnRecv(_recvBuffer, numOfBytes);

	// ���� ���
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

	// ������ �ڵ忡�� �����ε�
	OnSend(numOfBytes);
}

void Session::HandleError(int32 errorCode)
{
	switch (errorCode)
	{
		// WSA ���� �����ؼ��� �ڼ��� ������ �Ʒ� ���� ����
		// https://learn.microsoft.com/ko-kr/windows/win32/winsock/windows-sockets-error-codes-2
	case WSAECONNRESET: // ���� ȣ��Ʈ�� ���� ���� ����
	case WSAECONNABORTED: // �� ȣ��Ʈ�� ���� ���� ����
		Disconnect(L"HandleError");
		break;

	default: // �������� ������� ���� �������� �α׸� ��´�.
		// TODO: Log
		cout << "Handle Error : " << errorCode << endl;
		break;
	}
}
