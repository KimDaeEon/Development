using ServerCore;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;


class PacketHandler
{
    public static void S_PlayerInfoReqHandler(PacketSession session, IPacket ipacket)
    {
        S_PlayerInfoReq packet = ipacket as S_PlayerInfoReq;

        Console.WriteLine($"PlayerInfoReq: name:{packet.name}, playerId:{packet.playerId}");
    }
}
