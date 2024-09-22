#pragma once
#include "Actor.h"
#include "QuestComponent.h"

class Room;

//using PlayerComponentsPack = QuestComponent;
//
//class Player : public Actor
//{
//public:
//    Player()
//        : Actor(), _playerComponents(CreateComponents<PlayerComponentsPack>())
//    {
//    }
//
//
//    // Actor는 SkillComponent와 StatusComponent에 모두 접근 가능
//    template <typename T>
//    T& GetComponent()
//    {
//        if constexpr (is_one_of<T, PlayerComponentsPack>)
//        {
//            return _playerComponents.GetComponent<T>();
//        }
//        else
//        {
//            return Actor::GetComponent<T>();
//        }
//    }
//
//protected:
//    
//    Composite<PlayerComponentPack> _playerComponents;
//};

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
	Protocol::ActorInfo				_playerEntityInfo;
	std::weak_ptr<ClientSession>	_ownerSession; 
	
	std::weak_ptr<Room>		_room;
	Mutex					_lock;
};

