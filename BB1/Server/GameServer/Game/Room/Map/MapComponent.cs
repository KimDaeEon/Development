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
            // TODO
            return true;
        }

        public void LoadMap()
        {
            // TODO
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
    #endregion
}
