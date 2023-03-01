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

// Session, NetowrkAddress, IocpCore�� ���� ��� ������ �� ���� �����ϱ� ���� ��ü
// ���������� ����� �ϱ� ���� ��ҵ��� �� ���ִٰ� ���� �ȴ�.
// �̷��� �и��� ������ Iocp �ڵ��� Service ���� �и��Ͽ� Session�� �����ϰ� ���� ������ ���� ũ��.
class Service : public enable_shared_from_this<Service>
{
public:
	// address: Ŭ���̾�Ʈ��� ��� ������ �ּ�, ������� �ڽ��� �ּ�
	// maxSessionCount: ���� 1:1 �����̹Ƿ� �̷��� ����
	Service(ServiceType type, NetworkAddress address, IocpCoreRef core, SessionFactory factory, int32 maxSessionCount = 1);
	virtual ~Service();

	virtual bool	Start() abstract;
	bool			CanStart() { return _sessionFactory != nullptr; }

	virtual void	CloseService();
	void			SetSessionFactory(SessionFactory func) { _sessionFactory = func; }

	SessionRef		CreateSession(); // ���� ���� + Iocp�� ���
	void			AddSession(SessionRef session);
	void			ReleaseSession(SessionRef session);
	int32			GetCurrentSessionCount() { return _sessionCount; } // ���� lock�� �ȰǴ�. �뷫���� ���ڸ� �˸� �Ǳ⿡.. TODO: ���Ŀ� �̰� ������� ���� �ʿ� 
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
	ListenerRef		_listener = { nullptr }; // ������ ��� Listener�� �ʿ��ϹǷ� �߰�
};