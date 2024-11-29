using Google.Protobuf;
using ServerCore;
using System;
using System.Collections;
using System.Collections.Generic;
using System.Net;
using UnityEngine;

public class NetworkManager
{
    public string AccountName { get; set; }
    public int TokenDbId { get; set; }
    public int Token { get; set; }

    ServerSession _session = new ServerSession();
    public void Send(IMessage packet)
    {
        _session.Send(packet);
    }

    public void ConnectToGameServer(ServerInfo info)
    {
        // DNS 활용
        IPAddress ipAddr = IPAddress.Parse(info.IpAddress);
        IPEndPoint endPoint = new IPEndPoint(ipAddr, info.Port); // 포트는 8888번으로 설정
        Connector connector = new Connector();

        connector.Connect(endPoint,
            () =>
            {
                return _session;
            });
    }

    public void Update()
    {
        List<PacketMessage> packetList = PacketQueue.Instance.PopAll();
        foreach (PacketMessage packet in packetList)
        {
            Action<PacketSession, IMessage> handler = PacketManager.Instance.GetPacketHandler(packet.Id);
            if(handler != null)
            {
                handler.Invoke(_session, packet.Message);
            }
        }

    }

}
