#pragma once
#include "IocpCore.h"
#include "IocpEvent.h"
#include "NetworkAddress.h"

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

public:
						Session();
	virtual				~Session();

	// 관련 정보 함수
	shared_ptr<Service> GetService() { return _service.lock(); }
	void				SetSerivce(shared_ptr<Service> service) { _service = service; }
	void				SetNetworkAddress(NetworkAddress address) { _netAddress = address; }
	NetworkAddress		GetAddress() { return _netAddress; }
	SOCKET				GetSocket() { return _socket; }
	BYTE*				GetRecvBuffer() { return _recvBuffer; } // TEMP TODO: 추후 변경
	bool				IsConnected() { return _connected; }
	SessionRef			GetSessionRef() { return static_pointer_cast<Session>(shared_from_this()); }

	// 인터페이스 구현	
	virtual HANDLE		GetHandle() override;
	virtual void		Dispatch(class IocpEvent* iocpEvent, int32 numOfBytes = 0) override;

	// 외부 송수신 관련 함수
	void				Send(BYTE* buffer, int32 len);
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
	void				RegisterSend(SendEvent* sendEvent);

	void				ProcessConnect();
	void				ProcessDisconnect();
	void				ProecessDisconnect();
	void				ProcessRecv(int32 numOfBytes);
	void				ProcessSend(SendEvent* sendEvent, int32 numOfBytes);

	void				HandleError(int32 errorCode);
	
private:
	USE_LOCK;

	weak_ptr<Service>	_service; // 서버가 떠 있다면 이건 항상 있을테니 굳이 nullptr 체크는 안하고 사용
	SOCKET				_socket = { INVALID_SOCKET };
	NetworkAddress		_netAddress = {};
	Atomic<bool>		_connected = false;

	BYTE				_recvBuffer[1000] = {}; // TEMP TODO: 추후 변경

	// 수신 관련 기능

	// 송신 관련 기능

	// IocpEvent 재사용을 위한 멤버 변수들
	ConnectEvent		_connectEvent;
	DisconnectEvent		_disconnectEvent;
	RecvEvent			_recvEvent;

};

