using Google.Protobuf.Protocol;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CSharpServer.Contents
{
    public class Arrow : Projectile
    {
        public Actor Owner { get; set; }
        long _nextUpdateTick = 0;

        public override void Update()
        {
            if(Data == null || Data.projectile == null || Owner == null || CurrentRoom == null)
            {
                return;
            }

            if(_nextUpdateTick >= Environment.TickCount64)
            {
                return;
            }

            // TODO: 투사체 판정 관련해서 클라이언트에서는 맞은 것 같은데 안맞는 경우가 생긴다. 클라이언트와 보여지는 차이 문제 같은데 이거 맞추는 것도 고민 필요
            // Unity 에서 스피드가 1.0f 라고 한다면, 1초(1000 밀리초)에 1.0f 만큼을 이동한다는 뜻이다.
            // TickCount64 가 밀리초이므로 단위를 맞춰서 아래 정도의 Tick 간격으로 패킷을 보내주면 이동이 자연스럽게 맞춰진다.
            long tickInterval = (long)(1000 / Speed);
            _nextUpdateTick = Environment.TickCount64 + tickInterval;

            Vector2Int destPos = GetFronCellPos();
            if (CurrentRoom.Map.CanGo(destPos))
            {
                CellPos = destPos;

                S_Move movePacket = new S_Move();
                movePacket.ActorId = Id;
                movePacket.PosInfo = PosInfo;
                CurrentRoom.Broadcast(movePacket);
            }
            else
            {
                Actor target = CurrentRoom.Map.FindActorByCellPos(destPos);
                if(target != null)
                {
                    target.OnDamaged(this, Data.damage + Owner.Stat.Attack);
                }

                CurrentRoom.LeaveGame(Id);
            }
        }
    }
}
