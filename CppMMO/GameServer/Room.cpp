#include "pch.h"
#include "Room.h"
#include "Player.h"
#include "ClientSession.h"

void Room::HandleEnterGame(ClientSessionRef clientSession, PlayerRef player)
{
	if (player == nullptr)
	{
		return;
	}

	if (Enter(player) == false)
	{
		return;
	}

	Protocol::S_ENTER_GAME enterGamePkt;
	enterGamePkt.set_success(true);
	clientSession->SendPacket(enterGamePkt);

	// ���� ������ �÷��̾� ����� �ڽſ��� �˸���.
	{
		Protocol::S_SPAWN spawnPkt;
		for (auto& [id, actor] : _players)
		{
			Protocol::ActorInfo* actorInfo = spawnPkt.add_actors();
			actorInfo->CopyFrom(actor->GetActorInfo());
		}
		clientSession->SendPacket(spawnPkt);
	}

	// ���� ����� �ٸ� �÷��̾�鿡�Ե� �˸���.
	{
		Protocol::S_SPAWN spawnPkt;
		Protocol::ActorInfo* actorInfo = spawnPkt.add_actors();
		actorInfo->CopyFrom(player->GetActorInfo());

		BroadcastExceptPlayer(spawnPkt, player->GetActorInfo().gameid());
	}
}

void Room::HandleLeaveGame(ClientSessionRef clientSession, PlayerRef player)
{
	if (player == nullptr)
	{
		return;
	}

	if (Leave(player) == false)
	{
		return;
	}

	const uint64 gameId = player->GetActorInfo().gameid();
	auto session = player->GetOwnerSession();

	// ���� ����� �ڱ� �ڽſ��� �˸���.
	{
		Protocol::S_LEAVE_GAME leaveGamePkt;
		
		if (session)
		{
			session->SendPacket(leaveGamePkt);
		}
	}

	{
		Protocol::S_DESPAWN despawnPkt;
		despawnPkt.add_actorids(gameId);
		Broadcast(despawnPkt);

		if (session)
		{
			session->SendPacket(despawnPkt);
		}
	}

}

bool Room::Enter(PlayerRef player)
{
	int64 gameId = player->GetActorInfo().gameid();
	if (_players.find(gameId) != _players.end())
	{
		return false;
	}

	_players[gameId] = player;
	// TODO: �Ʒ� ���� ���� �� ������, Ȥ�� �𸣴� ���� �ٽ� Ȯ��
	_players[gameId]->SetRoom(std::static_pointer_cast<Room>(shared_from_this()));

	return true;
}

bool Room::Leave(PlayerRef player)
{
	uint64 gameId = player->GetActorInfo().gameid();
	if (_players.find(gameId) == _players.end())
	{
		return false;
	}

	PlayerRef erasedPlayer = _players[gameId];
	erasedPlayer->SetRoom(nullptr);
	_players.erase(gameId);

	return true;
}