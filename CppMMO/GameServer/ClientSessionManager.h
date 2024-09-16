#pragma once

class ClientSession;

using ClientSessionRef = std::shared_ptr<ClientSession>;

class ClientSessionManager
{
public:
	void Add(ClientSessionRef session);
	void Remove(ClientSessionRef session);
	void Broadcast(SendBufferRef sendBuffer);

private:
	Mutex _lock;
	std::set<ClientSessionRef> _sessions;
};

extern ClientSessionManager GClientSessionManager;
