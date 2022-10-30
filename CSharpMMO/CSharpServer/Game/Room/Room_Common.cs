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
        // TODO: 데이터나 config 로 빼자.
        public const int VisionCells = 5;

        public int RoomId { get; set; }
        Dictionary<int, Player> _players = new Dictionary<int, Player>();
        Dictionary<int, Monster> _monsters = new Dictionary<int, Monster>();
        Dictionary<int, Projectile> _projectiles = new Dictionary<int, Projectile>();
        public Map Map { get; private set; } = new Map();
        public Zone[,] Zones { get; private set; }
        public int ZoneCells { get; private set; } // 하나의 존이 차지하는 Cell 갯수
        Random _rand = new Random(); // EnterGame 시 위치 선정을 위한 것

        // TODO: 플레이어 가시 범위와 더불어 Zone 크기도 데이터로 빼도록 하자.
        public void Init(int mapId, int zoneCells)
        {
            Map.LoadMap(mapId);

            // Zone 
            ZoneCells = zoneCells; // 10

            // zoneCells 이 10이라고 했을 때에, 아래와 같은 식으로 계산이 된다.
            // 1~10 칸 = 1존
            // 11~20 칸 = 2존
            // 이를 통해 일정 범위의 영역을 zone 에 속하는 Cell 수로 조절이 가능하다.
            int countY = (Map.SizeY + zoneCells - 1) / zoneCells;
            int countX = (Map.SizeX + zoneCells - 1) / zoneCells;
            Zones = new Zone[countY, countX];
            for (int y = 0; y < countY; y++)
            {
                for (int x = 0; x < countX; x++)
                {
                    // Zone 을 생성하고 index 를 기입
                    Zones[y, x] = new Zone(y, x);
                }
            }

            // TODO: 임시로 몬스터 생성, 후에는 데이터를 통해 어떤 몬스터를 생성할 지 정할 수 있어야 한다.
            for (int i = 0; i < 1000; i++)
            {
                Monster monster = ActorManager.Instance.Add<Monster>();
                monster.Init(1);
                monster.CellPos = new Vector2Int(5, 5);
                EnterGame(monster, true);
            }
        }

        public void Update()
        {
            Flush();
        }

        public void EnterGame(Actor newActor, bool randomPos)
        {
            if (null == newActor)
            {
                return;
            }

            if (randomPos)
            {
                Vector2Int respawnPos;
                while (true)
                {
                    respawnPos.x = _rand.Next(Map.MinX, Map.MaxX + 1);
                    respawnPos.y = _rand.Next(Map.MinY, Map.MaxY + 1);

                    if (Map.CanGo(respawnPos, checkActorCollision:true))
                    {
                        newActor.CellPos = respawnPos;
                        break;
                    }
                }
            }

            ActorType actorType = ActorManager.GetActorTypeById(newActor.Id);

            if (actorType == ActorType.Player)
            {
                Player newPlayer = newActor as Player;

                _players.Add(newPlayer.Info.ActorId, newPlayer);
                newPlayer.CurrentRoom = this;

                newPlayer.RefreshStat();

                GetZone(newPlayer.CellPos).Players.Add(newPlayer);
                newPlayer.CurrentRoom.Map.ApplyMove(newPlayer, newPlayer.CellPos);

                // 자신에게 접속이 완료되었음을 전송
                S_EnterGame enterPacket = new S_EnterGame();
                enterPacket.Player = newPlayer.Info;
                newPlayer.Session.Send(enterPacket);

                newPlayer.Vision.Update();
            }
            else if (actorType == ActorType.Monster)
            {
                Monster newMonster = newActor as Monster;
                _monsters.Add(newMonster.Info.ActorId, newMonster);
                newMonster.CurrentRoom = this;
                GetZone(newMonster.CellPos).Monsters.Add(newMonster);
                newMonster.CurrentRoom.Map.ApplyMove(newMonster, newMonster.CellPos);

                newMonster.Update();
            }
            else if (actorType == ActorType.Projectile)
            {
                Projectile newProjectile = newActor as Projectile;
                _projectiles.Add(newProjectile.Info.ActorId, newProjectile);
                newProjectile.CurrentRoom = this;
                GetZone(newProjectile.CellPos).Projectiles.Add(newProjectile);
                newProjectile.CurrentRoom.Map.ApplyMove(newProjectile, newProjectile.CellPos);

                // 아래처럼 하지 않으면 바로 앞에서 몬스터를 맞을 경우 패킷 순서로 인해 클라이언트에서 Projectile 이 제거되지 않는다.
                Push(newProjectile.Update); 
            }


            S_Spawn spawnPacket = new S_Spawn();
            spawnPacket.Actors.Add(newActor.Info);
            Broadcast(newActor.CellPos, spawnPacket);
        }

        public void LeaveGame(int actorId)
        {
            ActorType actorType = ActorManager.GetActorTypeById(actorId);

            Vector2Int cellPos;
            if (actorType == ActorType.Player)
            {
                Player player;
                if (_players.Remove(actorId, out player) == false)
                {
                    return;
                }

                cellPos = player.CellPos;

                Map.ApplyLeave(player);
                player.OnLeaveGame(); // 게임 나갈 시의 DB 처리
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

                cellPos = monster.CellPos;
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

                cellPos = projectile.CellPos;
                Map.ApplyLeave(projectile);
                projectile.CurrentRoom = null;
            }
            else
            {
                return;
            }

            S_Despawn despanPacket = new S_Despawn();
            despanPacket.ActorIds.Add(actorId);
            Broadcast(cellPos, despanPacket);
        }

        public void Broadcast(Vector2Int pos, IMessage packet)
        {
            List<Zone> zones = GetAdjacentZones(pos);
            foreach (Zone zone in zones)
            {
                foreach (Player p in zone.Players)
                {
                    int dx = p.CellPos.x - pos.x;
                    int dy = p.CellPos.y - pos.y;

                    if (Math.Abs(dx) > Room.VisionCells)
                    {
                        continue;
                    }

                    if (Math.Abs(dy) > Room.VisionCells)
                    {
                        continue;
                    }

                    p.Session.Send(packet);
                }
            }
        }

        public Zone GetZone(Vector2Int cellPos)
        {
            // Zone 의 좌표는 0번부터 시작한다. 맨 왼쪽 위의 zone 은 Zones[0,0] 이다.
            int y = (Map.MaxY - cellPos.y) / ZoneCells;
            int x = (cellPos.x - Map.MinX) / ZoneCells;

            if (x < 0 || x >= Zones.GetLength(1))
            {
                return null;
            }

            if (y < 0 || y >= Zones.GetLength(0))
            {
                return null;
            }

            return Zones[y, x];
        }

        public List<Zone> GetAdjacentZones(Vector2Int cellPos, int cellsRange = VisionCells/* 보이는 cell 범위*/)
        {
            HashSet<Zone> zones = new HashSet<Zone>();
            int[] delta = new int[3] { -cellsRange, 0, +cellsRange };

            foreach (int dy in delta)
            {
                foreach (int dx in delta)
                {
                    if (dy == 0 && dx == 0)
                    {
                        continue;
                    }

                    int y = cellPos.y + dy;
                    int x = cellPos.x + dx;

                    Zone zone = GetZone(new Vector2Int(x, y));
                    if (zone == null)
                    {
                        continue;
                    }

                    zones.Add(zone);
                }
            }

            return zones.ToList();
        }

        // 좀 무거운 함수다.
        // 해당 위치에서 범위에 있는 사용자 중 가장 가까운 플레이어를 찾는다.
        public Player FindClosestPlayer(Vector2Int pos, int cellRange)
        {
            List<Player> players = GetAdjacentPlayers(pos, cellRange);

            // 거리 순으로 플레이어 정렬
            players.Sort((lhs, rhs) =>
            {
                int leftScore = Map.CalcHeuristics(lhs.CellPos, pos);
                int rhsScore = Map.CalcHeuristics(rhs.CellPos, pos);

                return leftScore - rhsScore;
            });

            // 일단 가는 경로가 있으면 해당 플레이어를 추적
            foreach (Player player in players)
            {
                List<Vector2Int> path = Map.FindPath(pos, player.CellPos, checkActorCollision: true, cellRange);
                if(path.Count < 2 || path.Count > cellRange)
                {
                    continue;
                }

                return player;
            }

            return null;
        }

        public List<Player> GetAdjacentPlayers(Vector2Int pos, int cellRange)
        {
            List<Zone> zones = GetAdjacentZones(pos, cellRange);
            return zones.SelectMany(z => z.Players).ToList();
        }

        Player FindPlayerByCondition(Func<Actor, bool> condition)
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
    }
}
