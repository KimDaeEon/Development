#include "pch.h"
#include "Room.h"
#include "Player.h"

void Player::SetActorInfo(const Protocol::ActorInfo& actorInfo)
{
	_actorInfo = actorInfo;
}

const Protocol::ActorInfo& Player::GetActorInfo() const
{
	return _actorInfo;
}

void Player::SetOwnerSession(ClientSessionRef session)
{
	_ownerSession = session;
}

ClientSessionRef Player::GetOwnerSession() const
{
	return _ownerSession.lock();
}

void Player::SetRoom(std::shared_ptr<Room> room)
{
	LockGuard lock(_lock);
	_room = room; // atomic 사용 시에 store, load 사용
}

std::shared_ptr<Room> Player::GetRoom()
{
	LockGuard lock(_lock);
	return _room.lock();
}
