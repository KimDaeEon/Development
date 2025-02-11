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

        // 특정 위치에 오브젝트가 있는지 체크하기 위함
        Entity[,] _entityCollision;


        public bool CanGo(Entity self, Vector2Int cellPos, bool checkEntityCollision = true)
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
                    if (CanGoInternal(self, checkedCellPos, checkEntityCollision) == false)
                    {
                        return false;
                    }
                }
            }

            return true;
        }

        bool CanGoInternal(Entity self, Vector2Int cellPos, bool checkEntityCollision = true)
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

            if (checkEntityCollision)
            {
                // 자신이 아닌 다른 엔티티 있으면 못 간다
                if (_entityCollision[x, y] != null && _entityCollision[x, y] != self)
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
            _entityCollision = new Entity[xCount, yCount];

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
