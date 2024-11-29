using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

public class ReqCreateAccount
{
    public string AccountName { get; set; }
    public string Password { get; set; }
}

public class ResCreateAccount
{
    public bool result { get; set; }
}

public class ReqLoginAccount
{
    public string AccountName { get; set; }
    public string Password { get; set; }
}

public class ServerInfo
{
    public string Name { get; set; }
    public string IpAddress { get; set; }
    public int Port { get; set; }
    public int CrowdedLevel { get; set; } // 서버 혼잡 상태
}

public class ResLoginAccount
{
    public bool result { get; set; }
    public int TokenDbId { get; set; }
    public int Token { get; set; }
    public List<ServerInfo> ServerList { get; set; } = new List<ServerInfo>();
}

