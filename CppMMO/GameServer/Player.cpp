#include "pch.h"
#include "Room.h"
#include "Player.h"

// TOOD: 아래 EntityInfo 관련 내용으로 수정 필요
void Player::SetActorInfo(const Protocol::ActorInfo& actorInfo)
{
	//_playerEntityInfo.CopyFrom(actorInfo);
}

const Protocol::ActorInfo& Player::GetActorInfo() const
{
	return _playerEntityInfo;
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
