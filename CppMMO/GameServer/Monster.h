#pragma once
#include "Actor.h"
#include "AIComponent.h"

using MonsterComponentPack = AIComponent;

class Monster : public Actor
{
public:
    Monster()
        : Actor(), _monsterComponents(CreateComponents<MonsterComponentPack>())
    {
    }

    // Actor는 SkillComponent와 StatusComponent에 모두 접근 가능
    template <typename T>
    T& GetComponent()
    {
        if constexpr (is_one_of<T, MonsterComponentPack>)
        {
            return _monsterComponents.GetComponent<T>();
        }
        else
        {
            return Actor::GetComponent<T>();
        }
    }

protected:
    // SkillComponent를 포함하는 Composite
    Composite<MonsterComponentPack> _monsterComponents;
};