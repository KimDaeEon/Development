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
        int y = MaxY - cellPos.y; // ���� _collision �迭 ��ǥ�� ���Ϸ���, �� ���� ��� _collision[0][x]�̹Ƿ� �̷��� �ؾ��Ѵ�.

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
            collision.SetActive(false); // ���⼭ ���� ������ �浹 ó���� ���� �����صξ��� ��������Ʈ�� ���� �� ���̴�.
        }

        CurrentGrid = gameObject.GetComponent<Grid>();

        // �տ��� Tilemap_Collision ���� ������ Collision ���� �ؽ�Ʈ ����
        TextAsset mapText = Managers.Resource.Load<TextAsset>($"Map/{mapName}");
        StringReader reader = new StringReader(mapText.text);

        MinX = int.Parse(reader.ReadLine());
        MaxX = int.Parse(reader.ReadLine());
        MinY = int.Parse(reader.ReadLine());
        MaxY = int.Parse(reader.ReadLine());

        int xCount = MaxX - MinX + 1;
        int yCount = MaxY - MinY + 1;
        _collision = new bool[yCount, xCount]; // y �� ���� �ؾ��� for �� �� ���� �� �������̴�.

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

    // �Ʒ� �Լ��� ���� ���� �־ �ϴ� �ִ��� ������� �� �ִ� ��δ� �����Ƿ� null �� �������� �ʴ´�.
    public List<Vector3Int> FindPath(Vector3Int startCellPos, Vector3Int destCellPos, bool ignoreDestCollision = false)
    {
        List<Pos> path = new List<Pos>();

        // F = G + H; // F�� Fuzzy �� �ǹ��ϴ� �� ����. �ٻ�ġ�� �ǹ̷�.. https://ko.wikipedia.org/wiki/%ED%8D%BC%EC%A7%80_%EB%85%BC%EB%A6%AC
        // G : ���������� ���� ��ġ������ ��θ� �̵��ϴµ� �ҿ�Ǵ� ��� // Greedy?
        // H(heuristics) : ���� ��ġ���� ���� ������ ������ '����' �̵� ���, �޸���ƽ�̶� ��ó�� �������� ����̴�. �������� ����

        // �湮�� �� ����, ������ ������ visited �� ǥ���� ���� ��µ� A* ��ü���� Closed List �� ǥ���� ����.
        bool[,] closed = new bool[SizeY, SizeX];

        // �� �� �ִ� �� ����, ���� ��� �߰��� �ȵǾ����� MaxValue �� ����, A* �� Open List
        int[,] open = new int[SizeY, SizeX];
        for (int y = 0; y < SizeY; y++)
        {
            for (int x = 0; x < SizeX; x++)
            {
                open[y, x] = Int32.MaxValue;
            }
        }

        // �θ� ��� ��ǥ ������ ����, ��� �߰� �Ŀ� ������ �θ� ���󰡾��ϱ� ����
        Pos[,] parent = new Pos[SizeY, SizeX];

        // ���� ���� �ĺ�(F ���� �ĺ�)�� �ٷ� �̾ƿ��� ����
        PriorityQueue<PQNode> pq = new PriorityQueue<PQNode>();

        Pos pos = Cell2Pos(startCellPos);
        Pos dest = Cell2Pos(destCellPos);

        // �������� Open ����Ʈ�� �߰�
        open[pos.Y, pos.X] = CalcHeuristics(dest.Y, dest.X, pos.Y, pos.X);
        pq.Push(new PQNode() { F = CalcHeuristics(dest.Y, dest.X, pos.Y, pos.X), G = 0, Y = pos.Y, X = pos.X });
        parent[pos.Y, pos.X] = new Pos(pos.Y, pos.X);

        while (pq.Count > 0)
        {
            // ���� �� �� �ִ� �� �� ���� ���� ��� �Ҵ�
            PQNode node = pq.Pop();

            // ��ã�� �������� �ش� ��带 �̹� �湮������ ��ŵ
            if (closed[node.Y, node.X])
            {
                continue;
            }

            // �湮
            closed[node.Y, node.X] = true;

            // �������� �����ߴٸ� ����
            if (node.Y == dest.Y && node.X == dest.X)
            {
                break;
            }

            // ��,��,��,�� �̵� ���� Ȯ���ؼ� Open List �� �߰�
            for (int i = 0; i < _deltaY.Length; i++)
            {
                Pos next = new Pos(node.Y + _deltaY[i], node.X + _deltaX[i]);

                // ignoreDestCollision �� false ��, ��ǥ ��ġ�� ���� �浹üũ ���� �ʴ´�.
                if (!ignoreDestCollision || next.Y != dest.Y || next.X != dest.X)
                {
                    // ��ȿ ���� ����ų�, ������ ���� ������ ��ŵ
                    if (CanGo(Pos2Cell(next)) == false || Managers.ObjectManager.Find(Pos2Cell(next)) != null)
                    {
                        continue;
                    }
                }

                // �̹� �湮������ ��ŵ
                if (closed[next.Y, next.X])
                {
                    continue;
                }

                int ng = node.F; // �� �κ� ��ġ�� �������� �� �� �ϴ�.
                int nh = CalcHeuristics(dest.Y, dest.X, next.Y, next.X);

                // �̹� ���� ��ο��� ã�� ���� �� ���ٸ� ��ŵ
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

    // �θ� ��带 ������ �����ؼ� �� �� �ִ� ���� ã�´�.
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

    // Vector3Int �� Pos ���� ��ǥ ǥ�� ���̷� ���� ����.
    // x, y  ��ǥ�� y �ִ� ��ǥ = Pos y ��ǥ 0, x �ּ� ��ǥ = Pos x ��ǥ 0
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
