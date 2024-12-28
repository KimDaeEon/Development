using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;
using Google.Protobuf;
using Protocol = Google.Protobuf.Protocol;


namespace GameServer
{
    public class Entity
    {
        protected EntityComposite<IEntityComponent> _entityComponents;
        protected Protocol.EntityInfo _entityInfo; // 모든 Entity 관련 정보는 Entity 객체에서 관리, 각 컴포넌트는 Owner를 통해 _entityInfo에서 본인 담당 정보를 업데이트
        // 뭔가 EntityInfo에 들어갈 내용 중 서버에만 따로 필요한 것이 생긴다면 각 Component가 추가 데이터를 들고 있도록 하자.

        public GameRoom Room { get; set; }

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

        protected ulong DataSheetId
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

        public Entity()
        {
            // 컴포넌트 추가가 필요하면 여기서 추가하고, _entityComponents에 등록
            var statusComponent = new StatusComponent();
            statusComponent.Owner = this;

            _entityComponents = new EntityComposite<IEntityComponent>(
                statusComponent
            );

            _entityInfo = new Protocol.EntityInfo();
        }

        // Entity 상속받는 클래스별로 DataSheetId 참조해서 필요한 데이터들 세팅 ex) _entityInfo의 StatInfo
        public virtual void LoadDataFromDataSheet()
        {
            if( DataSheetId == 0)
            {
                Console.WriteLine($"You must set DataSheetId before call {GameServer.Utils.GetCurrentFunctionName()}");
                throw new Exception("DataSheetId Exception");
            }

            // TODO: DataSheet를 기반으로 세팅해야할 부분 세팅
            MoveInfo = new Protocol.MoveInfo();

            MoveInfo.ActorState = Protocol.EActorState.ActorStateIdle;
            int randNum = Random.Shared.Next(-10, 10);
            MoveInfo.Position = new Protocol.Position { X = randNum, Y = randNum, Z = 0 };

            // TODO: DataSheet를 기반으로 세팅해야할 부분 세팅, DB 데이터 로딩하고, 데이터 Sheet 부분 적용하는 타이밍도 고민해보기
            StatInfo = new Protocol.StatInfo
            {
                Level = 0,
                Hp = 100,
                Mp = 100,
                Attack = 10,
                Defense = 10,
                Speed = 10,
                CurrentExp = 0,
            };


            _entityInfo.MoveInfo = MoveInfo;
            _entityInfo.StatInfo = StatInfo;


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
