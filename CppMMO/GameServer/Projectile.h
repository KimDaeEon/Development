#pragma once
#include "AIComponent.h"
// TODO: 추후 Component에 추가할 것 고민해서 추가 필요
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