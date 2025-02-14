using Google.Protobuf.Protocol;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.EventSystems;
using static CustomEnum;

public class GameScene : BaseScene
{
    protected override void Awake()
    {
        base.Awake();

#if UNITY_EDITOR
        gameObject.AddComponent<CaptureScreenShot>();
#endif

        Debug.Log("@>> GameScene Init()");
        SceneType = EScene.GameScene;

        Managers.Map.LoadMap("DefaultMap");
        C_EnterGame enterGame = new C_EnterGame();
        Managers.Network.Send(enterGame);
    }

    protected override void Start()
    {
        base.Start();
    }


    public override void Clear()
    {
    }

    void OnApplicationQuit()
    {
        Managers.Network.GameServer.Disconnect();
    }
}