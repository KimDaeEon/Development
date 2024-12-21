using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GameServer
{
    public class Item : Entity
    {
        private EntityComposite<IEntityComponent> _itemComponents;

        public Item()
        {
            _itemComponents = new EntityComposite<IEntityComponent>(
                //new SkillComponent() // Item도 SkillComponent를 포함
            );
        }

        public new T GetComponent<T>() where T : class, IEntityComponent
        {
            if (_itemComponents.TryGetComponent<T>(out var component))
            {
                return component;
            }

            return base.GetComponent<T>();
        }
    }
}
