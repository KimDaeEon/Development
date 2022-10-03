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

        if (clientSession.Room == null)
        {
            return;
        }

        // 함수가 실행되는 시점에 참조 값을 붙들어 두기 위함, c++ shared_ptr 이라 보면 된다.
        GameRoom room = clientSession.Room;
        if (room != null)
        {
            room.Push(
                () => room.Broadcast(clientSession, packet.chatMessage)
                );
        }
    }
}
