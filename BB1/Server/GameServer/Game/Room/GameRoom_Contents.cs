using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Google.Protobuf;
using Google.Protobuf.Protocol;
using Protocol = Google.Protobuf.Protocol;

namespace GameServer
{
    public partial class GameRoom : JobQueue
    {
        public void HandleEnterGame(ClientSession session, C_EnterGame pkt)
        {
            // TODO: 여기서 pkt의 character index 가지고 db 통해서 캐릭터 정보 불러온 후에,
            // 거기에 들어 있는 dataSheetId를 여기에 넣고 DB + DataSheet 데이터로 캐릭터 데이터 완성해야 함.
            // 일단 여기서는 임시로 넣기
            
            var character = EntityManager.Instance.Spawn<Character>(100);
            character.SetVector2IntToPosition(GetRandomSpawnPos(character));

            session.MyCharacter = character;
            session.MyCharacter.Room = this;
            
            // 선택한 캐릭터 정보 담아서 자신에게 S_EnterGame으로 전달
            S_EnterGame enterGamePkt = new S_EnterGame();
            enterGamePkt.MyCharInfo = session.MyCharacter.EntityInfo;
            enterGamePkt.Result = EResult.Success;
            session.Send(enterGamePkt);

            // 자기 자신은 뺄 거라서 해당 리스트에 추가
            List<Entity> excludedEntities = new List<Entity> { character };

            // 해당 Zone 주위의 Entity 정보 담아서 자신에게 전달
            {
                S_Spawn spawnPkt = new S_Spawn();
                spawnPkt.EntityInfos.AddRange(GetAdjacentEntityInfos(character.GetVector2IntFromPosition(), excludedEntities));
                spawnPkt.Result = EResult.Success;
                session.Send(spawnPkt);
            }

            // 자신의 정보 주위 다른 플레이어들에게 S_Spawn으로 전달, 자신은 전달하지 않음
            {
                S_Spawn spawnPkt = new S_Spawn();
                spawnPkt.EntityInfos.Add(character.EntityInfo);
                spawnPkt.Result = EResult.Success;
                BroadcastToAdjacentZones(spawnPkt, session.MyCharacter, new () { session.MyCharacter });
            }

        }
        public void HandleLeaveGame(ClientSession clientSession)
        {
            // TODO: 여기서 DbJob 쌓인거 있으면 정리해주어야 하는 로직 추가할 수도 있음, 추후 다시 확인
            if (clientSession?.MyCharacter == null)
            {
                return;
            }

            Character character = clientSession.MyCharacter;

            // 현재 캐릭터 정보 Zone에서 빼버림, 자신이 들고있던 Room 정보 해제
            Zone currentZone = GetZoneByCellpos(character.GetVector2IntFromPosition());
            currentZone?.Remove(character);
            
            // GameRoom에서 제거
            character.Room = null;

            clientSession.Send(new S_LeaveGame { Result = EResult.Success });

            // D_Spawn 패킷 자신이 인접한 Actor들에게 전달
            S_Despawn despawnPkt = new S_Despawn();
            despawnPkt.GameIds.Add(character.GameId);

            BroadcastToAdjacentZones(despawnPkt, character);

            // 캐릭터 참조 제거
            clientSession.MyCharacter = null;
        }
    }
}
