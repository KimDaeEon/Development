using System.Collections;
using System.Collections.Generic;
using UnityEngine;

// TODO: Json ���� �Ʒ� �������� �ȵǴ��� �׽�Ʈ
public class ReqCreateAccount
{
    public string AccountName;
    public string Password;
}

public class ResCreateAccount
{
    public bool result;
}

public class ReqLoginAccount
{
    public string AccountName;
    public string Password;
}

public class ServerInfo
{
    public string Name;
    public string IpAddress;
    public int Port;
    public int CrowdedLevel;// ���� ȥ�� ����
}

public class ResLoginAccount
{
    public bool result;
    public int AccountId;
    public int TokenDbId;
    public int Token;
    public List<ServerInfo> ServerList = new List<ServerInfo>();
}


public class WebContents
{
    //public static void SendCreateAccount (string account, string password)
    //{
    //    ReqCreateAccount reqPacket = new ReqCreateAccount()
    //    {
    //        AccountName = account,
    //        Password = password
    //    };

    //    Managers.Web.SendPostRequest<ResCreateAccount>("account/create", reqPacket, (resPacket) =>
    //    {
    //        Debug.Log($"ResCreateAcount result {resPacket.result}");
    //    });
    //}

    //public static void SendLoginAccount(string account, string password)
    //{
    //    ReqLoginAccount reqPacket = new ReqLoginAccount()
    //    {
    //        AccountName = account,
    //        Password = password
    //    };

    //    Managers.Web.SendPostRequest<ResLoginAccount>("account/login", reqPacket, (resPacket) =>
    //    {
    //        Debug.Log($"ResLoginAccount result {resPacket.result}");
    //    });
    //}
}