using UnityEngine;
using UnityEngine.EventSystems;

public abstract class BaseScene : MonoBehaviour
{
    public CustomEnum.EScene SceneType { get; protected set; } = CustomEnum.EScene.Unknown;

    protected virtual void Awake()
	{
		// EventSystem 이 UI와 입력 이벤트를 처리해주는 핵심 클래스이다. 그래서 추가하는 것
		Object obj = GameObject.FindObjectOfType(typeof(EventSystem));
		if (obj == null)
			Managers.Resource.Instantiate("UI/EventSystem").name = "@EventSystem";
	}

	protected virtual void Start() { }
	protected virtual void Update() { }

	public abstract void Clear();
}
