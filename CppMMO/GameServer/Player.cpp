#include "pch.h"
#include "Room.h"
#include "Player.h"

// TOOD: �Ʒ� EntityInfo ���� �������� ���� �ʿ�
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
	_room = room; // atomic ��� �ÿ� store, load ���
}

std::shared_ptr<Room> Player::GetRoom()
{
	LockGuard lock(_lock);
	return _room.lock();
}
