using Google.Protobuf.Protocol;
using Protocol = Google.Protobuf.Protocol;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using UnityEngine;
using UnityEngine.EventSystems;
using UnityEngine.Tilemaps;
using static CustomEnum;

// TODO: 아래 부분 서버랑 맞춰서 다시 확인 필요
public class MapManager
{
    // TODO: 프로토 버퍼 상에는 char를 똑같이 Enum 으로 값을 박아서 쓸 방법이 없다. 추후에 서버랑 공유 가능한 방법 찾으면 이거 바꾸자.
    public class MapToolConstants
    {
        public const char MAP_TOOL_WALL = '0';
        public const char MAP_TOOL_NONE = '1';
    }

    public GameObject Map { get; private set; }
    public string MapName { get; private set; }
    public Grid CellGrid { get; private set; } // 타일맵 시스템의 기본 구조이다. 각 타일을 좌표 기반으로 정렬할 수 있도록 그리드 형태로 관리
    public Dictionary<Vector3Int, Entity> _cells = new Dictionary<Vector3Int, Entity>();

    public int MinX { get; private set; }
    public int MaxX { get; private set; }
    public int MinY { get; private set; }
    public int MaxY { get; private set; }

    // 맵 자체에 충돌처리 되는 벽, 장애물 등이 있는지 체크하기 위함
    bool[,] _mapCollision;

    // 특정 위치에 액터가 있는지 체크하기 위함
    Actor[,] _actorCollision;


    public Entity GetObject(Vector3Int cellPos)
    {
        _cells.TryGetValue(cellPos, out Entity value);
        return value;
    }
    public Vector3 CellPosToWorldPos(Vector3Int cellPos)
    {
        return CellGrid.CellToWorld(cellPos);
    }


    public void DestroyMap()
    {
        _cells.Clear();

        if (Map != null)
        {
            Managers.Resource.Destroy(Map);
            Map = null;
        }
    }

    void ParseCollisionData(GameObject map, string mapName, string tilemap = "Tilemap_Collision")
    {
        GameObject collision = Utils.FindChild(map, tilemap, true);
        if (collision != null)
            collision.SetActive(false);

        // Collision 관련 파일
        TextAsset txt = Managers.Resource.Get<TextAsset>($"{mapName}Collision");
        StringReader reader = new StringReader(txt.text);

        MinX = int.Parse(reader.ReadLine());
        MaxX = int.Parse(reader.ReadLine());
        MinY = int.Parse(reader.ReadLine());
        MaxY = int.Parse(reader.ReadLine());

        int xCount = MaxX - MinX + 1;
        int yCount = MaxY - MinY + 1;
        _mapCollision = new bool[xCount, yCount];

        for (int y = 0; y < yCount; y++)
        {
            string line = reader.ReadLine();
            for (int x = 0; x < xCount; x++)
            {
                switch (line[x])
                {
                    case MAP_TOOL_WALL:
                        _mapCollision[x, y] = true;
                        break;
                    case MAP_TOOL_NONE:
                        _mapCollision[x, y] = false;
                        break;
                    default:
                        _mapCollision[x, y] = false;
                        break;
                }
            }
        }
    }
    public void LoadMap(string mapName)
    {
        DestroyMap();

        GameObject map = Managers.Resource.Instantiate(mapName);
        map.transform.position = Vector3.zero;
        map.name = $"@Map_{mapName}";

        Map = map;
        MapName = mapName;
        CellGrid = map.GetOrAddComponent<Grid>();

        ParseCollisionData(map, mapName);
    }


