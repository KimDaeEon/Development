using Google.Protobuf;
using Google.Protobuf.Protocol;
using ServerCore;
using System;
using System.Reflection;
using UnityEngine;

class PacketHandler
{
    public static void S_EnterGameHandler(PacketSession session, IMessage packet)
    {
        S_EnterGame enterGamePacket = packet as S_EnterGame;

        Managers.Object.Add(enterGamePacket.Player, isMyPlayer: true);
    }

    public static void S_LeaveGameHandler(PacketSession session, IMessage packet)
    {
        Managers.Object.Clear();
    }

    public static void S_SpawnHandler(PacketSession session, IMessage packet)
    {
        S_Spawn spawnPacket = packet as S_Spawn;

        // 해당 패킷에 자신에 대한 정보는 있으면 안된다. 아래 로직에서 문제 생김.
        foreach(ActorInfo player in spawnPacket.Actors)
        {
            Managers.Object.Add(player, isMyPlayer: false);
        }
    }

    public static void S_DespawnHandler(PacketSession session, IMessage packet)
    {
        S_Despawn despawnPacket = packet as S_Despawn;

        foreach( int id in despawnPacket.ActorIds)
        {
            Managers.Object.Remove(id);
        }
    }

    public static void S_MoveHandler(PacketSession session, IMessage packet)
    {
        S_Move movePacket = packet as S_Move;

        GameObject obj = Managers.Object.FindById(movePacket.ActorId);
        
        if(obj == null)
        {
            return;
        }

        BaseController ac = obj.GetComponent<BaseController>();
        if(ac == null)
        {
            Debug.Log($"{obj.name} has sent a move packet, but has no actorController.");
            return;
        }

        // 이 부분 잘 보자. State 를 모두 다 PacketHandler 에서 정하고 있는 것이다.
        // 이로 인해 캐릭터가 멈추고 뛰고 하는 상태가 동기화되어서 보인다.
        ac.PosInfo = movePacket.PosInfo;
    }

    public static void S_SkillHandler(PacketSession session, IMessage packet)
    {
        S_Skill skillPacket = packet as S_Skill;

        GameObject obj = Managers.Object.FindById(skillPacket.ActorId);
        if(obj == null)
        {
            return;
        }

        ActorController ac = obj.GetComponent<ActorController>();
        if(ac != null)
        {
            ac.UseSkill(skillPacket.Info.SkillId);
        }
    }

    public static void S_ChangeHpHandler(PacketSession session, IMessage packet)
    {
        S_ChangeHp changeHpPacket = packet as S_ChangeHp;

        GameObject obj = Managers.Object.FindById(changeHpPacket.ActorId);
        if (obj == null)
        {
            return;
        }

        ActorController ac = obj.GetComponent<ActorController>();
        if (ac != null)
        {
            ac.Hp = changeHpPacket.Hp;
        }
    }

    public static void S_DeadHandler(PacketSession session, IMessage packet)
    {
        S_Dead changeHpPacket = packet as S_Dead;

        GameObject obj = Managers.Object.FindById(changeHpPacket.ActorId);
        if (obj == null)
        {
            return;
        }

        ActorController ac = obj.GetComponent<ActorController>();
        if (ac != null)
        {
            ac.OnDead();
        }
    }
}

