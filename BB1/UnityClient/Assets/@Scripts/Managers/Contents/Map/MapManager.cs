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

// TODO: �Ʒ� �κ� ������ ���缭 �ٽ� Ȯ�� �ʿ�
public class MapManager
{
    // TODO: ������ ���� �󿡴� char�� �Ȱ��� Enum ���� ���� �ھƼ� �� ����� ����. ���Ŀ� ������ ���� ������ ��� ã���� �̰� �ٲ���.
    public class MapToolConstants
    {
        public const char MAP_TOOL_WALL = '0';
        public const char MAP_TOOL_NONE = '1';
    }

    public GameObject Map { get; private set; }
    public string MapName { get; private set; }
    public Grid CellGrid { get; private set; } // Ÿ�ϸ� �ý����� �⺻ �����̴�. �� Ÿ���� ��ǥ ������� ������ �� �ֵ��� �׸��� ���·� ����
    public Dictionary<Vector3Int, Entity> _cells = new Dictionary<Vector3Int, Entity>();

    public int MinX { get; private set; }
    public int MaxX { get; private set; }
    public int MinY { get; private set; }
    public int MaxY { get; private set; }

    // �� ��ü�� �浹ó�� �Ǵ� ��, ��ֹ� ���� �ִ��� üũ�ϱ� ����
    bool[,] _mapCollision;

    // Ư�� ��ġ�� ������Ʈ�� �ִ��� üũ�ϱ� ����
    Entity[,] _entityCollision;


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

        // Collision ���� ����
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
}