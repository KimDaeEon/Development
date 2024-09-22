#include "pch.h"
#include "Actor.h"

Actor::Actor() : Entity(), _actorComponents(CreateComponents<ActorComponentsPack>())
{
}
