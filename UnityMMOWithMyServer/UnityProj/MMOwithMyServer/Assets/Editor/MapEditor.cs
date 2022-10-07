using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.Tilemaps;
using System.IO;

#if UNITY_EDITOR
using UnityEditor;
#endif

public class MapEditor
{
#if UNITY_EDITOR

    // 단축키 관련 문자, % = Ctrl, # = Shift, & = Alt
    // 아래 같은 경우는 ctrl + q 를 누르면 아래 함수가 작동된다고 보면 된다.
    [MenuItem("Tools/GenerateMap %q")]
    private static void GenerateMap()
    {
        GameObject[] gameObjects = Resources.LoadAll<GameObject>("Prefabs/Map");

        foreach (GameObject gameObject in gameObjects)
        {
            if (gameObject == null)
            {
                Debug.Log("[Error] Map object is null!");
                return;
            }

            Tilemap tileMap = Util.FindChild<Tilemap>(gameObject, "Tilemap_Collision", true);
            if (tileMap == null)
            {
                Debug.Log("[Error] tileMap is null!");
                return;
            }

            using (var writer = File.CreateText($"Assets/Resources/Map/{gameObject.name}.txt"))
            {
                writer.WriteLine(tileMap.cellBounds.xMin);
                writer.WriteLine(tileMap.cellBounds.xMax);
                writer.WriteLine(tileMap.cellBounds.yMin);
                writer.WriteLine(tileMap.cellBounds.yMax);

                // 장애물이 있으면 1, 없으면 0을 표시 서버에서 해당 정보를 통해 이동로직을 작동시킨다.
                // 예를 들면 아래와 같은 형식으로 길찾기용 파일과 같이 적힌다.
                //-11
                //9
                //-4
                //5
                //000000000000000000000
                //000011111100000000000
                //000011111100000000000
                //000011011100000000000
                //110011011100000000010
                //000000000000000000000
                //000000000000100000000
                //000000000000000000000
                //000000000000000000000
                //000001000000000000010

                for (int y = tileMap.cellBounds.yMax; y >= tileMap.cellBounds.yMin; y--)
                {
                    for (int x = tileMap.cellBounds.xMin; x <= tileMap.cellBounds.xMax; x++)
                    {
                        TileBase tile = tileMap.GetTile(new Vector3Int(x, y, 0));
                        if (tile != null)
                        {
                            writer.Write("1");
                        }
                        else
                        {
                            writer.Write("0");
                        }
                    }
                    writer.WriteLine();
                }
            } // using (var writer = File.CreateText("Assets/Resources/Map/output.txt"))
        } // foreach (GameObject gameObject in gameObjects)

    }
#endif
}
