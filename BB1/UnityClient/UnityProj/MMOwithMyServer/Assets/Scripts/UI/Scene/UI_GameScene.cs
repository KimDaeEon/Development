using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class UI_GameScene : UI_Scene
{
    public UI_Stat StatUI { get; private set; }
    public UI_Inventory InvenUI { get; private set; }

    public override void Init()
    {
        base.Init();

        InvenUI = GetComponentInChildren<UI_Inventory>();
        StatUI = GetComponentInChildren<UI_Stat>();

        // 처음부터 스탯이나 아이템창 보이면 안되니 안보이게 해둔다.
        StatUI.gameObject.SetActive(false);
        InvenUI.gameObject.SetActive(false);
    }
}
