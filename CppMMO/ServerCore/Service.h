#pragma once
#include "NetworkAddress.h"
#include "IocpCore.h"
#include "Listener.h"
#include <functional>

enum class ServiceType : uint8
{
	Server,
	Client
};

// -----------------------------
//			 Service
// -----------------------------
using SessionFactory = function<SessionRef(void)>;

// Session, NetowrkAddress, IocpCore에 대한 모든 정보를 한 번에 관리하기 위한 객체
// 실질적으로 통신을 하기 위한 요소들이 다 모여있다고 보면 된다.
// 이렇게 분리한 이유는 Iocp 핸들을 Service 별로 분리하여 Session을 관리하고 싶은 이유가 가장 크다.
class Service : public enable_shared_from_this<Service>
{
public:
	// address: 클라이언트라면 상대 서버의 주소, 서버라면 자신의 주소
	// maxSessionCount: 보통 1:1 연결이므로 이렇게 설정
	Service(ServiceType type, NetworkAddress address, IocpCoreRef core, SessionFactory factory, int32 maxSessionCount = 1);
	virtual ~Service();

	virtual bool	Start() abstract;
	bool			CanStart() { return _sessionFactory != nullptr; }

	virtual void	CloseService();
	void			SetSessionFactory(SessionFactory func) { _sessionFactory = func; }

	void			Broadcast(SendBufferRef sendBuffer);
	SessionRef		CreateSession(); // 세션 생성 + Iocp에 등록
	void			AddSession(SessionRef session);
	void			ReleaseSession(SessionRef session);
	int32			GetCurrentSessionCount() { return _sessionCount; } // 굳이 lock은 안건다. 대략적인 숫자만 알면 되기에.. TODO: 추후에 이게 문제라면 수정 필요 
	int32			GetMaxSessionCount() { return _maxSessionCount; }

	ServiceType		GetServiceType() { return _type; }
	NetworkAddress	GetNetAddress() { return _netAddress; }
	IocpCoreRef		GetIocpCore() { return _iocpCore; }

protected:
	USE_LOCK;

	ServiceType			_type;
	NetworkAddress		_netAddress = {};
	IocpCoreRef			_iocpCore;

	mySet<SessionRef>	_sessions;
	int32				_sessionCount = 0;
	int32				_maxSessionCount = 0;
	SessionFactory		_sessionFactory;

};

// ---------------------------------
//			 ClientService
// ---------------------------------
class ClientService : public Service
{
public:
	ClientService(NetworkAddress targetAddress, IocpCoreRef core, SessionFactory factory, int32 maxSessionCount = 1);
	virtual ~ClientService();

	virtual bool Start() override;
};

// ---------------------------------
//			 ServerService
// ---------------------------------
class ServerService : public Service
{
public:
	ServerService(NetworkAddress address, IocpCoreRef core, SessionFactory factory, int32 maxSessionCount = 1);
	virtual ~ServerService();

	virtual bool	Start() override;
	virtual void	CloseService() override;

private:
	ListenerRef		_listener = { nullptr }; // 서버인 경우 Listener가 필요하므로 추가
};