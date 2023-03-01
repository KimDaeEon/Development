#pragma once

class Session;

enum class EventType : uint8
{
	Connect,
	Disconnect,
	Accept,
	//PreRecv, zero byte recv 용도, 설명은 링크 참조 // https://ozt88.tistory.com/26
	Recv,
	Send
};


// ---------------------------
//			IocpEvent
// ---------------------------

// Iocp 이벤트 종류 + 기타 정보를 관리하는 객체
// 중요한 점은 IocpEvent 내에 가상함수를 만들면 안된다.
// 그러면 vtable이 클래스에 맨 앞에 오게되어서 OVERLAPPED로 형변환할 때에 문제가 생길 수 있다.
// 테스트를 해보니 reinterpret_cast를 쓰면 가상함수 사용시에 문제가 생기고, static_cast같은 경우는 문제가 없었다.
// static_cast가 런타임시에 오프셋 변형을 해주어서 그런 것이라고 한다. 
// https://stackoverflow.com/questions/332030/when-should-static-cast-dynamic-cast-const-cast-and-reinterpret-cast-be-used
class IocpEvent : public OVERLAPPED
{
public:
	IocpEvent(EventType type);

	void			Init();

public:
	EventType		eventType;
	IocpObjectRef	owner; // IocpObject가 IocpEvent 처리 동안에는 해제되지 않도록 보장한다.
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