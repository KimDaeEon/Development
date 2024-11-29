using Google.Protobuf.Protocol;
using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.EventSystems;
using UnityEngine.UI;

public class UI_SelectServerPopup_Item : UI_Popup
{
    public ServerInfo Info { get; set; }
    
    enum Buttons
    {
        SelectServerButton,
    }

    enum Texts
    {
        ServerNameText,
    }
    
    public override void Init()
    {
        Bind<Button>(typeof(Buttons));
        Bind<Text>(typeof(Texts));

        GetButton((int)Buttons.SelectServerButton).gameObject.BindEvent(OnClickButton);
    }

    void OnClickButton(PointerEventData evt)
    {
        Managers.Network.ConnectToGameServer(Info);
        Managers.Scene.LoadScene(Define.Scene.Game);
        Managers.UI.ClosePopupUI();
    }

    public void RefreshUI()
    {
        if(Info == null)
        {
            return;
        }

        GetText((int)Texts.ServerNameText).text = Info.Name;
    }
}
