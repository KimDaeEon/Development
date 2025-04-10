using System;
using Google.Protobuf;
using Protocol = Google.Protobuf.Protocol;

namespace GameServer
{
    public abstract class Entity
    {
        protected EntityComposite _entityComponents;
        protected Object _lock = new object();

        protected Entity()
        {
            _entityComponents = new EntityComposite();
            _entityComponents.Owner = this;

            // 기본 컴포넌트 추가 (필요하면 추가)
            _entityComponents.AddComponent(new StatusComponent());
            EntityInfo = new Protocol.EntityInfo();
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

        // https://stackoverflow.com/questions/5209623/is-a-reference-assignment-threadsafe
        // bool, char 같이 작은 데이터들은 assign 이 thread-safe 하다지만 다른 경우는 아니라고 한다.
        // 참조가 포인터이고 테스트해보니 assign 자체는 thread-safe 한 거 같은데.. 가시성 문제도 있다고 하여서 일단 lock 을 잡자..
        GameRoom _gameRoom;
        public GameRoom Room
        {
            get
            {
                lock (_lock)
                {
                    return _gameRoom;
                }
            }
            set
            {
                lock (_lock)
                {
                    _gameRoom = value;
                }
            }
        }

        public int ExtraCells { get; protected set; } = 0; // 여러 칸을 차지하는 Entity의 경우에 사용

        public void SetCellPosToPosition(Vector2Int pos)
        {
            MoveInfo.Position.X = pos.x;
            MoveInfo.Position.Y = pos.y;
        }

        public Vector2Int GetCellPosFromPosition()
        {
            return new Vector2Int(MoveInfo.Position.X, MoveInfo.Position.Y);
        }

        // TODO: 서버 쪽은 클라이언트가 준 것을 그대로 반영하면 안되기에, 각각의 Component에서 EntityInfo에 정보 세팅하도록 해야 한다.
        public Protocol.EntityInfo EntityInfo { get; protected set; }

        public Protocol.EntityInfo EntityInfoForMove
        {
            get
            {
                var entityInfoForMove = new Protocol.EntityInfo();
                entityInfoForMove.MoveInfo = MoveInfo.Clone();
                entityInfoForMove.GameId = GameId;

                return entityInfoForMove;
            }
        }

        public Protocol.MoveInfo MoveInfo
        {
            get
            {
                return EntityInfo.MoveInfo;
            }
            set
            {
                EntityInfo.MoveInfo = value;
            }
        }

        public Protocol.StatInfo StatInfo
        {
            get
            {
                return EntityInfo.StatInfo;
            }
            set
            {
                EntityInfo.StatInfo = value;
            }
        }

        public ulong DataSheetId
        {
            get
            {
                return EntityInfo.DataSheetId;
            }
            set
            {
                EntityInfo.DataSheetId = value;
            }
        }

        public ulong GameId
        {
            get
            {
                return EntityInfo.GameId;
            }
            set
            {
                EntityInfo.GameId = value;
            }
        }

        public virtual void Tick() { }
    }
}
