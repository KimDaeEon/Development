using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GameServer
{
    // 주체적으로 상호 작용
    // 충돌 처리
    public class Actor : Entity
    {
        private EntityComposite<IEntityComponent> _actorComponents;

        public Actor()
        {
            var skillComponent = new SkillComponent();
            _actorComponents = new EntityComposite<IEntityComponent>(
                skillComponent
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
