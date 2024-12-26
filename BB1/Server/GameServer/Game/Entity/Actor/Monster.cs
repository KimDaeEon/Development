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
            var aiComponent = new AIComponent();
            // TODO: 여기서 EntityInfo 로 추후에 할당할 거 있으면 할당하기.

            _monsterComponents = new EntityComposite<IEntityComponent>(
                aiComponent
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
