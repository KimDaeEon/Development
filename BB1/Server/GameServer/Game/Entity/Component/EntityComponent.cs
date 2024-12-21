using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GameServer
{
    using System;
    using System.Collections.Generic;

    // EventData 정의
    public enum EventDataType { Int, Float, String, Example }

    public struct EventData
    {
        public EventDataType Type;
        public object Data;
    }

    // IEntityComponent 인터페이스
    public interface IEntityComponent
    {
        void Update();
        void Notify(EventData message);
        void OnNotify(EventData message);
    }

    // 기본 EntityComponent 클래스
    public abstract class EntityComponent : IEntityComponent
    {
        private readonly List<EntityComponent> _observers = new List<EntityComponent>();

        public abstract void Update();
        public abstract void Notify(EventData message);
        public abstract void OnNotify(EventData message);

        public void Subscribe(EntityComponent component)
        {
            _observers.Add(component);
        }

        public void NotifyObservers(EventData message)
        {
            foreach (var observer in _observers)
            {
                observer.OnNotify(message);
            }
        }
    }
}
