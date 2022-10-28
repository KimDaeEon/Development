using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Managers : MonoBehaviour
{
    static Managers s_instance; // 유일성이 보장된다
    static Managers Instance { get { Init(); return s_instance; } } // 유일한 매니저를 갖고온다

    #region Contents
    MapManager _mapManager = new MapManager();
    ObjectManager _objManager = new ObjectManager();
    NetworkManager _networkManager = new NetworkManager();
    ItemInventoryManager _itemInventoryManager = new ItemInventoryManager();

    public static MapManager Map { get { return Instance._mapManager; } }
    public static ObjectManager Object { get { return Instance._objManager; } }
    public static NetworkManager Network { get { return Instance._networkManager; } }
    public static ItemInventoryManager ItemInventory { get { return Instance._itemInventoryManager; } }

    #endregion

    #region Core
    DataManager _dataManager = new DataManager();
    PoolManager _poolManager = new PoolManager();
    ResourceManager _resourceManager = new ResourceManager();
    SceneManagerEx _sceneManager = new SceneManagerEx();
    SoundManager _soundManager = new SoundManager();
    UIManager _uiManager = new UIManager();

    public static DataManager Data { get { return Instance._dataManager; } }
    public static PoolManager Pool { get { return Instance._poolManager; } }
    public static ResourceManager Resource { get { return Instance._resourceManager; } }
    public static SceneManagerEx Scene { get { return Instance._sceneManager; } }
    public static SoundManager Sound { get { return Instance._soundManager; } }
    public static UIManager UI { get { return Instance._uiManager; } }
	#endregion

	void Start()
    {
        Init();
	}

    void Update()
    {
        _networkManager.Update();
    }

    static void Init()
    {
        if (s_instance == null)
        {
			GameObject go = GameObject.Find("@Managers");
            if (go == null)
            {
                go = new GameObject { name = "@Managers" };
                go.AddComponent<Managers>();
            }

            DontDestroyOnLoad(go);
            s_instance = go.GetComponent<Managers>();

            s_instance._networkManager.Init();
            s_instance._dataManager.Init();
            s_instance._poolManager.Init();
            s_instance._soundManager.Init();
        }		
	}

    public static void Clear()
    {
        Sound.Clear();
        Scene.Clear();
        UI.Clear();
        Pool.Clear();
    }
}
