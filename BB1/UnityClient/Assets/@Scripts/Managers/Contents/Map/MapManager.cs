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

    // 특정 위치에 오브젝트가 있는지 체크하기 위함
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
}