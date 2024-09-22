#pragma once
#include "AIComponent.h"
// TODO: ���� Component�� �߰��� �� ����ؼ� �߰� �ʿ�
using ProjectileComponentsPack = AIComponent;

class Projectile : public Entity
{
public:
	Projectile();

	template <typename T>
	T& GetComponent()
	{
		if constexpr (is_one_of<T, ProjectileComponentsPack>)
		{
			return _projectileComponents.GetComponent<T>();
		}
		else
		{
			return Entity::GetComponent<T>();
		}
	}

protected:
	Composite<ProjectileComponentsPack> _projectileComponents;
};