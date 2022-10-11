using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class GameScene : BaseScene
{
    protected override void Init()
    {
        base.Init();

        SceneType = Define.Scene.Game;

        Managers.MapManager.LoadMap(1);

        GameObject player = Managers.Resource.Instantiate("Actor/Player");
        player.name = "Player";
        Managers.ObjectManager.Add(player);

        for (int i = 0; i < 8; i++)
        {
            GameObject monster = Managers.Resource.Instantiate("Actor/Monster");
            monster.name = "Monster" + i;
            Managers.ObjectManager.Add(monster);

            Vector3Int pos = new Vector3Int()
            {
                x = Random.Range(-10, 10),
                y = Random.Range(-10, 10)
            };

            MonsterController controller = monster.GetComponent<MonsterController>();
            controller.CellPos = pos;
        }
    }

    public override void Clear()
    {

    }
}
