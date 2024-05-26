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

	// WSASend�� thread-safe�� �ƴϿ��� ���� �����忡�� Send�� ȣ���� �� �ֱ⿡ lock���� �������� ��������� �Ѵ�.
	// �׷��ٸ� Recv�� thread-safe ���� �ʳİ� �ñ��� �� �ִµ�, Recv�� thread-safe ���� �ʴ�.
	// �׷��� Recv�� ���� �ڵ忡�� lock�� ���� �ʴ´�. �� ���ǿ��� Recv�� ���� �����忡�� ȣ������ �ʱ� �����̴�.
	// WSASend�� WSARecv�� thread-safe�� ���� ������ �Ʒ� ��ũ ����
	// https://stackoverflow.com/questions/28770789/is-calling-wsasend-and-wsarecv-from-two-threads-safe-when-using-iocp
	{
		// WRITE_LOCK�� �ɾ��ָ鼭 _sendQueue�� �����͸� �־��ش�.
		WRITE_LOCK;

		_sendQueue.push(sendBuffer);

		// ó�� Send�� �� �� �ִ� �༮�� Send���� ó���Ѵ�. isSendRegistered�� ȹ������ ���� �༮���� _sendQueue�� push�� ���ش�.
		if (_isSendRegistered.exchange(true) == false)
		{
			isRegisterSend = true;
		}
	}

	// �̷��� RegisterSend�� �ٱ����� ���༭ WRITE_LOCK �κ��� �ʹ� ������� �ʰ� �Ѵ�.
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

	// ���� �����͸� sendEvent�� ���
	{
		// _sendQueue�� ���� �����尡 �����ϴ� ��Ȳ ����ؼ� lock�� �ɾ��ش�.
		WRITE_LOCK;

		int32 writeSize = 0;
		while (_sendQueue.empty() == false)
		{
			SendBufferRef sendBuffer = _sendQueue.front();
			writeSize += sendBuffer->WriteSize();
			// TODO: �ʹ� ���� �����Ͱ� ���� ���� ���� ó�� �ڵ� �߰� �ʿ�

			_sendQueue.pop();
			_sendEvent.sendBuffers.push_back(sendBuffer);
		}
	}

	// Scatter-Gather (����� �ִ� WSABUF ������ ��Ƽ� ������)
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
	// exchange �ߴµ� false�� ���ϵȴٸ� ������ ��� �ִ� ���� false�� ���̰�,
	// �̴� �̹� Disconnect ó���� �� ���� �ǹ��ϹǷ� �Լ��� �����Ѵ�.
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
			_recvEvent.owner = nullptr; // �̰� �����ָ� session ������ �ȵǾ �޸� �� �����.
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

	// ������ �ڵ忡�� �����ε��Ǿ� ó��
	// �Ʒ� �ڵ���� Disconnect()�� ������ Send �߿� ������ ���� �ÿ� _sessions�� �ǵ帮�� �ȴ�.
	OnDisconnected(); 
	GetService()->ReleaseSession(GetSessionRef());
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

	// writePos�� ����ش�.
	if (false == _recvBuffer.OnWrite(numOfBytes))
	{
		Disconnect(L"OnWrite Overflow"); // �̷� ��Ȳ�� ����� �ȵȴ�.
		return;
	}

	int32 dataSize = _recvBuffer.DataSize(); // �׿��� ó���Ǿ���� ������ ũ��

	// ������ �ڵ忡�� ������
	int32 processedLen = OnRecv(_recvBuffer.ReadPos(), numOfBytes);
	if (processedLen < 0 || dataSize < processedLen || _recvBuffer.OnRead(processedLen) == false)
	{
		Disconnect(L"OnRead Overflow");
		return;
	}

	// ������ �б��۾��� �������Ƿ� ���� ������ �ǽ�
	_recvBuffer.Relocate();

	// ���� ���
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

	// ������ �ڵ忡�� �����ε�
	OnSend(numOfBytes);

	WRITE_LOCK;
	if (_sendQueue.empty()) // TODO: �̰� LockFreeQueue�� �ٲٰ� ���� WRITE_LOCK ����� �� ����غ���
	{
		_isSendRegistered.store(false);
	}
	// sendQueue�� �����Ͱ� �߰��Ǿ��ٸ� �ش� �����忡�� �ٽ� Send�� ������ش�.
	else
	{
		RegisterSend();
	}
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


// ---------------------------------
//			PacketSession
// ---------------------------------
PacketSession::PacketSession()
{
}

PacketSession::~PacketSession()
{
}

// [size(2)][id(2)][data ...][size(2)][id(2)][data ...] <- �䷱ ������ �����͸� �и� �����Ѵ�.
int32 PacketSession::OnRecv(BYTE* buffer, int32 len)
{
	int32 processedLen = 0;

	while (true)
	{
		int32 dataSize = len - processedLen;
		// ����� �Ľ��� �� ���� ������ ���
		if (dataSize < sizeof(PacketHeader))
		{
			break;
		}

		PacketHeader header = *(reinterpret_cast<PacketHeader*>(&buffer[processedLen]));
		// ����� ��ϵ� ��Ŷ ũ���� �����͸� �Ľ��� �� ���� ������ ���
		if (dataSize < header.size)
		{
			break;
		}

		// ����� ������ ��� �и� �� ������ ������ �����̹Ƿ� ���� ����
		OnRecvPacket(&buffer[processedLen], header.size);

		processedLen += header.size;
	}

	return processedLen;
}
