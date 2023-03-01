#pragma once

class Session;

enum class EventType : uint8
{
	Connect,
	Disconnect,
	Accept,
	//PreRecv, zero byte recv �뵵, ������ ��ũ ���� // https://ozt88.tistory.com/26
	Recv,
	Send
};


// ---------------------------
//			IocpEvent
// ---------------------------

// Iocp �̺�Ʈ ���� + ��Ÿ ������ �����ϴ� ��ü
// �߿��� ���� IocpEvent ���� �����Լ��� ����� �ȵȴ�.
// �׷��� vtable�� Ŭ������ �� �տ� ���ԵǾ OVERLAPPED�� ����ȯ�� ���� ������ ���� �� �ִ�.
// �׽�Ʈ�� �غ��� reinterpret_cast�� ���� �����Լ� ���ÿ� ������ �����, static_cast���� ���� ������ ������.
// static_cast�� ��Ÿ�ӽÿ� ������ ������ ���־ �׷� ���̶�� �Ѵ�. 
// https://stackoverflow.com/questions/332030/when-should-static-cast-dynamic-cast-const-cast-and-reinterpret-cast-be-used
class IocpEvent : public OVERLAPPED
{
public:
	IocpEvent(EventType type);

	void			Init();

public:
	EventType		eventType;
	IocpObjectRef	owner; // IocpObject�� IocpEvent ó�� ���ȿ��� �������� �ʵ��� �����Ѵ�.
};


// ------------------------------
//			ConnectEvent
// ------------------------------
class ConnectEvent : public IocpEvent
{
public:
	ConnectEvent() : IocpEvent(EventType::Connect) {}
};

// ------------------------------
//		  DisconnectEvent
// ------------------------------
class DisconnectEvent : public IocpEvent
{
public:
	DisconnectEvent() : IocpEvent(EventType::Disconnect) {}
};


// ------------------------------
//			AcceptEvent
// ------------------------------
class AcceptEvent : public IocpEvent
{
public:
	AcceptEvent() : IocpEvent(EventType::Accept) {}

public:
	SessionRef	session = { nullptr };
};


// ------------------------------
//			 RecvEvent
// ------------------------------
class RecvEvent : public IocpEvent
{
public:
	RecvEvent() : IocpEvent(EventType::Recv) {}
};


// ------------------------------
//			 SendEvent
// ------------------------------
class SendEvent : public IocpEvent
{
public:
	SendEvent() : IocpEvent(EventType::Send) {}

	myVector<SendBufferRef> sendBuffers;
};