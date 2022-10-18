using CSharpServer.Data;
using Google.Protobuf;
using Google.Protobuf.Protocol;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CSharpServer.Contents
{
    public class Room : JobQueue
    {
        public int RoomId { get; set; }
        Dictionary<int, Player> _players = new Dictionary<int, Player>();
        Dictionary<int, Monster> _monsters = new Dictionary<int, Monster>();
        Dictionary<int, Projectile> _projectiles = new Dictionary<int, Projectile>();
        public Map Map { get; private set; } = new Map();

        #region CommonLogic
        public void Init(int mapId)
        {
            Map.LoadMap(mapId);

            // TODO: 임시로 몬스터 생성, 후에는 데이터를 통해 이것을 좌우할 필요가 있다.
            Monster monster = ActorManager.Instance.Add<Monster>();
            monster.CellPos = new Vector2Int(5, 5);
            EnterGame(monster);
        }

        public void Update()
        {
            foreach (Monster monster in _monsters.Values)
            {
                monster.Update();
            }

            foreach (Projectile projectile in _projectiles.Values)
            {
                projectile.Update();
            }
        }

        public void EnterGame(Actor newActor)
        {
            if (null == newActor)
            {
                return;
            }

            ActorType actorType = ActorManager.GetActorTypeById(newActor.Id);

            if (actorType == ActorType.Player)
            {
                Player newPlayer = newActor as Player;

                _players.Add(newPlayer.Info.ActorId, newPlayer);
                newPlayer.CurrentRoom = this;
                newPlayer.CurrentRoom.Map.ApplyMove(newPlayer, newPlayer.CellPos);

                // 자신에게 접속이 완료되었음을 전송
                S_EnterGame enterPacket = new S_EnterGame();
                enterPacket.Player = newPlayer.Info;
                newPlayer.Session.Send(enterPacket);

                // 나에게 현재 room 안에 있는 모든 actor 정보 전달하기 위해 저장
                S_Spawn spawnPacketForMe = new S_Spawn();
                foreach (var player in _players.Values)
                {
                    if (newPlayer != player)
                    {
                        spawnPacketForMe.Actors.Add(player.Info);
                    }
                }

                foreach (var monster in _monsters.Values)
                {
                    spawnPacketForMe.Actors.Add(monster.Info);
                }

                foreach (var projectile in _projectiles.Values)
                {
                    spawnPacketForMe.Actors.Add(projectile.Info);
                }

                // 앞에서 저장했던 다른 액터 정보 나에게 전달
                newPlayer.Session.Send(spawnPacketForMe);
            }
            else if (actorType == ActorType.Monster)
            {
                Monster newMonster = newActor as Monster;
                _monsters.Add(newMonster.Info.ActorId, newMonster);
                newMonster.CurrentRoom = this;
                newMonster.CurrentRoom.Map.ApplyMove(newMonster, newMonster.CellPos);
            }
            else if (actorType == ActorType.Projectile)
            {
                Projectile newProjectile = newActor as Projectile;
                _projectiles.Add(newProjectile.Info.ActorId, newProjectile);
                newProjectile.CurrentRoom = this;
                newProjectile.CurrentRoom.Map.ApplyMove(newProjectile, newProjectile.CellPos);
            }

            // 상대방에게 내가 접속했음을 전달
            S_Spawn spawnPacketForOthers = new S_Spawn();
            spawnPacketForOthers.Actors.Add(newActor.Info);
            foreach (var player in _players.Values)
            {
                if (newActor.Id != player.Id)
                {
                    player.Session.Send(spawnPacketForOthers);
                }
            }
        }

        public void LeaveGame(int actorId)
        {
            ActorType actorType = ActorManager.GetActorTypeById(actorId);

            if (actorType == ActorType.Player)
            {
                Player player;
                if (_players.Remove(actorId, out player) == false)
                {
                    return;
                }

                Map.ApplyLeave(player);
                player.CurrentRoom = null;

                // 나에게 전송
                S_LeaveGame leavePacket = new S_LeaveGame();
                player.Session.Send(leavePacket);
            }
            else if (actorType == ActorType.Monster)
            {
                Monster monster;
                if (_monsters.Remove(actorId, out monster) == false)
                {
                    return;
                }

                Map.ApplyLeave(monster);
                monster.CurrentRoom = null;
            }
            else if (actorType == ActorType.Projectile)
            {
                Projectile projectile;
                if (_projectiles.Remove(actorId, out projectile) == false)
                {
                    return;
                }

                projectile.CurrentRoom = null;
            }

            // 타인에게 전송
            S_Despawn despawnPacket = new S_Despawn();
            despawnPacket.ActorIds.Add(actorId);
            foreach (var player in _players.Values)
            {
                if (actorId != player.Id)
                {
                    player.Session.Send(despawnPacket);
                }
            }
        }

        public void Broadcast(IMessage packet)
        {
            foreach (var p in _players.Values)
            {
                p.Session.Send(packet);
            }
        }

        public Player FindPlayerByCondition(Func<Actor, bool> condition)
        {
            foreach (var player in _players.Values)
            {
                if (condition.Invoke(player))
                {
                    return player;
                }

            }
            return null;
        }
        #endregion

        #region ContentsLogic
        public void HandleMove(Actor actor, C_Move movePacket)
        {
            // TODO: 밖에서 확인했는데 또 actor Null 체크를 해야하나?
            // 후에 DummyClient 로 테스트해보면서 문제 있나 확인해보자.
            //if (actor == null)
            //{
            //    return;
            //}

            // TODO: 이동 패킷 검증해서 움직이는 양 보정하는 로직 필요
            ActorInfo info = actor.Info;
            PositionInfo packetPosInfo = movePacket.PosInfo;

            // 현재 클라이언트에서 움직인 좌표로 이동할 수 있는지 확인
            if (packetPosInfo.PosX != info.PosInfo.PosX || packetPosInfo.PosY != info.PosInfo.PosY)
            {
                if (Map.ApplyMove(actor, new Vector2Int(packetPosInfo.PosX, packetPosInfo.PosY)) == false)
                {
                    return;
                }
            }

            // TODO: MovePacket 으로 State 까지 맞추는 게 좀 그렇긴한데.. 고민해보자.
            info.PosInfo.State = packetPosInfo.State;
            info.PosInfo.MoveDir = packetPosInfo.MoveDir;

            // 다른 플레이어에게 변경된 사항을 전달
            S_Move resMovePacket = new S_Move();
            resMovePacket.ActorId = actor.Info.ActorId;
            resMovePacket.PosInfo = movePacket.PosInfo;

            Broadcast(resMovePacket);
        }

        public void HandleSkill(Actor actor, C_Skill skillPacket)
        {
            ActorInfo info = actor.Info;
            info = actor.Info;
            if (info.PosInfo.State != ActorState.Idle)
            {
                return;
            }

            // TODO: 스킬 사용 가능여부 체크 추가 필요
            info.PosInfo.State = ActorState.Skill;

            S_Skill resSkillPacket = new S_Skill() { Info = new SkillInfo() };
            resSkillPacket.ActorId = actor.Info.ActorId;
            resSkillPacket.Info.SkillId = skillPacket.Info.SkillId;
            Broadcast(resSkillPacket);

            Data.Skill skillData;
            if (DataManager.SkillDict.TryGetValue(skillPacket.Info.SkillId, out skillData) == false)
            {
                Console.WriteLine($"Cannot find skill data skillId : {skillPacket.Info.SkillId}");
                return;
            }

            switch (skillData.skillType)
            {
                case SkillType.SkillAuto:
                    Vector2Int skillPos = actor.GetFrontCellPos(info.PosInfo.MoveDir);
                    Actor target = Map.FindActorByCellPos(skillPos);
                    if (target != null)
                    {
                        // TODO: 추후 수정 필요
                        target.OnDamaged(actor, 10);
                    }
                    break;

                case SkillType.SkillProjectile:
                    // TODO: 혹시나 발사체 타입 스킬이 추가된다면 여기서 분기나 다른 방식의 설계 적용이 필요.
                    Arrow arrow = ActorManager.Instance.Add<Arrow>();

                    // TODO: 여기서 null 체크 필요성이 있을지 고민..
                    if (arrow == null)
                    {
                        return;
                    }

                    arrow.Owner = actor;
                    arrow.Data = skillData; // 후에 데미지 계산이나 이동을 위한 참조 데이터

                    arrow.PosInfo.State = ActorState.Moving;
                    arrow.PosInfo.MoveDir = actor.PosInfo.MoveDir;
                    arrow.PosInfo.PosX = actor.PosInfo.PosX;
                    arrow.PosInfo.PosY = actor.PosInfo.PosY;
                    arrow.Speed = skillData.projectile.speed;

                    EnterGame(arrow);
                    break;
            }

            // TODO: 데미지 판정, 범위 스킬도 여기서 관련 로직 및 데이터 추가 필요
        }
        #endregion
    }
}
