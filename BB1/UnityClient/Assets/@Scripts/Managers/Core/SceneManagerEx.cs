using UnityEngine;
using UnityEngine.SceneManagement;

public class SceneManagerEx
{
    public BaseScene CurrentScene { get { return GameObject.FindObjectOfType<BaseScene>(); } }

    public void LoadScene(CustomEnum.EScene type, Transform parents = null)
    {
        Managers.Clear();
        SceneManager.LoadScene(GetSceneNameByEnum(type));
        //switch (CurrentScene.SceneType)
        //{
        //    case Define.Scene.TitleScene:
        //        Managers.Clear();
        //        SceneManager.LoadScene(GetSceneName(type));
        //        break;
        //    case Define.Scene.GameScene:
        //        Managers.Resource.Destroy(Managers.UI.SceneUI.gameObject);
        //        Managers.Clear();
        //        SceneManager.LoadScene(GetSceneName(type));
        //        break;
        //    case Define.Scene.LobbyScene:
        //        Managers.Resource.Destroy(Managers.UI.SceneUI.gameObject);
        //        Managers.Clear();
        //        SceneManager.LoadScene(GetSceneName(type));
        //        break;
        //}

    }

    private string GetSceneNameByEnum(CustomEnum.EScene type)
    {
        string name = System.Enum.GetName(typeof(CustomEnum.EScene), type);
        return name;
    }

    public void Clear()
    {
        CurrentScene.Clear();
    }
}
