using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GameServer
{
    public class Actor : Entity
    {
        private EntityComposite<IEntityComponent> _actorComponents;

        public Actor()
        {
            _actorComponents = new EntityComposite<IEntityComponent>(
                //new SkillComponent() // Actor는 SkillComponent를 포함 // TODO: SkillComponent 추가 필요
            );
        }

        public new T GetComponent<T>() where T : class, IEntityComponent
        {
            if (_actorComponents.TryGetComponent<T>(out var component))
            {
                return component;
            }

            return base.GetComponent<T>();
        }
    }
}
