using CSharpServer;
using ServerCore;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;


class PacketHandler
{
    public static void C_ChatHandler(PacketSession session, IPacket ipacket)
    {
        C_Chat packet = ipacket as C_Chat;
        ClientSession clientSession = session as ClientSession;

        if(clientSession.Room == null)
        {
            return;
        }

        clientSession.Room.Broadcast(clientSession, packet.chatMessage);
    }
}
