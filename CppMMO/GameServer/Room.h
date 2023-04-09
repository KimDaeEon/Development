#pragma once
#include "JobQueue.h"

class Room : public JobQueue
{
	friend class ClientPacketHandler;
	friend class ClientSession;

private:
	// Job에서 실행되는 함수
	void Enter(PlayerRef player);
	void Leave(PlayerRef player);
	void Broadcast(SendBufferRef sendBuffer);

private:
	USE_LOCK;
	unordered_map<uint64, PlayerRef> _players;
};

extern std::shared_ptr<Room> GRoom; // TODO: 추후 RoomManager에서 관리하도록 변경 필요
