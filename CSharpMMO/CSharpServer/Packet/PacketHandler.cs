using CSharpServer;
using Google.Protobuf.Protocol;
using Google.Protobuf;
using ServerCore;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;


class PacketHandler
{
    public static void C_ChatHandler(PacketSession session, IMessage packet)
    {
        S_Chat chatPacket = packet as S_Chat;
        ClientSession serverSession = session as ClientSession;

        Console.WriteLine(chatPacket.Context);
    }
}
