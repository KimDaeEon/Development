using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.EventSystems;
using UnityEngine.UI;

public class UI_LoginScene : UI_Scene
{
    enum GameObjects
    {
        AccountName,
        Password
    }

    enum ButtonImages
    {
        CreateBtn,
        LoginBtn
    }
    public override void Init()
    {
        base.Init();

        Bind<GameObject>(typeof(GameObjects));
        Bind<Image>(typeof(ButtonImages));

        GetImage((int)ButtonImages.CreateBtn).gameObject.BindEvent(OnClickCreateButton);
        GetImage((int)ButtonImages.LoginBtn).gameObject.BindEvent(OnClickLoginButton);
    }

    public void OnClickCreateButton(PointerEventData evt)
    {
        string account = Get<GameObject>((int)GameObjects.AccountName).GetComponent<InputField>().text;
        string password = Get<GameObject>((int)GameObjects.Password).GetComponent<InputField>().text;

        Debug.Log("OnClickCreateButton");

        ReqCreateAccount reqPacket = new ReqCreateAccount()
        {
            AccountName = account,
            Password = password
        };

        Managers.Web.SendPostRequest<ResCreateAccount>("account/create", reqPacket, (resPacket) =>
        {
            Debug.Log($"ResCreateAcount result {resPacket.result}");

            Get<GameObject>((int)GameObjects.AccountName).GetComponent<InputField>().text = "";
            Get<GameObject>((int)GameObjects.Password).GetComponent<InputField>().text = "";
        });
    }

    public void OnClickLoginButton(PointerEventData evt)
    {
        string account = Get<GameObject>((int)GameObjects.AccountName).GetComponent<InputField>().text;
        string password = Get<GameObject>((int)GameObjects.Password).GetComponent<InputField>().text;

        // 게임 서버 로그인을 위해 들고 있는 이름
        Managers.Network.AccountName = account;

        Debug.Log("OnClickLoginButton");

        ReqLoginAccount reqPacket = new ReqLoginAccount()
        {
            AccountName = account,
            Password = password
        };

        Managers.Web.SendPostRequest<ResLoginAccount>("account/login", reqPacket, (resPacket) =>
        {
            Debug.Log($"ResCreateAcount result {resPacket.result}");
            Get<GameObject>((int)GameObjects.AccountName).GetComponent<InputField>().text = "";
            Get<GameObject>((int)GameObjects.Password).GetComponent<InputField>().text = "";

            if (resPacket.result)
            {
                Managers.Network.Token = resPacket.Token;
                Managers.Network.TokenDbId = resPacket.TokenDbId;
                UI_SelectServerPopup popup = Managers.UI.GetPopupUI<UI_SelectServerPopup>();
                popup.SetServerInfos(resPacket.ServerList);
            }

        });
    }
}
