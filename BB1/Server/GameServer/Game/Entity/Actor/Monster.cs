using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GameServer
{
    public class Monster : Actor
    {
        private EntityComposite<IEntityComponent> _monsterComponents;

        public Monster()
        {
            _monsterComponents = new EntityComposite<IEntityComponent>(
                //new AIComponent() // Monster는 AIComponent를 포함 // TODO: AIComponent 추가 필요
            );
        }

        public new T GetComponent<T>() where T : class, IEntityComponent
        {
            if (_monsterComponents.TryGetComponent<T>(out var component))
            {
                return component;
            }

            return base.GetComponent<T>();
        }
    }
}
