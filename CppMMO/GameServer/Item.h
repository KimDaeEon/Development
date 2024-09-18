#pragma once
#include "Entity.h"
#include "SkillComponent.h"

using ItemComponentsPack = SkillComponent;

class Item : public Entity
{
public:
    Item()
        : Entity(), _itemComponents(CreateComponents<ItemComponentsPack>())
    {
    }


    template <typename T>
    T& GetComponent()
    {
        if constexpr (is_one_of<T, ItemComponentsPack>)
        {
            return _itemComponents.GetComponent<T>();
        }
        else
        {
            return Entity::GetComponent<T>();
        }
    }

protected:
    Composite<ItemComponentsPack> _itemComponents;
};