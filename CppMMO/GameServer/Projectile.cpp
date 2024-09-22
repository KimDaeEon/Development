#include "pch.h"
#include "Entity.h"
#include "Projectile.h"

Projectile::Projectile()
	: Entity(), _projectileComponents(CreateComponents<ProjectileComponentsPack>())
{
}
