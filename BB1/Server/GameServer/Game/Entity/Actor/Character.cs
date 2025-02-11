using System;
using Google.Protobuf;
using Google.Protobuf.Protocol;
using Protocol = Google.Protobuf.Protocol;

namespace GameServer
{
    // 사용자가 조종하는 Actor 이기에 Session 을 통한 통신 기능이 존재
    public class Character : Actor
    {
        public ClientSession Session { get; set; }

        protected Character()
        {
            AddComponent(new QuestComponent()); // ✅ 필요할 때만 추가
        }

        // TODO: DB 관련 로직 추후 아래에서 처리
        public void LoadDbData()
        {
        }

        public override void Init(ulong dataSheetId)
        {
            base.Init(dataSheetId);

            // TODO: 데이터 세팅, 일단 지금은 기본 캐릭터 정보로 넣자.

            // 기본 EntityInfo 세팅
            EntityInfo = new Protocol.EntityInfo
            {
                GameId = this.GameId, // 게임 ID 설정
                DataSheetId = dataSheetId, // 데이터 시트 ID 설정
                MoveInfo = new Protocol.MoveInfo
                {
                    ActorState = Protocol.EActorState.ActorStateIdle, // 기본 상태 설정
                    Position = new Protocol.Position { X = 0, Y = 0 } // 기본 위치 설정
                },
                StatInfo = new Protocol.StatInfo
                {
                    Level = 1,
                    Hp = 100,
                    Mp = 50,
                    Attack = 10,
                    Defense = 5,
                    Speed = 10,
                    CurrentExp = 0
                },

                ActorInfo = new Protocol.ActorInfo
                {
                    CharacterInfo = new Protocol.CharacterInfo
                    {
                        Name = "DefaultCharacter" + this.GameId, // 기본 이름 설정
                        Type = ECharacterType.CharacterTypeWarrior,
                    }
                }
            };
        }
    }
}
