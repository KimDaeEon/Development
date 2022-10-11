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

    // ����Ű ���� ����, % = Ctrl, # = Shift, & = Alt
    // �Ʒ� ���� ���� ctrl + q �� ������ �Ʒ� �Լ��� �۵��ȴٰ� ���� �ȴ�.
    [MenuItem("Tools/GenerateMap %q")]
    private static void GenerateMap()
    {
        GameObject[] gameObjects = Resources.LoadAll<GameObject>("Prefabs/Map");

        foreach (GameObject gameObject in gameObjects)
        {
            Tilemap tileMapBase = Util.FindChild<Tilemap>(gameObject, "Tilemap_Base", true);
            Tilemap tileMapCollision = Util.FindChild<Tilemap>(gameObject, "Tilemap_Collision", true);

            if (tileMapCollision == null)
            {
                Debug.Log("[Error] tileMap is null!");
                return;
            }

            using (var writer = File.CreateText($"Assets/Resources/Map/{gameObject.name}.txt"))
            {
                writer.WriteLine(tileMapBase.cellBounds.xMin);
                writer.WriteLine(tileMapBase.cellBounds.xMax);
                writer.WriteLine(tileMapBase.cellBounds.yMin);
                writer.WriteLine(tileMapBase.cellBounds.yMax);

                // ��ֹ��� ������ 1, ������ 0�� ǥ�� �������� �ش� ������ ���� �̵������� �۵���Ų��.
                // ���� ��� �Ʒ��� ���� �������� ��ã��� ���ϰ� ���� ������.
                //-11 (xMin)
                //9 (xMax)
                //-4 (yMin)
                //5 (yMax)
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

                for (int y = tileMapBase.cellBounds.yMax; y >= tileMapBase.cellBounds.yMin; y--)
                {
                    for (int x = tileMapBase.cellBounds.xMin; x <= tileMapBase.cellBounds.xMax; x++)
                    {
                        TileBase tile = tileMapCollision.GetTile(new Vector3Int(x, y, 0));
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
