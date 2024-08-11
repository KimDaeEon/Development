#pragma once
#include "JobQueue.h"

class Room : public JobQueue
{
	friend class ClientPacketHandler;
	friend class ClientSession;
	friend class JobQueue;

private:
	// Job에서 실행되는 함수
	void HandleEnterGame(ClientSessionRef clientSession, PlayerRef player);
	void Enter(PlayerRef player);
	void Leave(PlayerRef player);
	
	template<typename T>
	void Broadcast(T pkt)
	{
		for (auto& [_playerId, _player] : _players)
		{
			auto session = _player->GetOwnerSession();
			if (session)
			{
				session->SendPacket(pkt);
			}
		}
	}

	template<typename T>
	void BroadcastExceptPlayer(T pkt, uint64 playerId)
	{
		for (auto& [_playerId, _player] : _players)
		{
			if (playerId == _playerId)
				continue;

			auto session = _player->GetOwnerSession();
			if (session)
			{
				session->SendPacket(pkt);
			}
		}
	}

private:
	USE_LOCK;
	unordered_map<uint64, PlayerRef> _players;
};