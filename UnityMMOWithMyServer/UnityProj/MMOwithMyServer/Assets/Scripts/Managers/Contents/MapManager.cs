using System.Collections;
using System.Collections.Generic;
using System;
using UnityEngine;
using System.IO;

public struct Pos
{
    public Pos(int y, int x) { Y = y; X = x; }
    public int Y;
    public int X;
}

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

public class MapManager
{
    public Grid CurrentGrid { get; private set; }
    public int MinX { get; set; }
    public int MaxX { get; set; }
    public int MinY { get; set; }
    public int MaxY { get; set; }

    public int SizeX { get { return MaxX - MinX + 1; } }
    public int SizeY { get { return MaxY - MinY + 1; } }

    bool[,] _collision;

    public bool CanGo(Vector3Int cellPos)
    {
        if (cellPos.x < MinX || cellPos.x > MaxX)
        {
            return false;
        }

        if (cellPos.y < MinY || cellPos.y > MaxY)
        {
            return false;
        }

        int x = cellPos.x - MinX;
        int y = MaxY - cellPos.y; // 위의 _collision 배열 좌표를 구하려면, 맨 위인 경우 _collision[0][x]이므로 이렇게 해야한다.

        return !_collision[y, x];
    }

    public void LoadMap(int mapId)
    {
        DestoryMap("Map_001");

        string mapName = "Map_" + mapId.ToString("000");
        GameObject gameObject = Managers.Resource.Instantiate($"Map/{mapName}");
        gameObject.name = mapName;

        GameObject collision = Util.FindChild(gameObject, "Tilemap_Collision", true);
        if (collision != null)
        {
            collision.SetActive(false); // 여기서 끄지 않으면 충돌 처리를 위해 세팅해두었던 스프라이트가 눈에 띌 것이다.
        }

        CurrentGrid = gameObject.GetComponent<Grid>();

        // 앞에서 Tilemap_Collision 에서 추출한 Collision 관련 텍스트 파일
        TextAsset mapText = Managers.Resource.Load<TextAsset>($"Map/{mapName}");
        StringReader reader = new StringReader(mapText.text);

        MinX = int.Parse(reader.ReadLine());
        MaxX = int.Parse(reader.ReadLine());
        MinY = int.Parse(reader.ReadLine());
        MaxY = int.Parse(reader.ReadLine());

        int xCount = MaxX - MinX + 1;
        int yCount = MaxY - MinY + 1;
        _collision = new bool[yCount, xCount]; // y 를 먼저 해야지 for 문 돌 때에 더 직관적이다.

        for (int y = 0; y < yCount; y++)
        {
            string line = reader.ReadLine();
            for (int x = 0; x < xCount; x++)
            {
                _collision[y, x] = (line[x] == '1' ? true : false);
            }
        }
    }

    public void DestoryMap(string mapName)
    {
        GameObject map = GameObject.Find(mapName);
        if (map != null)
        {
            GameObject.Destroy(map);
            CurrentGrid = null;
        }
    }

    #region A* PathFinding

    int[] _deltaY = new int[] { 1, -1, 0, 0 };
    int[] _deltaX = new int[] { 0, 0, -1, 1 };
    //int[] _cost = new int[] { 10, 10, 10, 10 };

