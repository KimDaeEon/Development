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

    // StatusComponent를 포함한 Composite에서 컴포넌트 가져오기
    template <typename T>
    T& GetComponent()
    {
        return _entityComponents.GetComponent<T>();
    }

protected:
    // StatusComponent만 포함하는 Composite
    Composite<EntityComponentsPack> _entityComponents;

    // Entity의 Transform 정보
    Protocol::Transform _transform;
};