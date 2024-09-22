#pragma once
#include "StatusComponent.h"

using EntityComponentsPack = StatusComponent;

class Entity
{
public:
    Entity();

    virtual void Init() {}
    virtual void Tick() {}

    // StatusComponent�� ������ Composite���� ������Ʈ ��������
    template <typename T>
    T& GetComponent()
    {
        return _entityComponents.GetComponent<T>();
    }


    virtual const Protocol::EntityInfo& GetEntityInfo() const
    {
        return _entityInfo;
    }
    // TODO: ���Ŀ� ������Ʈ ��������� ���� �ʿ�.. �ٵ� �̰� �ʿ� ���� ����?
	virtual void SetEntityInfo(const Protocol::EntityInfo& entityInfo)
	{
		_entityInfo = entityInfo;
	}


public:
	// EntityInfo ���� get, set �Լ���
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
    // StatusComponent�� �����ϴ� Composite
    Composite<EntityComponentsPack> _entityComponents;

	Protocol::EntityInfo _entityInfo;
};