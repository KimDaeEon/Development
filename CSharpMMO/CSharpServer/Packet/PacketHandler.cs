using CSharpServer;
using ServerCore;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;


class PacketHandler
{
    public static void C_ChatHandler(PacketSession session, IPacket iPacket)
    {
        //C_Chat chatPacket = iPacket as C_Chat;
        //ClientSession clientSession = session as ClientSession;

        //if (clientSession.Room == null)
        //{
        //    return;
        //}

        //// 함수가 실행되는 시점에 참조 값을 붙들어 두기 위함, c++ shared_ptr 이라 보면 된다.
        //GameRoom room = clientSession.Room;
        //if (room != null)
        //{
        //    room.Push(
        //        () => room.Broadcast(clientSession, chatPacket.chatMessage)
        //        );
        //}
    }

    internal static void C_LeaveGameHandler(PacketSession session, IPacket iPacket)
    {
        //C_LeaveGame packet = iPacket as C_LeaveGame;
        ClientSession clientSession = session as ClientSession;

        if(clientSession.Room == null)
        {
            return;
        }

        GameRoom room = clientSession.Room;
        room.Push(()=> room.Leave(clientSession));
    }

    internal static void C_MoveHandler(PacketSession session, IPacket iPacket)
    {
        C_Move packet = iPacket as C_Move;
        ClientSession clientSession = session as ClientSession;

        if(clientSession.Room == null)
        {
            return;
        }

        //Console.WriteLine($"session {clientSession.SessionId}'s position : {packet.posX}, {packet.posY}, {packet.posZ}");

        GameRoom room = clientSession.Room;
        room.Push(() => room.Move(clientSession, packet));
    }
}
