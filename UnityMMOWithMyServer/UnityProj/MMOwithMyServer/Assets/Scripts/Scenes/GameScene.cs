using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class GameScene : BaseScene
{
    protected override void Init()
    {
        base.Init();

        SceneType = Define.Scene.Game;

        Managers.Map.LoadMap(1);

        Screen.SetResolution(640, 480, false);

        //for (int i = 0; i < 8; i++)
        //{
        //    GameObject monster = Managers.Resource.Instantiate("Actor/Monster");
        //    monster.name = "Monster" + (i+100);
        //    Managers.ObjectManager.Add(i+100, monster);

        //    Vector3Int pos = new Vector3Int()
        //    {
        //        x = Random.Range(-10, 10),
        //        y = Random.Range(-10, 10)
        //    };

        //    MonsterController controller = monster.GetComponent<MonsterController>();
        //    controller.CellPos = pos;
        //}
    }

    public override void Clear()
    {

    }
}
