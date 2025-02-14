using UnityEngine;

// 매니저 클래스 관리 + Update 같은 Unity 종속적 함수를 묶어서 실행하기 위한 클래스
public class Managers : MonoBehaviour
{
	public static bool Initialized { get; set; }

	private static Managers s_instance;
    public static Managers Instance { get { Init(); return s_instance; } }

    #region Contents

    private GameManager _game = new GameManager();
    private GameObjectManager _object = new GameObjectManager();
    private MapManager _map = new MapManager();
    
    public static GameManager Game { get { return Instance?._game; } }
    public static GameObjectManager Object { get { return Instance?._object; } }
    public static MapManager Map { get { return Instance?._map; } }
    
    #endregion

    #region Core

    private DataManager _data = new DataManager();
    private PoolManager _pool = new PoolManager();
    private ResourceManager _resource = new ResourceManager();
    private SceneManagerEx _scene = new SceneManagerEx();
    private SoundManager _sound = new SoundManager();
    private UIManager _ui = new UIManager();
    NetworkManager _network = new NetworkManager();

    public static DataManager Data { get { return Instance?._data; } }
    public static PoolManager Pool { get { return Instance?._pool; } }
    public static ResourceManager Resource { get { return Instance?._resource; } }
    public static SceneManagerEx Scene { get { return Instance?._scene; } }
    public static SoundManager Sound { get { return Instance?._sound; } }
    public static UIManager UI { get { return Instance?._ui; } }
    public static NetworkManager Network { get { return Instance?._network; } }

    #endregion

    #region Language
    private static CustomEnum.ELanguage _language = CustomEnum.ELanguage.Korean;
    public static CustomEnum.ELanguage Language
    {
        get { return _language; }
        set
        {
            _language = value;
        }
    }

    public static string GetText(string textId)
    {
        switch (_language)
        {
            case CustomEnum.ELanguage.Korean:
                break;
            case CustomEnum.ELanguage.English:
                break;
            case CustomEnum.ELanguage.French:
                break;
            case CustomEnum.ELanguage.SimplifiedChinese:
                break;
            case CustomEnum.ELanguage.TraditionalChinese:
                break;
            case CustomEnum.ELanguage.Japanese:
                break;
        }

        return "";
    }
    #endregion

    public static void Init()
    {
        if (s_instance == null && Initialized == false)
        {
            // 활성화된 Scene에서 이름이 @Managers인 객체를 찾는다.
			GameObject go = GameObject.Find("@Managers");
            if (go == null)
            {
                go = new GameObject { name = "@Managers" };
                go.AddComponent<Managers>();
            }

            DontDestroyOnLoad(go);
            s_instance = go.GetComponent<Managers>();
            s_instance._sound.Init();

            Initialized = true;
        }
    }

	public void Update()
	{
		_network?.Update();
	}

	public static void Clear()
    {
        Sound.Clear();
        Scene.Clear();
        UI.Clear();
        Pool.Clear();
    }
}
