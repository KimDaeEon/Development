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

	// TODO: 이 아래 부분 실패했을 때에 Enum 추가해서 결과 주는 것으로 변경
	if (Enter(player) == false)
	{
		return;
	}

	// 입장 사실을 자기 자신에게 알린다.
	{
		Protocol::S_ENTER_GAME enterGamePkt;
		enterGamePkt.mutable_playercharacter()->CopyFrom(player->GetActorInfo());
		enterGamePkt.set_success(true);
		clientSession->SendPacket(enterGamePkt);
	}

	// 기존 입장한 플레이어 목록을 자신에게 알린다.
	{
		Protocol::S_SPAWN spawnPkt;
		for (auto& [id, actor] : _players)
		{
			Protocol::ActorInfo* actorInfo = spawnPkt.add_actors();
			actorInfo->CopyFrom(actor->GetActorInfo());
		}
		clientSession->SendPacket(spawnPkt);
	}

	// 입장 사실을 다른 플레이어들에게도 알린다.
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

	// TODO: 이 아래 부분 실패했을 때에 Enum 추가해서 결과 주는 것으로 변경
	if (Leave(player) == false)
	{
		return;
	}

	const uint64 gameId = player->GetActorInfo().gameid();

	// 퇴장 사실을 자기 자신에게 알린다.
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

	// TODO: 움직임 검증 로직 추가 필요
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
	// TODO: 아래 문제 없을 것 같은데, 혹시 모르니 추후 다시 확인
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