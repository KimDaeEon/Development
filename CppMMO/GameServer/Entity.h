#pragma once
#include "StatusComponent.h"

using EntityComponentsPack = StatusComponent;

class Entity
{
public:
    Entity();

    virtual void Init() {}
    virtual void Tick() {}

    // StatusComponent를 포함한 Composite에서 컴포넌트 가져오기
    template <typename T>
    T& GetComponent()
    {
        return _entityComponents.GetComponent<T>();
    }


    virtual const Protocol::EntityInfo& GetEntityInfo() const
    {
        return _entityInfo;
    }
    // TODO: 추후에 컴포넌트 만들어지면 적용 필요.. 근데 이거 필요 없을 수도?
	virtual void SetEntityInfo(const Protocol::EntityInfo& entityInfo)
	{
		_entityInfo = entityInfo;
	}


public:
	// EntityInfo 관련 get, set 함수들
	uint64 GetGameId() const { return _entityInfo.gameid(); }
	void SetGameid(uint64 gameId) { _entityInfo.set_gameid(gameId); }


	const Protocol::MoveInfo& GetMoveInfo() const { return _entityInfo.moveinfo(); }
    void SetMoveInfo(const Protocol::MoveInfo& moveInfo)
    {
        _entityInfo.mutable_moveinfo()->CopyFrom(moveInfo);
    }


	const Protocol::Transform& GetTransform() const { return _entityInfo.moveinfo().transform(); }
    void SetTransform(const Protocol::Transform& transform)
    {
        _entityInfo.mutable_moveinfo()->mutable_transform()->CopyFrom(transform);
    }

    const Protocol::MoveState& GetMoveState() const { return _entityInfo.moveinfo().movestate(); }
    void SetMoveState(const Protocol::MoveState& moveState)
    {
        _entityInfo.mutable_moveinfo()->set_movestate(moveState);
    }


protected:
    // StatusComponent만 포함하는 Composite
    Composite<EntityComponentsPack> _entityComponents;

	Protocol::EntityInfo _entityInfo;
};