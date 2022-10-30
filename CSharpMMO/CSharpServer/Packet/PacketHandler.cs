using CSharpServer;
using Google.Protobuf.Protocol;
using Google.Protobuf;
using ServerCore;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using CSharpServer.Game;
using CSharpServer.DB;
using CSharpServer.Data;

class PacketHandler
{
    public static void C_HeartBeatHandler(PacketSession session, IMessage packet)
    {
        ClientSession clientSession = (ClientSession)session;
        clientSession.HandleHeartBeat();
    }

    public static void C_MoveHandler(PacketSession session, IMessage packet)
    {
        C_Move movePacket = (C_Move)packet;
        ClientSession clientSession = (ClientSession)session;

        Player player = null;
        Room room = null;
        if(Player.IsInValidSessionAndPlayerAndRoom(clientSession, ref player, ref room))
        {
            return;
        }

        room.Push(room.HandleMove, player, movePacket);
    }

    public static void C_SkillHandler(PacketSession session, IMessage packet)
    {
        C_Skill skillPacket = (C_Skill)packet;
        ClientSession clientSession = (ClientSession)session;

        Player player = null;
        Room room = null;
        if(Player.IsInValidSessionAndPlayerAndRoom(clientSession, ref player, ref room))
        {
            return;
        }

        room.Push(room.HandleSkill, player, skillPacket);
    }

    public static void C_LoginHandler(PacketSession session, IMessage packet)
    {
        if (ConfigManager.Config.isDummyClientLoginPossible)
        {
            C_Login loginPacket = (C_Login)packet;
            ClientSession clientSession = (ClientSession)session;

            clientSession.HandleLogin(loginPacket);
        }
        else
        {
            // TODO: 이 부분 로그도 파일로 찍고 로그 레벨 및 포맷을 설정하도록 하자.
            Console.WriteLine("Hacking Detected!");
            session.Disconnect();
        }
    }

    public static void C_LoginDummyHandler(PacketSession session, IMessage packet)
    {
        C_LoginDummy loginPacket = (C_LoginDummy)packet;
        ClientSession clientSession = (ClientSession)session;

        if (ConfigManager.Config.isDummyClientLoginPossible)
        {
            clientSession.HandleDummyClientLogin(loginPacket);
        }
        else
        {
            // TODO: 이 부분 로그도 파일로 찍고 로그 레벨 및 포맷을 설정하도록 하자.
            Console.WriteLine("Hacking Detected!");
            clientSession.Disconnect();
        }
    }

    public static void C_CreatePlayerHandler(PacketSession session, IMessage packet)
    {
        C_CreatePlayer createPlayerPacket = (C_CreatePlayer)packet;
        ClientSession clientSession = (ClientSession)session;

        clientSession.HandleCreatePlayer(createPlayerPacket);
    }

    public static void C_EnterGameHandler(PacketSession session, IMessage packet)
    {
        C_EnterGame enterGamePacket = (C_EnterGame)packet;
        ClientSession clientSession = (ClientSession)session;

        clientSession.HandleEnterGame(enterGamePacket);
    }

    public static void C_EquipItemHandler(PacketSession session, IMessage packet)
    {
        C_EquipItem equipItemPacket = (C_EquipItem)packet;
        ClientSession clientSession = (ClientSession)session;

        Player player = null;
        Room room = null;
        if (Player.IsInValidSessionAndPlayerAndRoom(clientSession, ref player, ref room))
        {
            return;
        }

        room.Push(room.HandleEquipItem, player, equipItemPacket);
    }
}
