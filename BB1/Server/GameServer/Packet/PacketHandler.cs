using GameServer;
using ServerCore;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using GameServer.DB;
using GameServer.Data;

using Google.Protobuf;
using Google.Protobuf.Protocol;

class PacketHandler
{
    public static void C_TestHandler(PacketSession session, IMessage packet)
    {
        C_Test pkt = packet as C_Test;
        System.Console.WriteLine(pkt);


    }

    internal static void C_EnterGameHandler(PacketSession session, IMessage message)
    {
        throw new NotImplementedException();
    }

    internal static void C_LeaveGameHandler(PacketSession session, IMessage message)
    {
        throw new NotImplementedException();
    }
}
