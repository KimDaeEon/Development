using CSharpServer;
using Google.Protobuf.Protocol;
using Google.Protobuf;
using ServerCore;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using CSharpServer.Contents;

class PacketHandler
{
    public static void C_MoveHandler(PacketSession session, IMessage packet)
    {
        C_Move movePacket = packet as C_Move;
        ClientSession clientSession = session as ClientSession;

        Player player = null;
        Room room = null;
        if(Player.IsInValidSessionAndPlayerAndRoom(clientSession, ref player, ref room))
        {
            return;
        }

        room.Push(room.HandleMove, player, movePacket);
    }

    internal static void C_SkillHandler(PacketSession session, IMessage packet)
    {
        C_Skill skillPacket = packet as C_Skill;
        ClientSession clientSession = session as ClientSession;

        Player player = null;
        Room room = null;
        if(Player.IsInValidSessionAndPlayerAndRoom(clientSession, ref player, ref room))
        {
            return;
        }

        room.Push(room.HandleSkill, player, skillPacket);
    }
}