    // 아래 함수는 벽에 막혀 있어도 일단 최대한 가까워질 수 있는 경로는 나오므로 null 은 리턴하지 않는다.
    public List<Vector3Int> FindPath(Vector3Int startCellPos, Vector3Int destCellPos, bool ignoreDestCollision = false)
    {
        List<Pos> path = new List<Pos>();

        // F = G + H; // F가 Fuzzy 를 의미하는 것 같다. 근사치란 의미로.. https://ko.wikipedia.org/wiki/%ED%8D%BC%EC%A7%80_%EB%85%BC%EB%A6%AC
        // G : 시작점부터 현재 위치까지의 경로를 이동하는데 소요되는 비용 // Greedy?
        // H(heuristics) : 현재 위치에서 다음 목적지 까지의 '예상' 이동 비용, 휴리스틱이란 말처럼 개략적인 계산이다. 작을수록 좋음

        // 방문한 곳 정보, 코테할 때에는 visited 란 표현을 자주 썼는데 A* 자체에서 Closed List 란 표현을 쓴다.
        bool[,] closed = new bool[SizeY, SizeX];

        // 갈 수 있는 곳 정보, 아직 경로 발견이 안되었으면 MaxValue 로 설정, A* 의 Open List
        int[,] open = new int[SizeY, SizeX];
        for (int y = 0; y < SizeY; y++)
        {
            for (int x = 0; x < SizeX; x++)
            {
                open[y, x] = Int32.MaxValue;
            }
        }

        // 부모 노드 좌표 정보를 저장, 경로 발견 후에 역으로 부모를 따라가야하기 때문
        Pos[,] parent = new Pos[SizeY, SizeX];

        // 가장 좋은 후보(F 작은 후보)를 바로 뽑아오기 위함
        PriorityQueue<PQNode> pq = new PriorityQueue<PQNode>();

        Pos pos = Cell2Pos(startCellPos);
        Pos dest = Cell2Pos(destCellPos);

        // 시작점을 Open 리스트에 추가
        open[pos.Y, pos.X] = CalcHeuristics(dest.Y, dest.X, pos.Y, pos.X);
        pq.Push(new PQNode() { F = CalcHeuristics(dest.Y, dest.X, pos.Y, pos.X), G = 0, Y = pos.Y, X = pos.X });
        parent[pos.Y, pos.X] = new Pos(pos.Y, pos.X);

        while (pq.Count > 0)
        {
            // 현재 갈 수 있는 곳 중 제일 좋은 노드 할당
            PQNode node = pq.Pop();

            // 길찾기 과정에서 해당 노드를 이미 방문했으면 스킵
            if (closed[node.Y, node.X])
            {
                continue;
            }

            // 방문
            closed[node.Y, node.X] = true;

            // 목적지에 도착했다면 종료
            if (node.Y == dest.Y && node.X == dest.X)
            {
                break;
            }

            // 상,하,좌,우 이동 여부 확인해서 Open List 에 추가
            for (int i = 0; i < _deltaY.Length; i++)
            {
                Pos next = new Pos(node.Y + _deltaY[i], node.X + _deltaX[i]);

                // ignoreDestCollision 이 false 면, 목표 위치에 대해 충돌체크 하지 않는다.
                if (!ignoreDestCollision || next.Y != dest.Y || next.X != dest.X)
                {
                    // 유효 범위 벗어났거나, 벽으로 막혀 있으면 스킵
                    if (CanGo(Pos2Cell(next)) == false || Managers.ObjectManager.Find(Pos2Cell(next)) != null)
                    {
                        continue;
                    }
                }

                // 이미 방문했으면 스킵
                if (closed[next.Y, next.X])
                {
                    continue;
                }

                int ng = node.F; // 이 부분 수치는 이정도면 될 듯 하다.
                int nh = CalcHeuristics(dest.Y, dest.X, next.Y, next.X);

                // 이미 이전 경로에서 찾은 값이 더 낫다면 스킵
                if (open[next.Y, next.X] < ng + nh)
                {
                    continue;
                }

                open[next.Y, next.X] = ng + nh;
                pq.Push(new PQNode() { F = ng + nh, G = ng, Y = next.Y, X = next.X });
                parent[next.Y, next.X] = new Pos(node.Y, node.X);
            }
        }

        return CalcCellPathFromParent(parent, dest);
    }

    int CalcHeuristics(int destY, int destX, int curY, int curX)
    {
        return 10 * ((destY - curY) * (destY - curY) + (destX - curX) * (destX - curX));
    }

    // 부모 노드를 역으로 추적해서 갈 수 있는 길을 찾는다.
    List<Vector3Int> CalcCellPathFromParent(Pos[,] parent, Pos dest)
    {
        List<Vector3Int> cells = new List<Vector3Int>();

        int y = dest.Y;
        int x = dest.X;
        while (parent[y, x].Y != y || parent[y, x].X != x)
        {
            cells.Add(Pos2Cell(new Pos(y, x)));
            Pos pos = parent[y, x];
            y = pos.Y;
            x = pos.X;
        }
        cells.Add(Pos2Cell(new Pos(y, x)));
        cells.Reverse();

        return cells;
    }

    // Vector3Int 와 Pos 간의 좌표 표기 차이로 인해 존재.
    // x, y  좌표계 y 최대 좌표 = Pos y 좌표 0, x 최소 좌표 = Pos x 좌표 0
    Pos Cell2Pos(Vector3Int cell)
    {
        // CellPos -> ArrayPos
        return new Pos(MaxY - cell.y, cell.x - MinX);
    }

    Vector3Int Pos2Cell(Pos pos)
    {
        // ArrayPos -> CellPos
        return new Vector3Int(pos.X + MinX, MaxY - pos.Y, 0);
    }
    #endregion
}
