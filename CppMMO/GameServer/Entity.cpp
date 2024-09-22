#include "pch.h"
#include "Entity.h"

Entity::Entity() : _entityComponents(CreateComponents<EntityComponentsPack>())
{
	
}
