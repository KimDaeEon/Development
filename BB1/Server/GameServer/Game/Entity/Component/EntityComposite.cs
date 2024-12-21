using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GameServer
{
    // EntityComposite 클래스 정의
    public class EntityComposite<T> where T : IEntityComponent
    {
        private readonly Dictionary<Type, IEntityComponent> _components = new Dictionary<Type, IEntityComponent>();

        public EntityComposite(params IEntityComponent[] components)
        {
            foreach (var component in components)
            {
                _components[component.GetType()] = component;
            }
        }

        public bool TryGetComponent<TComponent>(out TComponent component) where TComponent : class, IEntityComponent
        {
            if (_components.TryGetValue(typeof(TComponent), out var foundComponent))
            {
                component = foundComponent as TComponent;
                return component != null;
            }

            component = null;
            return false;
        }

        public TComponent GetComponent<TComponent>() where TComponent : class, IEntityComponent
        {
            if (TryGetComponent<TComponent>(out var component))
            {
                return component;
            }

            throw new InvalidOperationException($"Component of type {typeof(TComponent).Name} does not exist.");
        }
    }
}
