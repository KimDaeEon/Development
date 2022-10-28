using CSharpServer.Data;
using Google.Protobuf;
using Google.Protobuf.Protocol;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CSharpServer.Game
{
    public partial class Room : JobQueue
    {
        public void HandleMove(Player player, C_Move movePacket)
        {
            // TODO: 밖에서 확인했는데 또 actor Null 체크를 해야하나?
            // 후에 DummyClient 로 테스트해보면서 문제 있나 확인해보자.
            //if (player == null)
            //{
            //    return;
            //}

            // TODO: 이동 패킷 검증해서 움직이는 양 보정하는 로직 필요
            ActorInfo info = player.Info;
            PositionInfo packetPosInfo = movePacket.PosInfo;

            // 현재 클라이언트에서 움직인 좌표로 이동할 수 있는지 확인
            if (packetPosInfo.PosX != info.PosInfo.PosX || packetPosInfo.PosY != info.PosInfo.PosY)
            {
                if (Map.ApplyMove(player, new Vector2Int(packetPosInfo.PosX, packetPosInfo.PosY)) == false)
                {
                    return;
                }
            }

            // TODO: MovePacket 으로 State 까지 맞추는 게 좀 그렇긴한데.. 고민해보자.
            info.PosInfo.State = packetPosInfo.State;
            info.PosInfo.MoveDir = packetPosInfo.MoveDir;

            // 다른 플레이어에게 변경된 사항을 전달
            S_Move resMovePacket = new S_Move();
            resMovePacket.ActorId = player.Info.ActorId;
            resMovePacket.PosInfo = movePacket.PosInfo;

            Broadcast(player.CellPos, resMovePacket);
        }
        public void HandleSkill(Player player, C_Skill skillPacket)
        {
            ActorInfo info = player.Info;
            info = player.Info;
            if (info.PosInfo.State != ActorState.Idle)
            {
                return;
            }

            // TODO: 스킬 사용 가능여부 체크 추가 필요
            info.PosInfo.State = ActorState.Skill;

            S_Skill resSkillPacket = new S_Skill() { Info = new SkillInfo() };
            resSkillPacket.ActorId = player.Info.ActorId;
            resSkillPacket.Info.SkillId = skillPacket.Info.SkillId;
            Broadcast(player.CellPos, resSkillPacket);

            Data.SkillData skillData;
            if (DataManager.SkillDict.TryGetValue(skillPacket.Info.SkillId, out skillData) == false)
            {
                Console.WriteLine($"Cannot find skill data skillId : {skillPacket.Info.SkillId}");
                return;
            }

            switch (skillData.skillType)
            {
                case SkillType.SkillAuto:
                    Vector2Int skillPos = player.GetFrontCellPos(info.PosInfo.MoveDir);
                    Actor target = Map.FindActorByCellPos(skillPos);
                    if (target != null)
                    {
                        target.OnDamaged(player, player.TotalAttack);
                    }
                    break;

                case SkillType.SkillProjectile:
                    // TODO: 혹시나 발사체 타입 스킬이 추가된다면 여기서 분기나 다른 방식의 설계 적용이 필요.
                    Arrow arrow = ActorManager.Instance.Add<Arrow>();

                    if (arrow == null)
                    {
                        return;
                    }

                    arrow.Owner = player;
                    arrow.Data = skillData; // 후에 데미지 계산이나 이동을 위한 참조 데이터

                    arrow.PosInfo.State = ActorState.Moving;
                    arrow.PosInfo.MoveDir = player.PosInfo.MoveDir;
                    arrow.PosInfo.PosX = player.PosInfo.PosX;
                    arrow.PosInfo.PosY = player.PosInfo.PosY;
                    arrow.Speed = skillData.projectile.speed;

                    EnterGame(arrow, false);
                    break;
            }

            // TODO: 데미지 판정, 범위 스킬도 여기서 관련 로직 및 데이터 추가 필요
        }
    }
}
