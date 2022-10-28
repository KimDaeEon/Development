using Google.Protobuf.Protocol;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CSharpServer.Game
{
    // 플레이어 이동 시에 보이는 범위에 따라서 주위에 보이는 것을 관리하는 클래스
    public class VisionArea
    {
        public Player Owner { get; private set; }
        public HashSet<Actor> PreviousActors { get; private set; } = new HashSet<Actor>();

        public VisionArea(Player owner)
        {
            Owner = owner;
        }

        public HashSet<Actor> GetActors()
        {
            if(Owner == null || Owner.CurrentRoom == null)
            {
                return null;
            }

            HashSet<Actor> actors = new HashSet<Actor>();

            Vector2Int cellPos = Owner.CellPos;
            List<Zone> zones = Owner.CurrentRoom.GetAdjacentZones(cellPos);

            foreach(Zone zone in zones)
            {
                foreach (Player player in zone.Players)
                {
                    int dx = player.CellPos.x - cellPos.x;
                    int dy = player.CellPos.y - cellPos.y;

                    if(Math.Abs(dx) > Room.VisionCells)
                    {
                        continue;
                    }

                    if (Math.Abs(dy) > Room.VisionCells)
                    {
                        continue;
                    }

                    actors.Add(player);
                }

                foreach (Monster monster in zone.Monsters)
                {
                    int dx = monster.CellPos.x - cellPos.x;
                    int dy = monster.CellPos.y - cellPos.y;

                    if (Math.Abs(dx) > Room.VisionCells)
                    {
                        continue;
                    }

                    if (Math.Abs(dy) > Room.VisionCells)
                    {
                        continue;
                    }

                    actors.Add(monster);
                }

                foreach (Projectile projectile in zone.Projectiles)
                {
                    int dx = projectile.CellPos.x - cellPos.x;
                    int dy = projectile.CellPos.y - cellPos.y;

                    if (Math.Abs(dx) > Room.VisionCells)
                    {
                        continue;
                    }

                    if (Math.Abs(dy) > Room.VisionCells)
                    {
                        continue;
                    }

                    actors.Add(projectile);
                }
            }

            return actors;
        }

        public void Update()
        {
            if(Owner == null || Owner.CurrentRoom == null)
            {
                return;
            }

            HashSet<Actor> currentActors = GetActors();

            // 기존에 없었는데 생긴 애들은 Spawn 처리
            List<Actor> spawns = currentActors.Except(PreviousActors).ToList();
            if(spawns.Count > 0)
            {
                S_Spawn spawnPacket = new S_Spawn();

                foreach (Actor actor in spawns)
                {
                    ActorInfo info = new ActorInfo();
                    info.MergeFrom(actor.Info);
                    spawnPacket.Actors.Add(info);
                }

                Owner.Session.Send(spawnPacket);
            }


            // 기존에 있었는데 없어진 애들은 Despawn 처리
            List<Actor> despawns = PreviousActors.Except(currentActors).ToList();
            if(despawns.Count > 0)
            {
                S_Despawn despawnPacket = new S_Despawn();

                foreach (Actor actor in despawns)
                {
                    despawnPacket.ActorIds.Add(actor.Id);
                }

                Owner.Session.Send(despawnPacket);
            }

            // 이전 액터 정보 할당
            PreviousActors = currentActors;

            Owner.CurrentRoom.PushFutureJob(250, Update);
        }
    }
}
