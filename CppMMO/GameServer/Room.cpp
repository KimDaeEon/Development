#include "pch.h"
#include "Room.h"
#include "Player.h"
#include "ClientSession.h"

void Room::Enter(PlayerRef player)
{
	_players[player->GetActorInfo().gameid()] = player;
}

void Room::Leave(PlayerRef player)
{
	_players.erase(player->GetActorInfo().gameid());
}