using Google.Protobuf.Protocol;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CSharpServer.Game
{
    public class Arrow : Projectile
    {
        public Actor Owner { get; set; }
        //public int MovedDistance { get; set; } = 0; // TODO: 투사체가 내가 보이는 시야 범위를 넘어서 영향을 줘도 될지.. 확인 필요

        public override void Update()
        {
            if(Data == null || Data.projectile == null || Owner == null || CurrentRoom == null)
            {
                return;
            }

            //if(MovedDistance > Room.VisionCells)
            //{
            //    CurrentRoom.LeaveGame(Id);
            //    return;
            //}

            // TODO: 투사체 판정 관련해서 클라이언트에서는 맞은 것 같은데 안맞는 경우가 생긴다. 
            // 너무 빨리 움직이면 자기 앞에 누가 있는지 판정하기 전에 맞아야할 대상이 이미 자신의 위치로 와버리기 때문인 것 같은데..
            // 피격 판정 방식을 바꾸는 것도 고민이 필요하다.

            // Unity 에서 스피드가 1.0f 라고 한다면, 1초(1000 밀리초)에 1.0f 만큼을 이동한다는 뜻이다.
            // Tick 이 단위가 밀리초이므로 단위를 맞춰서 아래 정도의 Tick 간격으로 패킷을 보내주면 이동이 자연스럽게 맞춰진다.
            int tickInterval = (int)(1000 / Speed);
            CurrentRoom.PushFutureJob(tickInterval, Update);

            Vector2Int destPos = GetFronCellPos();
            if (CurrentRoom.Map.ApplyMove(this, destPos, isCollisionType:false))
            {
                S_Move movePacket = new S_Move();
                movePacket.ActorId = Id;
                movePacket.PosInfo = PosInfo;
                CurrentRoom.Broadcast(CellPos, movePacket);

                //MovedDistance++;
            }
            else
            {
                Actor target = CurrentRoom.Map.FindActorByCellPos(destPos);
                if(target != null)
                {
                    target.OnDamaged(this, Data.damage + Owner.TotalAttack);
                }

                CurrentRoom.LeaveGame(Id);
            }
        }

        public override Actor GetOwner()
        {
            return Owner;
        }
    }
}
