#pragma once
#include "JobQueue.h"

class Room : public JobQueue
{
	friend class ClientPacketHandler;
	friend class ClientSession;
	friend class JobQueue;

public:
		RoomRef GetRoomRef();

private:
	// Job에서 실행되는 함수
	void HandleEnterGame(ClientSessionRef clientSession, PlayerRef player);
	void HandleLeaveGame(ClientSessionRef clientSession, PlayerRef player);
	void HandleMove(ClientSessionRef clientSession, PlayerRef player, const Protocol::C_MOVE& pkt);

	bool Enter(PlayerRef player);
	bool Leave(PlayerRef player);
	
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