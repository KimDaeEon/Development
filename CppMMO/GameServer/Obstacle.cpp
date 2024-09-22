#include "pch.h"
#include "Entity.h"
#include "Obstacle.h"

Obstacle::Obstacle()
	: Entity(), _obstacleComponents(CreateComponents<ProjectileComponentsPack>())
{
}
