using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Google.Protobuf;
using Protocol = Google.Protobuf.Protocol;


namespace GameServer
{
    public class Entity
    {
        protected EntityComposite<IEntityComponent> _entityComponents;
        protected Protocol.EntityInfo _entityInfo; // TODO: Protocol에 추가 or 프로토콜 내용 적용 필요

        public Entity()
        {
            _entityComponents = new EntityComposite<IEntityComponent>(
                new StatusComponent() // Entity는 StatusComponent를 포함
            );

            _entityInfo = new Protocol.EntityInfo();
        }

        public virtual void Init() { }
        public virtual void Tick() { }

        public T GetComponent<T>() where T : class, IEntityComponent
        {
            if (_entityComponents.TryGetComponent<T>(out var component))
            {
                return component;
            }

            throw new InvalidOperationException($"Component of type {typeof(T).Name} does not exist in Entity.");
        }

        public Protocol.EntityInfo GetEntityInfo()
        {
            return _entityInfo;
        }

        public void SetEntityInfo(Protocol.EntityInfo entityInfo)
        {
            _entityInfo = entityInfo;
        }

        public ulong GetGameId() => _entityInfo.GameId;
        public void SetGameId(ulong gameId) => _entityInfo.GameId = gameId;
    }
}
