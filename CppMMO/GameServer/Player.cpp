#include "pch.h"
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
	return _ownerSession;
}
