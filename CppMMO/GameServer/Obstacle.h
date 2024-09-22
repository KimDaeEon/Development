#pragma once
#include "AIComponent.h"
// TODO: ���� Component�� �߰��� �� ����ؼ� �߰� �ʿ�
using ProjectileComponentsPack = AIComponent;

class Obstacle : public Entity
{
public:
    Obstacle();

    template <typename T>
    T& GetComponent()
    {
        if constexpr (is_one_of<T, ProjectileComponentsPack>)
        {
            return _obstacleComponents.GetComponent<T>();
        }
        else
        {
            return Entity::GetComponent<T>();
        }
    }

protected:
    Composite<ProjectileComponentsPack> _obstacleComponents;
};