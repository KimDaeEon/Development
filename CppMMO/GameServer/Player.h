#pragma once
class Room;


class Player : public std::enable_shared_from_this<Player>
{
public:
	void SetActorInfo(const Protocol::ActorInfo& actorInfo);
	const Protocol::ActorInfo& GetActorInfo() const;

	void SetOwnerSession(ClientSessionRef session);
	ClientSessionRef GetOwnerSession() const;

	void SetRoom(std::shared_ptr<Room> room);
	std::shared_ptr<Room> GetRoom();

private:
	Protocol::ActorInfo				_actorInfo;
	std::weak_ptr<ClientSession>	_ownerSession; 
	
	std::weak_ptr<Room>		_room;
	Mutex					_lock;
};

