#pragma once
struct Player
{
	uint64					playerId = 0;
	string					name;
	Protocol::ActorType		type = Protocol::PLAYER_TYPE_NONE;
	ClientSessionRef		ownerSession; // cycle 발생 유의
};

