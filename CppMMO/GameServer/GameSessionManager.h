#pragma once

class GameSession;

using GameSessionRef = std::shared_ptr<GameSession>;

class GameSessionManager
{
public:
	void Add(GameSessionRef session);
	void Remove(GameSessionRef session);
	void Broadcast(SendBufferRef sendBuffer);

private:
	USE_LOCK;
	std::set<GameSessionRef> _sessions;
};

extern GameSessionManager GSessionManager;
