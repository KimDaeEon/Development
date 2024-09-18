#pragma once
#include "Entity.h"
#include "SkillComponent.h"

using ActorComponentsPack = SkillComponent;

class Actor : public Entity
{
public:
    Actor()
        : Entity(), _actorComponents(CreateComponents<ActorComponentsPack>())
    {
    }


    template <typename T>
    T& GetComponent()
    {
        if constexpr (is_one_of<T, ActorComponentsPack>)
        {
            return _actorComponents.GetComponent<T>();
        }
        else
        {
            return Entity::GetComponent<T>();
        }
    }

protected:
    Composite<ActorComponentsPack> _actorComponents;
};