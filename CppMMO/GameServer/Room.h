#pragma once
#include "JobQueue.h"

class Room : public JobQueue
{
	friend class ClientPacketHandler;
	friend class ClientSession;

private:
	// Job���� ����Ǵ� �Լ�
	void Enter(PlayerRef player);
	void Leave(PlayerRef player);
	
	template<typename T>
	void Broadcast(T pkt)
	{
		for (auto& [playerId, player] : _players)
		{
			player->_ownerSession->SendPacket(pkt);
		}
	}

private:
	USE_LOCK;
	unordered_map<uint64, PlayerRef> _players;
};