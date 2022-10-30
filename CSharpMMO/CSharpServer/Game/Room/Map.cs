using Google.Protobuf.Protocol;
using ServerCore;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CSharpServer.Game
{
    // Unity 상의 CellPos 를 배열 형식으로 다시 표현한 좌표
    // CellPos 가 절대 좌표를 의미한다면 ArrayPos 는 맵의 왼쪽 맨 위부터의 상대좌표라고 할 수 있다.
    public struct ArrayPos
    {
        public ArrayPos(int y, int x) { Y = y; X = x; }
        public int Y;
        public int X;

        public static bool operator ==(ArrayPos lhs, ArrayPos rhs)
        {
            return lhs.Y == rhs.Y && lhs.X == rhs.X;
        }

        public static bool operator !=(ArrayPos lhs, ArrayPos rhs)
        {
            return !(lhs == rhs);
        }

        public override bool Equals(object obj)
        {
            return (ArrayPos)obj == this;
        }

        public override int GetHashCode()
        {
            long value = (Y << 32) | X;
            return value.GetHashCode();
        }

        public override string ToString()
        {
            return $"Y:{Y}, X:{X}";
        }
    }

    public struct Vector2Int
    {
        public int x;
        public int y;

        public Vector2Int(int x, int y)
        {
            this.x = x;
            this.y = y;
        }

        public static Vector2Int up { get { return new Vector2Int(0, 1); } }
        public static Vector2Int down { get { return new Vector2Int(0, -1); } }
        public static Vector2Int left { get { return new Vector2Int(-1, 0); } }
        public static Vector2Int right { get { return new Vector2Int(1, 0); } }

        public static Vector2Int operator +(Vector2Int lhs, Vector2Int rhs)
        {
            return new Vector2Int(lhs.x + rhs.x, lhs.y + rhs.y);
        }

        public static Vector2Int operator -(Vector2Int lhs, Vector2Int rhs)
        {
            return new Vector2Int(lhs.x - rhs.x, lhs.y - rhs.y);
        }

        public float Magnitude { get { return (float)Math.Sqrt(SqrMagnitude); } }
        public float SqrMagnitude { get { return x * x + y * y; } }
        public int CellDistFromZero { get { return Math.Abs(x) + Math.Abs(y); } } // 특정 셀에서 다른 셀로 움직일 때에 움직여야 할 횟수를 의미
    }

    // Priority Queue 에서 사용되는 노드
    public struct PQNode : IComparable<PQNode>
    {
        public int F;
        public int G;
        public int Y;
        public int X;

        public int CompareTo(PQNode other)
        {
            if (F == other.F)
            {
                return 0;
            }

            return F < other.F ? 1 : -1;
        }
    }

    public class Map
    {
        public int MinX { get; set; }
        public int MaxX { get; set; }
        public int MinY { get; set; }
        public int MaxY { get; set; }

        public int SizeX { get { return MaxX - MinX + 1; } }
        public int SizeY { get { return MaxY - MinY + 1; } }

        bool[,] _collision;
        Actor[,] _actors; // TODO: 몬스터도 충돌하려면 상위 객체를 만들어서 관리할 필요가 있다.

        // 해당 함수 내부에서 Zone 이동을 실시한다.
        public bool ApplyMove(Actor actor, Vector2Int destCellPos,
            bool checkActorCollision = true /* 이동할 때에 actor 와 충돌 체크 여부 */,
            bool isCollisionType = true /* 다른 actor 와 충돌하는지 여부 (해당 영역 점유 여부) */)
        {
            if (CanGo(destCellPos, checkActorCollision) == false)
            {
                return false;
            }

            PositionInfo posInfo = actor.Info.PosInfo;

            // Map 상에서 내가 움직일 때마다 특정 지점을 점유하여 충돌을 일으킨다면
            if (isCollisionType)
            {
                // 현재 위치에서 내가 점유하고 있다면 반납
                int curArrayPosX = posInfo.PosX - MinX;
                int curArrayPosY = MaxY - posInfo.PosY;
                if (_actors[curArrayPosY, curArrayPosX] == actor)
                {
                    _actors[curArrayPosY, curArrayPosX] = null;
                }

                // 목표 위치 점유
                int destArrayPosX = destCellPos.x - MinX;
                int destArrayPosY = MaxY - destCellPos.y;
                _actors[destArrayPosY, destArrayPosX] = actor;
            }

            // Zone 처리
            ActorType type = ActorManager.GetActorTypeById(actor.Id);
            Zone now = actor.CurrentRoom.GetZone(actor.CellPos);
            Zone after = actor.CurrentRoom.GetZone(destCellPos);
            if (type == ActorType.Player)
            {
                Player player = (Player)actor;

                if (now != after)
                {
                    now.Players.Remove(player);
                    after.Players.Add(player);
                }
            }
            else if (type == ActorType.Monster)
            {
                Monster monster = (Monster)actor;

                if (now != after)
                {
                    now.Monsters.Remove(monster);
                    after.Monsters.Add(monster);
                }
            }
            else if (type == ActorType.Projectile)
            {
                Projectile projectile = (Projectile)actor;

                if (now != after)
                {
                    now.Projectiles.Remove(projectile);
                    after.Projectiles.Add(projectile);
                }
            }

            // 충돌 관련 정보 수정 및 Zone 이동 처리 후에 좌표 이동
            posInfo.PosX = destCellPos.x;
            posInfo.PosY = destCellPos.y;
            return true;
        }

        // 아래 함수는 
        public bool ApplyLeave(Actor actor)
        {
            if (actor.CurrentRoom == null)
            {
                return false;
            }

            if (actor.CurrentRoom.Map != this)
            {
                return false;
            }

            PositionInfo posInfo = actor.Info.PosInfo;
            if (posInfo.PosX < MinX | posInfo.PosX > MaxX)
            {
                return false;
            }

            if (posInfo.PosY < MinY | posInfo.PosY > MaxY)
            {
                return false;
            }

            // Zone 처리
            Zone zone = actor.CurrentRoom.GetZone(actor.CellPos);
            zone.Remove(actor);

            // 현재 위치에서 내가 점유하고 있다면 반납
            int curArrayPosX = posInfo.PosX - MinX;
            int curArrayPosY = MaxY - posInfo.PosY;
            if (_actors[curArrayPosY, curArrayPosX] == actor)
            {
                _actors[curArrayPosY, curArrayPosX] = null;
            }

            return true;
        }

        public Actor FindActorByCellPos(Vector2Int cellPos)
        {
            if (cellPos.x < MinX || cellPos.x > MaxX)
            {
                return null;
            }

            if (cellPos.y < MinY || cellPos.y > MaxY)
            {
                return null;
            }

            int arrayPosX = cellPos.x - MinX;
            int arrayPosY = MaxY - cellPos.y;
            return _actors[arrayPosY, arrayPosX];
        }

        public bool CanGo(Vector2Int cellPos, bool checkActorCollision = true)
        {
            if (cellPos.x < MinX || cellPos.x > MaxX)
            {
                return false;
            }

            if (cellPos.y < MinY || cellPos.y > MaxY)
            {
                return false;
            }

            int arrayPosX = cellPos.x - MinX;
            int arrayPosY = MaxY - cellPos.y; // 위의 _collision 배열 좌표를 구하려면, 맨 위인 경우 _collision[0][x]이므로 이렇게 해야한다.

            return !_collision[arrayPosY, arrayPosX] && (!checkActorCollision || _actors[arrayPosY, arrayPosX] == null);
        }

        public void LoadMap(int mapId, string pathPrefix = "../../../../Common/MapData")
        {

            string mapName = "Map_" + mapId.ToString("000");

            string mapText = File.ReadAllText($"{pathPrefix}/{mapName}.txt");
            StringReader reader = new StringReader(mapText);

            MinX = int.Parse(reader.ReadLine());
            MaxX = int.Parse(reader.ReadLine());
            MinY = int.Parse(reader.ReadLine());
            MaxY = int.Parse(reader.ReadLine());

            int xCount = MaxX - MinX + 1;
            int yCount = MaxY - MinY + 1;
            _collision = new bool[yCount, xCount]; // y 를 먼저 해야지 for 문 돌 때에 더 직관적이다.
            _actors = new Actor[yCount, xCount];

            for (int y = 0; y < yCount; y++)
            {
                string line = reader.ReadLine();
                for (int x = 0; x < xCount; x++)
                {
                    _collision[y, x] = (line[x] == '1' ? true : false);
                }
            }
        }

        #region A* PathFinding

        int[] _deltaY = new int[] { 1, -1, 0, 0 };
        int[] _deltaX = new int[] { 0, 0, -1, 1 };
        //int[] _cost = new int[] { 10, 10, 10, 10 };

        // 아래 함수는 벽에 막혀 있어도 일단 최대한 가까워질 수 있는 경로는 나오므로 null 은 리턴하지 않는다.
        public List<Vector2Int> FindPath(Vector2Int startCellPos, Vector2Int destCellPos, bool checkActorCollision = false, int searchDistLimit = 10)
        {
            List<ArrayPos> path = new List<ArrayPos>();

            // F = G + H; // F가 Fuzzy 를 의미하는 것 같다. 근사치란 의미로.. https://ko.wikipedia.org/wiki/%ED%8D%BC%EC%A7%80_%EB%85%BC%EB%A6%AC
            // G : 시작점부터 현재 위치까지의 경로를 이동하는데 소요되는 비용 // Greedy?
            // H(heuristics) : 현재 위치에서 다음 목적지 까지의 '예상' 이동 비용, 휴리스틱이란 말처럼 개략적인 계산이다. 작을수록 좋음

            // 방문한 곳 정보
            HashSet<ArrayPos> closeList = new HashSet<ArrayPos>();

            // 갈 수 있는 곳 정보, A* 의 Open List
            Dictionary<ArrayPos, int> openList = new Dictionary<ArrayPos, int>();

            // 부모 노드 좌표 정보를 저장, 경로 발견 후에 역으로 부모를 따라가야하기 때문
            Dictionary<ArrayPos, ArrayPos> parent = new Dictionary<ArrayPos, ArrayPos>();

            // 가장 좋은 후보(F 작은 후보)를 바로 뽑아오기 위함
            PriorityQueue<PQNode> pq = new PriorityQueue<PQNode>();

            ArrayPos pos = CellPos2ArrayPos(startCellPos);
            ArrayPos dest = CellPos2ArrayPos(destCellPos);

            // 시작점을 Open 리스트에 추가
            openList.Add(pos, CalcHeuristics(dest.Y, dest.X, pos.Y, pos.X));

            pq.Push(new PQNode() { F = CalcHeuristics(dest.Y, dest.X, pos.Y, pos.X), G = 0, Y = pos.Y, X = pos.X });
            parent.Add(pos, pos);

            while (pq.Count > 0)
            {
                // 현재 갈 수 있는 곳 중 제일 좋은 노드 할당
                PQNode node = pq.Pop();
                ArrayPos nodePos = new ArrayPos(node.Y, node.X);

                // 길찾기 과정에서 해당 노드를 이미 방문했으면 스킵
                if (closeList.Contains(nodePos))
                {
                    continue;
                }

                // 방문
                closeList.Add(nodePos);

                // 목적지에 도착했다면 종료
                if (node.Y == dest.Y && node.X == dest.X)
                {
                    break;
                }

                // 상,하,좌,우 이동 여부 확인해서 Open List 에 추가
                for (int i = 0; i < _deltaY.Length; i++)
                {
                    ArrayPos next = new ArrayPos(node.Y + _deltaY[i], node.X + _deltaX[i]);
                    
                    // 너무 먼 길을 찾으려 한다면 스킵
                    if(Math.Abs(pos.Y - next.Y) + Math.Abs(pos.X - next.X) > searchDistLimit)
                    {
                        continue;
                    }

                    // ignoreDestCollision 이 false 면, 목표 위치에 대해 충돌체크 하지 않는다.
                    if (next.Y != dest.Y || next.X != dest.X)
                    {
                        // 유효 범위 벗어났거나, 벽으로 막혀 있으면 스킵
                        // TODO: 아래에서 장애물 뿐만이 아니라 다른 사용자나 몬스터 충돌 체크도 추가할 필요 있음.
                        if (CanGo(ArrayPos2CellPos(next), checkActorCollision) == false /*|| Managers.Object.FindByCellPos(Pos2Cell(next)) != null*/)
                        {
                            continue;
                        }
                    }

                    // 이미 방문했으면 스킵
                    if (closeList.Contains(next))
                    {
                        continue;
                    }

                    int ng = node.F; // 이 부분 수치는 이정도면 될 듯 하다.
                    int nh = CalcHeuristics(dest.Y, dest.X, next.Y, next.X);

                    int value;
                    if (openList.TryGetValue(next, out value) == false)
                    {
                        value = Int32.MaxValue;
                    }

                    // 이미 이전 경로에서 찾은 값이 더 낫다면 스킵
                    if (value < ng + nh)
                    {
                        continue;
                    }

                    if (openList.TryAdd(next, ng + nh) == false)
                    {
                        openList[next] = ng + nh;
                    }

                    pq.Push(new PQNode() { F = ng + nh, G = ng, Y = next.Y, X = next.X });

                    // 다음 장소 전에 거쳐야할 장소가 현재 장소가 된다.
                    if (parent.TryAdd(next, nodePos) == false)
                    {
                        parent[next] = nodePos;
                    }
                }
            }

            return CalcCellPathFromParent(parent, dest);
        }

        public int CalcHeuristics(int destY, int destX, int curY, int curX)
        {
            return 10 * ((destY - curY) * (destY - curY) + (destX - curX) * (destX - curX));
        }
        public int CalcHeuristics(Vector2Int destCellPos, Vector2Int curCellPos)
        {
            return CalcHeuristics(destCellPos.y, destCellPos.x, curCellPos.y, curCellPos.x);
        }

        // 부모 노드를 역으로 추적해서 갈 수 있는 길을 찾는다.
        List<Vector2Int> CalcCellPathFromParent(Dictionary<ArrayPos, ArrayPos> parent, ArrayPos destPos)
        {
            List<Vector2Int> cells = new List<Vector2Int>();

            // 목적지에 못가더라도 갈 수 있는 가장 가까운 장소로 가도록 변경
            if (parent.ContainsKey(destPos) == false)
            {
                ArrayPos secondBestPos = new ArrayPos();
                int secondBestScore = Int32.MaxValue;

                foreach(ArrayPos pos in parent.Keys)
                {
                    int score = CalcHeuristics(destPos.Y, destPos.X, pos.Y, pos.X);

                    if(score < secondBestScore)
                    {
                        secondBestScore = score;
                        secondBestPos = pos;
                    }
                }

                destPos = secondBestPos;
            }

            {
                ArrayPos pos = destPos;

                while (parent[pos] != pos)
                {
                    cells.Add(ArrayPos2CellPos(pos));
                    pos = parent[pos];
                }
                cells.Add(ArrayPos2CellPos(pos));
                cells.Reverse();
            }

            return cells;
        }

        // Vector3Int 와 Pos 간의 좌표 표기 차이로 인해 존재.
        // x, y  좌표계 y 최대 좌표 = Pos y 좌표 0, x 최소 좌표 = Pos x 좌표 0
        ArrayPos CellPos2ArrayPos(Vector2Int cell)
        {
            // CellPos -> ArrayPos
            return new ArrayPos(MaxY - cell.y, cell.x - MinX);
        }

        Vector2Int ArrayPos2CellPos(ArrayPos pos)
        {
            // ArrayPos -> CellPos
            return new Vector2Int(pos.X + MinX, MaxY - pos.Y);
        }
        #endregion
    }
}
