using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GameServer
{
    // 수동적인 상호 작용만 가능 (폭발, 파괴, 밀려서 이동 등등)
    // 충돌 처리
    public class Obstacle : Actor
    {
        private EntityComposite<IEntityComponent> _obstacleComponents;

        public Obstacle()
        {
            //_obstacleComponents = new EntityComposite<IEntityComponent>(
            //);
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
