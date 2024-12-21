using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GameServer
{
    public class Obstacle : Entity
    {
        private EntityComposite<IEntityComponent> _obstacleComponents;

        public Obstacle()
        {
            _obstacleComponents = new EntityComposite<IEntityComponent>(
                //new AIComponent() // Obstacle도 AIComponent를 포함
            );
        }

        public new T GetComponent<T>() where T : class, IEntityComponent
        {
            if (_obstacleComponents.TryGetComponent<T>(out var component))
            {
                return component;
            }

            return base.GetComponent<T>();
        }
    }
}
