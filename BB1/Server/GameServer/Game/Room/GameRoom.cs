using GameServer.DB.EFCore;
using Google.Protobuf.Protocol;
using Microsoft.EntityFrameworkCore.ChangeTracking;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GameServer
{
    // 맵 관련 정보 저장 필요
    // 맵 정보를 Zone 으로 쪼개서 저장 필요
    // 플레이어 이동 시에 Zone 이동하는 기능 구현 필요
    // 사용자 입장 및 퇴직 처리 관련 로직 구현 필요
    // Room 내의 존재에게 패킷을 보낼 때에 Zone을 계산해서 보내는 기능 구현 필요
    public partial class GameRoom : JobQueue
    {
        public int GameRoomId { get; set; }
        public int DataSheetId { get; set; }

        Random _rand = new Random(); // 현재는 몬스터나 플레이어를 랜덤한 위치에 뿌리려고 쓰는데, TODO: 추후에 데이터로 다 빠지면 제거

        public MapComponent Map { get; private set; } = new MapComponent();
        public Zone[,] Zones { get; private set; } // 인근 오브젝트를 빠르게 찾기 위한 일종의 캐시이다

        public const int VisionCells = 10; // 플레이어가 자신을 중심으로 한 번에 볼 수 있는 셀 개수
        public int ZoneCells { get; private set; } // 하나의 존을 구성하는 셀 개수


        public void Init(int dataSheetId, int zoneCells)
        {
            DataSheetId = dataSheetId;
            ZoneCells = zoneCells; // 이게 10이면 X, Y 각각 10칸씩 갖는 Zone으로 맵이 쪼개진다.

            // TODO: 아래 dataSheetId로 map 이름을 찾던지 하는 로직 추가하기.
            Map.LoadMap("DefaultMapCollision");

            // 맵 사이즈가 Cell이 1개라면 올림해서 Zone이 1개는 나와야 하므로 올림 연산이 되도록 이렇게 계산
            int zoneCountX = (Map.SizeX + ZoneCells - 1) / ZoneCells; 
            int zoneCountY = (Map.SizeY + ZoneCells - 1) / ZoneCells;

            Zones = new Zone[zoneCountX, zoneCountY];

            for (int x = 0; x < zoneCountX; x++)
            {
                for (int y = 0; y < zoneCountY; y++)
                {
                    Zones[x, y] = new Zone(x, y);
                }
            }
        }

        public Zone GetZoneByCellpos(Vector2Int cellPos)
        {
            int indexX = (cellPos.x - Map.MinX) / ZoneCells;
            int indexY = (Map.MaxY - cellPos.y) / ZoneCells;

            return GetZoneByIndex(indexX, indexY);
        }

        public Zone GetZoneByIndex(int indexX, int indexY)
        {
            if (indexX < 0 || indexX >= Zones.GetLength(0))
            {
                return null;
            }
            if (indexY < 0 || indexY >= Zones.GetLength(1))
            {
                return null;
            }

            return Zones[indexX, indexY];
        }

        public List<Zone> GetAdjacentZones(Vector2Int cellPos, int cells = GameRoom.VisionCells)
        {
            HashSet<Zone> zones = new HashSet<Zone>();

            int maxY = cellPos.y + cells;
            int minY = cellPos.y - cells;
            int maxX = cellPos.x + cells;
            int minX = cellPos.x - cells;

            // 자기 자신 + VisionCell을 중심으로 왼쪽 맨 위 Zone을 찾음
            Vector2Int leftTop = new Vector2Int(minX, maxY);
            int minIndexX = (leftTop.x - Map.MinX) / ZoneCells;
            int minIndexY = (Map.MaxY - leftTop.y) / ZoneCells;

            // 자기 자신 + VisionCell을 중심으로 오른쪽 맨 아래 Zone을 찾음
            Vector2Int rightBottom = new Vector2Int(maxX, minY);
            int maxIndexX = (rightBottom.x - Map.MinX) / ZoneCells;
            int maxIndexY = (Map.MaxY - rightBottom.y) / ZoneCells;

            for (int x = minIndexX; x <= maxIndexX; x++)
            {
                for (int y = minIndexY; y <= maxIndexY; y++)
                {
                    Zone zone = GetZoneByIndex(x, y);
                    if (zone == null)
                        continue;

                    zones.Add(zone);
                }
            }

            return zones.ToList();
        }

        public List<Character> GetAdjacentCharacters(Vector2Int cellPos, List<Entity> excludedEntities = null, int cells = GameRoom.VisionCells)
        {
            HashSet<Character> characters = new HashSet<Character>();
            List<Zone> zones = GetAdjacentZones(cellPos, cells);

            foreach (var zone in zones)
            {
                characters.UnionWith(zone.Characters);
            }

            // 제외할 엔티티가 있으면 제거
            if (excludedEntities != null)
            {
                characters.ExceptWith(excludedEntities.OfType<Character>());
            }

            return characters.ToList();
        }

        public List<Monster> GetAdjacentMonsters(Vector2Int cellPos, List<Entity> excludedEntities = null, int cells = GameRoom.VisionCells)
        {
            HashSet<Monster> monsters = new HashSet<Monster>();
            List<Zone> zones = GetAdjacentZones(cellPos, cells);

            foreach (var zone in zones)
            {
                monsters.UnionWith(zone.Monsters);
            }

            // 제외할 엔티티가 있으면 제거
            if (excludedEntities != null)
            {
                monsters.ExceptWith(excludedEntities.OfType<Monster>());
            }

            return monsters.ToList();
        }

        public List<Entity> GetAdjacentEntities(Vector2Int cellPos, List<Entity> excludedEntities = null, int cells = GameRoom.VisionCells)
        {
            HashSet<Entity> entities = new HashSet<Entity>();
            List<Zone> zones = GetAdjacentZones(cellPos, cells);

            foreach (var zone in zones)
            {
                entities.UnionWith(zone.Characters.Cast<Entity>());
                entities.UnionWith(zone.Monsters.Cast<Entity>());
            }

            // 제외할 엔티티가 있으면 제거
            if (excludedEntities != null)
            {
                entities.ExceptWith(excludedEntities);
            }

            return entities.ToList();
        }


        public List<EntityInfo> GetAdjacentEntityInfos(Vector2Int cellPos, List<Entity> excludedEntities = null, int cells = GameRoom.VisionCells)
        {
            HashSet<EntityInfo> entityInfos = new HashSet<EntityInfo>();
            List<Zone> zones = GetAdjacentZones(cellPos, cells);

            foreach (var zone in zones)
            {
                // Characters에서 EntityInfo 추출
                entityInfos.UnionWith(zone.Characters.Select(character => character.EntityInfo));
                // Monsters에서 EntityInfo 추출
                entityInfos.UnionWith(zone.Monsters.Select(monster => monster.EntityInfo));
            }

            // 제외할 엔티티가 있으면 해당 EntityInfo 제거
            if (excludedEntities != null)
            {
                entityInfos.ExceptWith(excludedEntities.Select(e => e.EntityInfo));
            }

            return entityInfos.ToList();
        }

        public Vector2Int GetRandomSpawnPos(Entity entity, bool checkEntityCollision = true)
        {
            Vector2Int randomPos;

            while (true)
            {
                randomPos.x = _rand.Next(0, 100);
                randomPos.y = _rand.Next(0, 100);

                if (Map.CanGo(entity, randomPos, checkEntityCollision))
                {
                    return randomPos;
                }
            }
        }


        public void Update()
        {
            Flush();
        }


        public void BroadCastToAllZones<T>(T packet, HashSet<Character> excludeCharacters = null) where T : Google.Protobuf.IMessage
        {
            foreach (var zone in Zones)
            {
                zone.BroadCast(packet, excludeCharacters);
            }
        }

        public void BroadcastToAdjacentZones<T>(T packet, Entity self, HashSet<Character> excludeCharacters = null) where T : Google.Protobuf.IMessage
        {
            var zones = GetAdjacentZones(self.GetVector2IntFromPosition());
            foreach (var zone in zones)
            {
                zone.BroadCast(packet, excludeCharacters);
            }
        }

    }
}
