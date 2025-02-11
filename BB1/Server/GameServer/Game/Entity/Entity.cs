using System;
using Google.Protobuf;
using Protocol = Google.Protobuf.Protocol;

namespace GameServer
{
    public abstract class Entity
    {
        protected EntityComposite _entityComponents;
        protected Protocol.EntityInfo _entityInfo;

        protected Entity()
        {
            _entityComponents = new EntityComposite();
            _entityComponents.Owner = this;

            // 기본 컴포넌트 추가 (필요하면 추가)
            _entityComponents.AddComponent(new StatusComponent());
            _entityInfo = new Protocol.EntityInfo();
        }

        public T AddComponent<T>(T component) where T : class, IEntityComponent
        {
            return _entityComponents.AddComponent(component);
        }

        public T GetComponent<T>() where T : class, IEntityComponent
        {
            if (_entityComponents.TryGetComponent<T>(out var component))
            {
                return component;
            }

            throw new InvalidOperationException($"Component of type {typeof(T).Name} does not exist in Entity.");
        }

        public abstract void Init(ulong dataSheetId);

        public GameRoom Room { get; set; }

        public int ExtraCells { get; protected set; } = 0; // 여러 칸을 차지하는 Entity의 경우에 사용

        public void SetVector2IntToPosition(Vector2Int pos)
        {
            MoveInfo.Position.X = pos.x;
            MoveInfo.Position.Y = pos.y;
        }

        public Vector2Int GetVector2IntFromPosition()
        {
            return new Vector2Int(MoveInfo.Position.X, MoveInfo.Position.Y);
        }

        public Protocol.EntityInfo EntityInfo
        {
            get
            {
                return _entityInfo;
            }
            set
            {
                _entityInfo = value;
            }
        }

        public Protocol.MoveInfo MoveInfo
        {
            get
            {
                return _entityInfo.MoveInfo;
            }
            set
            {
                _entityInfo.MoveInfo = value;
            }
        }

        public Protocol.StatInfo StatInfo
        {
            get
            {
                return _entityInfo.StatInfo;
            }
            set
            {
                _entityInfo.StatInfo = value;
            }
        }

        public ulong DataSheetId
        {
            get
            {
                return _entityInfo.DataSheetId;
            }
            set
            {
                _entityInfo.DataSheetId = value;
            }
        }

        public ulong GameId
        {
            get
            {
                return _entityInfo.GameId;
            }
            set
            {
                _entityInfo.GameId = value;
            }
        }

        public virtual void Tick() { }
    }
}
