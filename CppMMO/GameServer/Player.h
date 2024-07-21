#pragma once
class Player
{
public:
	void SetActorInfo(const Protocol::ActorInfo& actorInfo);
	const Protocol::ActorInfo& GetActorInfo() const;

	void SetOwnerSession(ClientSessionRef session);
	ClientSessionRef GetOwnerSession() const;

private:
	Protocol::ActorInfo		_actorInfo;
	ClientSessionRef		_ownerSession; // cycle 발생 유의
};

