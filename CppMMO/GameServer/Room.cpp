#include "pch.h"
#include "Room.h"
#include "Player.h"
#include "ClientSession.h"

std::shared_ptr<Room> GRoom = myMakeShared<Room>(); // TODO: �̰��� �����ϴ� RoomManager �߰� �ʿ�

void Room::Enter(PlayerRef player)
{
	_players[player->playerId] = player;
}

void Room::Leave(PlayerRef player)
{
	_players.erase(player->playerId);
}

void Room::Broadcast(SendBufferRef sendBuffer)
{
	for (auto& [playerId, player]: _players)
	{
		player->ownerSession->Send(sendBuffer);
	}
}
