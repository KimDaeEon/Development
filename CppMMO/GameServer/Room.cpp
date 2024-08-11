#include "pch.h"
#include "Room.h"
#include "Player.h"
#include "ClientSession.h"

void Room::HandleEnterGame(ClientSessionRef clientSession, PlayerRef player)
{
	Enter(player);

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

void Room::Enter(PlayerRef player)
{
	_players[player->GetActorInfo().gameid()] = player;
}

void Room::Leave(PlayerRef player)
{
	_players.erase(player->GetActorInfo().gameid());
}