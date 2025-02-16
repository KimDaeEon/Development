using System;
using Google.Protobuf;
using Protocol = Google.Protobuf.Protocol;

namespace GameServer
{
    public abstract class Entity
    {
        protected EntityComposite _entityComponents;
        protected Protocol.EntityInfo _entityInfo;
        protected Object _lock = new object();

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
                if (value == null)
                {
                    return;
                }

                // Entity 기본 정보 할당
                if (value.GameId != 0)
                {
                    _entityInfo.GameId = value.GameId;
                }

                if (value.DataSheetId != 0)
                {
                    _entityInfo.DataSheetId = value.DataSheetId;
                }

                if (value.MoveInfo != null)
                {
                    _entityInfo.MoveInfo = value.MoveInfo;
                }

                if (value.StatInfo != null)
                {
                    _entityInfo.StatInfo = value.StatInfo;
                }

                // ENtity 하위 클래스 정보에 따라 할당
                if (value.EntityTypeCase == Protocol.EntityInfo.EntityTypeOneofCase.ActorInfo)
                {
                    _entityInfo.ActorInfo = value.ActorInfo;
                }
                else if (value.EntityTypeCase == Protocol.EntityInfo.EntityTypeOneofCase.ItemInfo)
                {
                    _entityInfo.ItemInfo = value.ItemInfo;
                }
                else if (value.EntityTypeCase == Protocol.EntityInfo.EntityTypeOneofCase.ProjectileInfo)
                {
                    _entityInfo.ProjectileInfo = value.ProjectileInfo;
                }
                else if (value.EntityTypeCase == Protocol.EntityInfo.EntityTypeOneofCase.ObstacleInfo)
                {
                    _entityInfo.ObstacleInfo = value.ObstacleInfo;
                }
            }
        }

        public Protocol.EntityInfo EntityInfoForMove
        {
            get
            {
                var entityInfoForMove = new Protocol.EntityInfo();
                entityInfoForMove.MoveInfo = MoveInfo;
                entityInfoForMove.GameId = GameId;

                return entityInfoForMove;
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
