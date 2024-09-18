#pragma once
#include "StatusComponent.h"

using EntityComponentsPack = StatusComponent;

class Entity
{
public:
    Entity()
        : _entityComponents(CreateComponents<EntityComponentsPack>())
    {
    }

    virtual void Init() {}
    virtual void Tick() {}

    // StatusComponent�� ������ Composite���� ������Ʈ ��������
    template <typename T>
    T& GetComponent()
    {
        return _entityComponents.GetComponent<T>();
    }

protected:
    // StatusComponent�� �����ϴ� Composite
    Composite<EntityComponentsPack> _entityComponents;

    // Entity�� Transform ����
    Protocol::Transform _transform;
};