using DummyClient;
using ServerCore;
using System;
using System.Collections;
using System.Collections.Generic;
using System.Net;
using UnityEngine;

public class NetworkManager : MonoBehaviour
{
    ServerSession _session = new ServerSession();
    public void Send(ArraySegment<byte> sendBuff)
    {
        _session.Send(sendBuff);
    }

    // Start is called before the first frame update
    void Start()
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

    // Update is called once per frame
    void Update()
    {
        List<IPacket> packetList = PacketQueue.Instance.PopAll();
        if (packetList != null)
        {
            foreach (IPacket iPacket in packetList)
            {
                PacketManager.Instance.HandlePacket(_session, iPacket);
            }
        }
    }

}
