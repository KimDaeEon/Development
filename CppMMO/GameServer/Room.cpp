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

	// TODO: �� �Ʒ� �κ� �������� ���� Enum �߰��ؼ� ��� �ִ� ������ ����
	if (Enter(player) == false)
	{
		return;
	}

	// ���� ����� �ڱ� �ڽſ��� �˸���.
	{
		Protocol::S_ENTER_GAME enterGamePkt;
		enterGamePkt.mutable_playercharacter()->CopyFrom(player->GetActorInfo());
		enterGamePkt.set_success(true);
		clientSession->SendPacket(enterGamePkt);
	}

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

	// TODO: �� �Ʒ� �κ� �������� ���� Enum �߰��ؼ� ��� �ִ� ������ ����
	if (Leave(player) == false)
	{
		return;
	}

	const uint64 gameId = player->GetActorInfo().gameid();

	// ���� ����� �ڱ� �ڽſ��� �˸���.
	{
		Protocol::S_LEAVE_GAME leaveGamePkt;
		
		if (clientSession)
		{
			clientSession->SendPacket(leaveGamePkt);
		}
	}

	{
		Protocol::S_DESPAWN despawnPkt;
		despawnPkt.add_actorids(gameId);
		Broadcast(despawnPkt);

		if (clientSession)
		{
			clientSession->SendPacket(despawnPkt);
		}
	}

}

void Room::HandleMove(ClientSessionRef clientSession, PlayerRef player, const Protocol::C_MOVE& pkt)
{
	const uint64 gameId = player->GetActorInfo().gameid();

	// TODO: ������ ���� ���� �߰� �ʿ�
	player->SetActorInfo(pkt.actorinfo());

	{
		Protocol::S_MOVE movePkt;
		auto* actorInfo = movePkt.mutable_actorinfo();
		actorInfo->CopyFrom(player->GetActorInfo());

		cout << actorInfo->DebugString() << endl;
		Broadcast(movePkt);
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