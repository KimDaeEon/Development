#include "pch.h"
#include "Service.h"
#include "Session.h"
#include "Listener.h"

Service::Service(ServiceType type, NetworkAddress address, IocpCoreRef core, SessionFactory factory, int32 maxSessionCount)
	: _type(type), _netAddress(address), _iocpCore(core), _sessionFactory(factory), _maxSessionCount(maxSessionCount)
{
}

Service::~Service()
{
}

void Service::CloseService()
{
	// TODO
}

void Service::Broadcast(SendBufferRef sendBuffer)
{
	WRITE_LOCK;
	for (const auto& session : _sessions)
	{
		session->Send(sendBuffer);
	}
}

SessionRef Service::CreateSession()
{
	SessionRef session = _sessionFactory();
	session->SetSerivce(shared_from_this()); // 세션을 관리하고 있는 서비스 등록

	if (_iocpCore->Register(session) == false)
	{
		return nullptr;
	}

	return session;
}

void Service::AddSession(SessionRef session)
{
	WRITE_LOCK;
	_sessionCount++;
	_sessions.insert(session);	
}

void Service::ReleaseSession(SessionRef session)
{
	WRITE_LOCK;
	ASSERT_CRASH(_sessions.erase(session) != 0);
	_sessionCount--;
}


// ---------------------------------
//			 ClientService
// ---------------------------------
ClientService::ClientService(NetworkAddress targetAddress, IocpCoreRef core, SessionFactory factory, int32 maxSessionCount)
	: Service(ServiceType::Client, targetAddress, core, factory, maxSessionCount)
{
}

ClientService::~ClientService()
{
}

bool ClientService::Start()
{
	if (CanStart() == false)
	{
		return false;
	}

	const int32 sessionCount = GetMaxSessionCount();
	for (int32 i = 0; i < sessionCount; i++)
	{
		SessionRef session = CreateSession();
		if (session->Connect() == false)
		{
			return false;
		}
	}

	return true;
}


// ---------------------------------
//			 ServerService
// ---------------------------------
ServerService::ServerService(NetworkAddress address, IocpCoreRef core, SessionFactory factory, int32 maxSessionCount)
	: Service(ServiceType::Server, address, core, factory, maxSessionCount)
{
}

ServerService::~ServerService()
{
}

bool ServerService::Start()
{
	if (CanStart() == false)
	{
		return false;
	}

	_listener = myMakeShared<Listener>();
	if (_listener == nullptr)
	{
		// TODO: 크리티컬 로그 필요, 메모리 부족
		return false;
	}

	ServerServiceRef service = static_pointer_cast<ServerService>(shared_from_this());
	if (_listener->StartAccept(service) == false)
	{
		return false;
	}

	return true;
}

void ServerService::CloseService()
{
	// TODO: 무언가 서버 종료 시에 추가할 작업이 생긴다면 여기에 추가
	Service::CloseService();
}
