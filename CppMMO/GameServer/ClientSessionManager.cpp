#include "pch.h"
#include "ClientSessionManager.h"
#include "ClientSession.h"

ClientSessionManager GClientSessionManager;

void ClientSessionManager::Add(ClientSessionRef session)
{
	LockGuard lg(_lock);
	_sessions.insert(session);
}

void ClientSessionManager::Remove(ClientSessionRef session)
{
	LockGuard lg(_lock);
	_sessions.erase(session);
}

void ClientSessionManager::Broadcast(SendBufferRef sendBuffer)
{
	LockGuard lg(_lock);
	for (ClientSessionRef session : _sessions)
	{
		if (session)
		{
			session->Send(sendBuffer);
		}
	}
}