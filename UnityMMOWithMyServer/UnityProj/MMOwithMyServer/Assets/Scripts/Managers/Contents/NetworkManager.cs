using DummyClient;
using Google.Protobuf;
using ServerCore;
using System;
using System.Collections;
using System.Collections.Generic;
using System.Net;
using UnityEngine;

public class NetworkManager
{
    ServerSession _session = new ServerSession();
    public void Send(ArraySegment<byte> sendBuff)
    {
        _session.Send(sendBuff);
    }

    public void Init()
    {
        // DNS 활용
        string host = Dns.GetHostName();
        IPHostEntry ipHost = Dns.GetHostEntry(host);
        IPAddress ipAddr = ipHost.AddressList[0];
        IPEndPoint endPoint = new IPEndPoint(ipAddr, 8888); // 포트는 8888번으로 설정
        Connector connector = new Connector();

        connector.Connect(endPoint,
            () =>
            {
                return _session;
            });
    }

    void Update()
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
