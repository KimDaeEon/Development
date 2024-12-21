using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GameServer
{
    public class Projectile : Entity
    {
        private EntityComposite<IEntityComponent> _projectileComponents;

        public Projectile()
        {
            _projectileComponents = new EntityComposite<IEntityComponent>(
                //new AIComponent() // Projectile도 AIComponent를 포함
            );
        }

        public new T GetComponent<T>() where T : class, IEntityComponent
        {
            if (_projectileComponents.TryGetComponent<T>(out var component))
            {
                return component;
            }

            return base.GetComponent<T>();
        }
    }
}
