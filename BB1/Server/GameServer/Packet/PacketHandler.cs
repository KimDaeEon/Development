using GameServer;
using ServerCore;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using GameServer.DB;

using Google.Protobuf;
using Google.Protobuf.Protocol;
using System.Net.Sockets;
using static System.Formats.Asn1.AsnWriter;
using GameServer.Data;

class PacketHandler
{
    internal static void C_TestHandler(PacketSession session, IMessage packet)
    {
        C_Test pkt = packet as C_Test;
        ClientSession clientSession = session as ClientSession;

        //if (clientSession.IsVerified == false)
        //{
        //    clientSession.Disconnect();
        //}

        System.Console.WriteLine(pkt);
    }

    internal static void C_EnterGameHandler(PacketSession session, IMessage packet)
    {
        C_EnterGame pkt = packet as C_EnterGame;
        ClientSession clientSession = session as ClientSession;

        System.Console.WriteLine(pkt);

        // 패킷 버전 불일치 시에 바로 실패 및 연결 종료
        if(pkt.Version != ConfigManager.Config.packetVer)
        {
            S_EnterGame sEnterGamePkt = new S_EnterGame();
            sEnterGamePkt.Result = EResult.Failed;
            clientSession.Send(sEnterGamePkt);
            Console.WriteLine($"Wrong Packet Version: {pkt.Version}");
            clientSession.Disconnect();
            return;
        }
        else
        {
            // TODO: Web 요청이나 인증 처리 여기서 하기
            clientSession.IsVerified = true;
        }

        // 패킷에 있는 방 찾기, 없으면 실패 패킷 보내기
        GameRoomManager.Instance.FindRoom(roomId: 1, callback: (room) =>
        {
            if (room != null)
            {
                Console.WriteLine($"Found Room ID: {room.GetHashCode()}");

                room.HandleEnterGame(clientSession, pkt);
            }
            else
            {
                Console.WriteLine("Room not found.");

                // 실패 패킷 전달
                S_EnterGame pkt = new S_EnterGame();
                pkt.Result = EResult.Failed;

                clientSession.Send(pkt);
            }
        });

    }

    internal static void C_LeaveGameHandler(PacketSession session, IMessage packet)
    {
        C_LeaveGame pkt = packet as C_LeaveGame;
        ClientSession clientSession = session as ClientSession;

        clientSession.Disconnect();
    }
}
