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

    // Actor�� SkillComponent�� StatusComponent�� ��� ���� ����
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
    // SkillComponent�� �����ϴ� Composite
    Composite<MonsterComponentPack> _monsterComponents;
};