    public bool ApplyMove(Entity self, Vector3Int toPos, bool checkActorCollision = true)
    {
        if (self == null)
        {
            return false;
        }

        Vector3Int fromPos = self.GetVector3IntFromPosition();

        if (fromPos == toPos)
        {
            return false;
        }

        // 이동 가능 여부 확인
        if (CanGo(self, toPos, checkActorCollision) == false)
        {
            return false;
        }

        int fromX = fromPos.x - MinX;
        int fromY = MaxY - fromPos.y;

        int toX = toPos.x - MinX;
        int toY = MaxY - toPos.y;

        if (self is Actor && _actorCollision[toX, toY] == null)
        {
            // 새로운 위치에 액터 배치
            _actorCollision[toX, toY] = (Actor)self;

            // 기존 위치의 충돌 정보 제거
            if (_actorCollision[fromX, fromY] == self)
            {
                _actorCollision[fromX, fromY] = null;
            }
        }

        // 위치 정보 변경
        self.SetVector3IntToPosition(toPos);

        return true;
    }

    public bool ApplyLeave(Entity self)
    {
        if (self == null)
        {
            return false;
        }

        Vector3Int actorPos = self.GetVector3IntFromPosition();

        // ActorCollision 에서 제거
        int x = actorPos.x - MinX;
        int y = MaxY - actorPos.y;

        if (self is Actor && _actorCollision[x, y] == self)
        {
            _actorCollision[x, y] = null;

        }

        return true;
    }

    #region A* Path Finding
    public struct PathFindingNode : IComparable<PathFindingNode>
    {
        public int H;
        public Vector3Int CellPos;
        public int Level;

        public int CompareTo(PathFindingNode other)
        {
            if (H == other.H)
            {
                return 0;
            }

            if (H < other.H)
            {
                return 1;
            }
            else
            {
                return -1;
            }
        }
    }

    List<Vector3Int> _delta = new List<Vector3Int>()
    {
        new Vector3Int(0, 1, 0), // U
        new Vector3Int(1, 1, 0), // UR
        new Vector3Int(1, 0, 0), // R
        new Vector3Int(1, -1, 0), // DR
        new Vector3Int(0, -1, 0), // D
        new Vector3Int(-1, -1, 0), // LD
        new Vector3Int(-1, 0, 0), // L
        new Vector3Int(-1, 1, 0), // LU
    };

    public Vector3Int WorldPosToCellPos(Vector3 worldPos)
    {
        return CellGrid.WorldToCell(worldPos);
    }

    public bool CanGo(Entity self, Vector3 worldPos, bool ignoreObjects = false)
    {
        return CanGo(self, WorldPosToCellPos(worldPos), ignoreObjects);
    }

    public bool CanGo(Entity self, Vector3Int cellPos, bool ignoreObjects = false, int extraCell = 0)
    {
        int extraCells = extraCell;
        if (self != null)
            extraCells = self.ExtraCells;

        for (int dx = -extraCells; dx <= extraCells; dx++)
        {
            for (int dy = -extraCells; dy <= extraCells; dy++)
            {
                Vector3Int checkPos = new Vector3Int(cellPos.x + dx, cellPos.y + dy);

                if (CanGoInternal(self, checkPos, ignoreObjects) == false)
                {
                    return false;
                }
            }
        }

        return true;
    }

    bool CanGoInternal(Entity self, Vector3Int targetCellPos, bool checkActorCollision = true)
    {
        // 맵 범위 밖으로 가려면 통과 불가
        if (targetCellPos.x < MinX || targetCellPos.x > MaxX)
        {
            return false;
        }

        if (targetCellPos.y < MinY || targetCellPos.y > MaxY)
        {
            return false;
        }

        int indexX = targetCellPos.x - MinX;
        int indexY = MaxY - targetCellPos.y;

        // 충돌 영역이면 못 간다
        if (_mapCollision[indexX, indexY])
        {
            return false;
        }

        if (checkActorCollision)
        {
            // 다른 액터 있으면 못간다.
            if (_actorCollision[indexX, indexY] != null)
            {
                return false;
            }
        }

        return true;
    }

