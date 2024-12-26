using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GameServer
{
    public class Chracter : Actor
    {
        private EntityComposite<IEntityComponent> _characterComponents;

        public Chracter()
        {
            var questComponent = new QuestComponent();

            // TODO: 여기서 EntityInfo 로 추후에 할당할 거 있으면 할당하기.

            _characterComponents = new EntityComposite<IEntityComponent>(
            questComponent
            );
        }

        public new T GetComponent<T>() where T : class, IEntityComponent
        {
            if (_characterComponents.TryGetComponent<T>(out var component))
            {
                return component;
            }

            return base.GetComponent<T>();
        }
    }
}