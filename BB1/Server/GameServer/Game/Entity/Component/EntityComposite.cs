using System;
using System.Collections.Generic;

namespace GameServer
{
    public class EntityComposite
    {
        private readonly Dictionary<Type, IEntityComponent> _components = new Dictionary<Type, IEntityComponent>();
        public Entity Owner { get; set; } // Owner 참조

        public EntityComposite() { }

        public T AddComponent<T>(T component) where T : class, IEntityComponent
        {
            if (component == null)
            {
                throw new ArgumentNullException(nameof(component));
            }

            Type type = typeof(T);
            if (_components.ContainsKey(type))
            {
                throw new InvalidOperationException($"Component of type {type.Name} already exists.");
            }

            _components[type] = component;
            component.Owner = Owner;

            return component;
        }

        public bool TryGetComponent<T>(out T component) where T : class, IEntityComponent
        {
            if (_components.TryGetValue(typeof(T), out var foundComponent))
            {
                component = foundComponent as T;
                return component != null;
            }

            component = null;
            return false;
        }

        public T GetComponent<T>() where T : class, IEntityComponent
        {
            if (TryGetComponent<T>(out var component))
            {
                return component;
            }

            throw new InvalidOperationException($"Component of type {typeof(T).Name} does not exist.");
        }
    }
}
