using GameServer.Data;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GameServer
{
    // TODO: 맵 로딩, 길찾기 관련 로직 추가 필요

    public class MapToolConstants
    {
        public const char MAP_TOOL_WALL = '0';
        public const char MAP_TOOL_NONE = '1';
    }

    public class MapComponent
    {
        public int MinX { get; set; }
        public int MaxX { get; set; }
        public int MinY { get; set; }
        public int MaxY { get; set; }

        public int SizeX { get { return MaxX - MinX + 1; } }
        public int SizeY { get { return MaxY - MinY + 1; } }

        // 맵 자체에 충돌처리 되는 벽, 장애물 등이 있는지 체크하기 위함
        bool[,] _mapCollision;

        // 특정 위치에 액터가 있는지 체크하기 위함
        Actor[,] _actorCollision;


        public bool CanGo(Entity self, Vector2Int cellPos, bool checkActorCollision = true)
        {
            int extraCells = 0;
            if (self != null)
            {
                extraCells = self.ExtraCells;
            }
            
            // 추가된 Cell 크기가 있는 Entity 라면 추가 된 Cell 중 하나라도 걸리면 갈 수 없다.
            for (int dx = -extraCells; dx <= extraCells; dx++)
            {
                for (int dy = -extraCells; dy <= extraCells; dy++)
                {
                    Vector2Int checkedCellPos = new Vector2Int(cellPos.x + dx, cellPos.y + dy);
                    if (CanGoInternal(self, checkedCellPos, checkActorCollision) == false)
                    {
                        return false;
                    }
                }
            }

            return true;
        }

        bool CanGoInternal(Entity self, Vector2Int cellPos, bool checkActorCollision = true)
        {
            // 맵 범위 밖으로 가려면 통과 불가
            if (cellPos.x < MinX || cellPos.x > MaxX)
            {
                return false;
            }

            if (cellPos.y < MinY || cellPos.y > MaxY)
            {
                return false;
            }

            int x = cellPos.x - MinX;
            int y = MaxY - cellPos.y;

            // 충돌 영역이면 못 간다
            if (_mapCollision[x, y])
            {
                return false;
            }

            if (checkActorCollision)
            {
                // 자신이 아닌 다른 엔티티 있으면 못 간다
                if (_actorCollision[x, y] != null && _actorCollision[x, y] != self)
                    return false;
            }

            return true;
        }


        internal bool ApplyMove(Character myCharacter, Vector2Int toPos, bool checkActorCollision = true)
        {
            if (myCharacter == null)
            {
                return false;
            }

            // 이동 가능 여부 확인
            if (CanGo(myCharacter, toPos, checkActorCollision) == false)
            {
                return false;
            }

            Vector2Int fromPos = myCharacter.GetVector2IntFromPosition();

            int fromX = fromPos.x - MinX;
            int fromY = MaxY - fromPos.y;

            int toX = toPos.x - MinX;
            int toY = MaxY - toPos.y;

            if (_actorCollision[fromX, fromY] == null && _actorCollision[fromX, fromY] == myCharacter)
            {
                // 기존 위치의 충돌 정보 제거
                _actorCollision[toX, toY] = myCharacter;

                // 새로운 위치에 캐릭터 배치
                _actorCollision[fromX, fromY] = null;
            }
            else
            {
                Logger.LogError("Somethings wrong with _actorCollision");
                return false;
            }

            // Zone 이동 처리
            var curRoom = myCharacter.Room;

            var fromZone = curRoom.GetZoneByCellpos(fromPos);
            var toZone = curRoom.GetZoneByCellpos(toPos);

            // 이동하려는 Zone이 기존 Zone과 다르다면 Zone 이동 실시
            if (fromZone != toZone)
            {
                fromZone.Remove(myCharacter);
                toZone.Add(myCharacter);
            }

            // 캐릭터 위치 정보 변경
            myCharacter.SetVector2IntToPosition(toPos);

            return true;
        }

        internal bool ApplyLeave(Actor actor)
        {
            if (actor == null)
            {
                return false;
            }

            var room = actor.Room;

            if (room == null)
            {
                return false;
            }

            Vector2Int actorPos = actor.GetVector2IntFromPosition();

            // Zone 에서 제거
            Zone zone = room.GetZoneByCellpos(actorPos);
            if (zone != null)
            {
                zone.Remove(actor); // Character라면 RemoveCharacter 호출
            }

            // ActorCollision 에서 제거
            int x = actorPos.x - MinX;
            int y = MaxY - actorPos.y;

            if (_actorCollision[x, y] == actor)
            {
                _actorCollision[x, y] = null;
            
            }
            else
            {
                // TODO: 일단 Actor 는 무조건 충돌 처리를 하도록 하는데, 설계가 바뀌면 여기 바꿔야 한다.
                Logger.LogError("Somethings wrong with actorCollision");
                return false;
            }

            return true;
        }



        public void LoadMap(string mapName)
        {
            // Collision 관련 파일
            string text = File.ReadAllText($"{ConfigManager.Config.dataPath}/MapData/{mapName}.txt");
            StringReader reader = new StringReader(text);

            MinX = int.Parse(reader.ReadLine());
            MaxX = int.Parse(reader.ReadLine());
            MinY = int.Parse(reader.ReadLine());
            MaxY = int.Parse(reader.ReadLine());

            int xCount = MaxX - MinX + 1;
            int yCount = MaxY - MinY + 1;
            _mapCollision = new bool[xCount, yCount];
            _actorCollision = new Actor[xCount, yCount];

            for (int y = 0; y < yCount; y++)
            {
                string line = reader.ReadLine();
                for (int x = 0; x < xCount; x++)
                {
                    switch (line[x])
                    {
                        case MapToolConstants.MAP_TOOL_WALL:
                            _mapCollision[x, y] = true;
                            break;
                        default:
                            _mapCollision[x, y] = false;
                            break;
                    }
                }
            }
        }





        #region A* PathFinding
        public struct AStarPathFindingNode : IComparable<AStarPathFindingNode>
        {
            public int F;
            public int G;
            public int X;
            public int Y;

            public int CompareTo(AStarPathFindingNode other)
            {
                if (F == other.F)
                    return 0;
                return F < other.F ? 1 : -1;
            }
        }

        List<Vector2Int> _pathFindingDelta = new List<Vector2Int>()
        {
            new Vector2Int(0, 1), // U
			new Vector2Int(1, 1), // UR
			new Vector2Int(1, 0), // R
			new Vector2Int(1, -1), // DR
			new Vector2Int(0, -1), // D
			new Vector2Int(-1, -1), // LD
			new Vector2Int(-1, 0), // L
			new Vector2Int(-1, 1), // LU
		};

        #endregion
    }

}
