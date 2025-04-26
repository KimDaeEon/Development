using Google.Protobuf;
using Google.Protobuf.Protocol;
using ServerCore;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.Net;
using System;
using System.Linq;
using static CustomEnum;

class PacketHandler
{
    ///////////////////////////////////// GameServer - Client /////////////////////////////////////
    public static void S_ConnectedHandler(PacketSession session, IMessage packet)
    {
        Debug.Log("S_Connected");
    }
    public static void S_EnterGameHandler(PacketSession session, IMessage packet)
    {
        Debug.Log("S_EnterGame");

        S_EnterGame enterGamePacket = packet as S_EnterGame;
        Managers.Object.SpawnMyCharacter(enterGamePacket.MyCharInfo);
    }

    public static void S_LeaveGameHandler(PacketSession session, IMessage packet)
    {
        Debug.Log("S_Leave");
        Managers.Object.Clear();
    }

    public static void S_DespawnHandler(PacketSession session, IMessage packet)
    {
        Debug.Log("S_DespawnHandler");

        S_Despawn despawnPacket = packet as S_Despawn;
        Managers.Object.Despawn(despawnPacket.GameIds.ToList());
    }

    internal static void S_SpawnHandler(PacketSession session, IMessage packet)
    {
        Debug.Log("S_SpawnHandler");

        S_Spawn spawnPacket = packet as S_Spawn;

        foreach (EntityInfo entityInfo in spawnPacket.EntityInfos)
        {
            Managers.Object.Spawn(entityInfo);

        }
    }

    internal static void S_MoveHandler(PacketSession session, IMessage packet)
    {
        S_Move movePkt = packet as S_Move;

        if (movePkt.Result == EResult.Success)
        {
            GameObject go = Managers.Object.FindByGameId(movePkt.EntityInfo.GameId);
            if (go != null)
            {
                Entity entity = go.GetComponent<Entity>();
                entity.EntityInfo = movePkt.EntityInfo;
                Debug.Log($"S_Move = {movePkt}");
            }
            else
            {
                Debug.LogError("Something is wrong with move packet");
            }

        }
    }
}
