#pragma once
#include "IocpCore.h"
#include "IocpEvent.h"
#include "NetworkAddress.h"
#include "RecvBuffer.h"

class Service;

// --------------------------------
//			   Session
// --------------------------------
class Session : public IocpObject
{
	// 아래의 클래스들에서는 Session 내부 접근 허용
	friend class Listener;
	friend class IocpCore;
	friend class Service;

	enum
	{
		BUFFER_SIZE = 0x10000, // 64kb
	};

public:
	Session();
	virtual				~Session();

	// 관련 정보 함수
	shared_ptr<Service> GetService() { return _service.lock(); }
	void				SetSerivce(shared_ptr<Service> service) { _service = service; }
	void				SetNetworkAddress(NetworkAddress address) { _netAddress = address; }
	NetworkAddress		GetAddress() { return _netAddress; }
	SOCKET				GetSocket() { return _socket; }
	bool				IsConnected() { return _connected; }
	SessionRef			GetSessionRef() { return static_pointer_cast<Session>(shared_from_this()); }

	// 인터페이스 구현	
	virtual HANDLE		GetHandle() override;
	virtual void		Dispatch(class IocpEvent* iocpEvent, int32 numOfBytes = 0) override;

	// 외부 송수신 관련 함수
	void				Send(SendBufferRef sendBuffer);
	bool				Connect();
	void				Disconnect(const WCHAR* cause);

protected:
	// 컨텐츠 코드에서 해당 함수들을 오버라이딩해서 사용, 어떤 종류의 세션이냐에 따라 다른 동작을 한다.
	virtual void		OnConnected() {}
	virtual int32		OnRecv(BYTE* buffer, int32 len) { return len; }
	virtual void		OnSend(int32 len) {}
	virtual void		OnDisconnected() {}

private:
	// 내부 송수신 관련 함수
	bool				RegisterConnect();
	bool				RegisterDisconnect();
	void				RegisterRecv();
	void				RegisterSend();

	void				ProcessConnect();
	void				ProcessDisconnect();
	void				ProecessDisconnect();
	void				ProcessRecv(int32 numOfBytes);
	void				ProcessSend(int32 numOfBytes);

	void				HandleError(int32 errorCode);

private:
	USE_LOCK;

	weak_ptr<Service>		_service; // 서버가 떠 있다면 이건 항상 있을테니 굳이 nullptr 체크는 안하고 사용
	SOCKET					_socket = { INVALID_SOCKET };
	NetworkAddress			_netAddress = {};
	Atomic<bool>			_connected = false;

	// 수신 관련
	RecvBuffer				_recvBuffer; // TEMP TODO: 추후 변경

	// 송신 관련
	myQueue<SendBufferRef>	_sendQueue; // SendEvent 처리가 안되었으면 여기에 보낼 패킷들이 쌓인다. // TODO: LockFreeQueue와 성능 비교 후 더 나은 것 적용
	Atomic<bool>			_isSendRegistered; // SendEvent가 처리가 된 상황인지 체크

	// IocpEvent 재사용을 위한 멤버 변수들
	ConnectEvent			_connectEvent;
	DisconnectEvent			_disconnectEvent;
	RecvEvent				_recvEvent;
	SendEvent				_sendEvent;

};


// ---------------------------------
//			PacketSession
// ---------------------------------
struct PacketHeader
{
	uint16 size;
	uint16 id; // Protocl ID
};

class PacketSession : public Session
{
public:
	PacketSession();
	virtual ~PacketSession();

	PacketSessionRef	GetPacketSessionRef() { return static_pointer_cast<PacketSession>(shared_from_this()); }

protected:
	virtual int32		OnRecv(BYTE* buffer, int32 len) sealed; // OnRecv는 재정의 불가능, OnRecvPacket만 재정의 가능
	virtual void		OnRecvPacket(BYTE* buffer, int32 len) abstract;

};