    public List<Vector3Int> FindPath(Entity self, Vector3Int startCellPos, Vector3Int destCellPos, int maxLevel = 10)
    {
        // 각 위치별 현재 위치에서 최적의 H 값 기록
        Dictionary<Vector3Int, int> bestH = new Dictionary<Vector3Int, int>();
        
        // 목표 좌표 -> 시작 좌표 경로 기록, 움직일 때에는 시작 -> 목표 좌표로 가야하기에 추후에 뒤집어야 한다.
        Dictionary<Vector3Int, Vector3Int> reachablePathMap = new Dictionary<Vector3Int, Vector3Int>();

        PriorityQueue<PathFindingNode> pq = new PriorityQueue<PathFindingNode>(); // OpenList

        Vector3Int curCellPos = startCellPos;

        // destCellPos에 도착 못하면 가장 가까운 애로 설정, 이래야 몬스터가 가만히 있지 않고 여기저기 움직인다.
        Vector3Int closestCellPos = startCellPos;
        int closestH = (destCellPos - curCellPos).sqrMagnitude;

        // 시작 정보 할당
        {
            int h = (destCellPos - curCellPos).sqrMagnitude;
            pq.Push(new PathFindingNode() { H = h, CellPos = curCellPos, Level = 1 });
            reachablePathMap[curCellPos] = curCellPos;
            bestH[curCellPos] = h;
        }

        while (pq.Count > 0)
        {
            // 제일 좋은 후보를 찾는다
            PathFindingNode node = pq.Pop();
            curCellPos = node.CellPos;

            // 목적지 도착했으면 바로 종료.
            if (curCellPos == destCellPos)
            {
                break;
            }

            // 일정 거리 이상 추격하지 않음
            if (node.Level >= maxLevel)
            {
                break;
            }

            // 이동할 수 있는 좌표인지 확인
            foreach (Vector3Int delta in _delta)
            {
                Vector3Int nextCellPos = curCellPos + delta;

                // 갈 수 없는 장소면 스킵.
                if (CanGo(self, nextCellPos) == false)
                {
                    continue;
                }

                int nextH = (destCellPos - nextCellPos).sqrMagnitude;

                // 일단 없으면 bestH에 최고값으로 추가해서 아래 로직 작동하게 만든다.
                if (bestH.ContainsKey(nextCellPos) == false)
                {
                    bestH[nextCellPos] = int.MaxValue;
                }

                // 현재 경로보다 더 좋은 경로 못찾았으면 처리 안함.
                if (bestH[nextCellPos] <= nextH)
                {
                    continue;
                }

                bestH[nextCellPos] = nextH;

                pq.Push(new PathFindingNode() { H = nextH, CellPos = nextCellPos, Level = node.Level + 1 });
                reachablePathMap[nextCellPos] = curCellPos;

                // 목적지 못가면 가장 가까운 위치 추적, 이 위치를 통해 가만히 있지 않고 이리저리 움직인다.
                if (nextH < closestH)
                {
                    closestH = nextH;
                    closestCellPos = nextCellPos;
                }
            }
        }
         
        // 목표 지점이 갈 수 있는 경로에 포함되지 못했을 경우, 가장 가까운 경로를 목표 경로로 설정
        if (reachablePathMap.ContainsKey(destCellPos) == false)
        {
            return MakePath(reachablePathMap, closestCellPos);
        }

        return MakePath(reachablePathMap, destCellPos);
    }

    List<Vector3Int> MakePath(Dictionary<Vector3Int, Vector3Int> reachablePathMap, Vector3Int destCellPos)
    {
        if (!reachablePathMap.TryGetValue(destCellPos, out Vector3Int curCellPos))
        {
            Debug.LogError($"Pathfinding failed: Destination unreachable. {curCellPos}");
            return new List<Vector3Int>();
        }

        Stack<Vector3Int> pathStack = new Stack<Vector3Int>();

        // 목표 좌표 -> 시작 좌표로 역으로 가면서 스택에 저장해서 경로를 뒤집는다.
        do
        {
            pathStack.Push(curCellPos);
            curCellPos = reachablePathMap[curCellPos];
        }
        while (curCellPos != pathStack.Peek());

        return pathStack.ToList();
    }
    #endregion
}