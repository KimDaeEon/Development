using Google.Protobuf;
using Google.Protobuf.Protocol;
using ServerCore;
using System;
using System.Collections.Generic;

public class PacketManager
{
    #region Singleton
    static PacketManager _instance = new PacketManager();
    public static PacketManager Instance
    {
        get
        {
            if(_instance == null)
            {
                _instance = new PacketManager();
            }
            return _instance;
        }
    }
    #endregion

    PacketManager()
    {
        Register();
    }

    Dictionary<ushort, Action<PacketSession, ArraySegment<byte>, ushort>> _makePacketMap = new Dictionary<ushort, Action<PacketSession, ArraySegment<byte>, ushort>>();
    Dictionary<ushort, Action<PacketSession, IMessage>> _handlerMap = new Dictionary<ushort, Action<PacketSession, IMessage>>();

    public Action<PacketSession, IMessage, ushort> CustomHandler { get; set; }

    public void Register()
    {
        _makePacketMap.Add((ushort)MsgId.SEnterGame, MakePacket<S_EnterGame>);
        _handlerMap.Add((ushort)MsgId.SEnterGame, PacketHandler.S_EnterGameHandler);
        _makePacketMap.Add((ushort)MsgId.SConnected, MakePacket<S_Connected>);
        _handlerMap.Add((ushort)MsgId.SConnected, PacketHandler.S_ConnectedHandler);
        _makePacketMap.Add((ushort)MsgId.SLogin, MakePacket<S_Login>);
        _handlerMap.Add((ushort)MsgId.SLogin, PacketHandler.S_LoginHandler);
        _makePacketMap.Add((ushort)MsgId.SCreatePlayer, MakePacket<S_CreatePlayer>);
        _handlerMap.Add((ushort)MsgId.SCreatePlayer, PacketHandler.S_CreatePlayerHandler);
        _makePacketMap.Add((ushort)MsgId.SLeaveGame, MakePacket<S_LeaveGame>);
        _handlerMap.Add((ushort)MsgId.SLeaveGame, PacketHandler.S_LeaveGameHandler);
        _makePacketMap.Add((ushort)MsgId.SSpawn, MakePacket<S_Spawn>);
        _handlerMap.Add((ushort)MsgId.SSpawn, PacketHandler.S_SpawnHandler);
        _makePacketMap.Add((ushort)MsgId.SDespawn, MakePacket<S_Despawn>);
        _handlerMap.Add((ushort)MsgId.SDespawn, PacketHandler.S_DespawnHandler);
        _makePacketMap.Add((ushort)MsgId.SMove, MakePacket<S_Move>);
        _handlerMap.Add((ushort)MsgId.SMove, PacketHandler.S_MoveHandler);
        _makePacketMap.Add((ushort)MsgId.SSkill, MakePacket<S_Skill>);
        _handlerMap.Add((ushort)MsgId.SSkill, PacketHandler.S_SkillHandler);
        _makePacketMap.Add((ushort)MsgId.SChangeHp, MakePacket<S_ChangeHp>);
        _handlerMap.Add((ushort)MsgId.SChangeHp, PacketHandler.S_ChangeHpHandler);
        _makePacketMap.Add((ushort)MsgId.SDead, MakePacket<S_Dead>);
        _handlerMap.Add((ushort)MsgId.SDead, PacketHandler.S_DeadHandler);

    }
    public void OnRecvPacket(PacketSession session, ArraySegment<byte> buffer)
    {
        ushort count = 0;

        ushort size = BitConverter.ToUInt16(buffer.Array, buffer.Offset);
        count += 2;

        ushort packetId = BitConverter.ToUInt16(buffer.Array, buffer.Offset + count);
        count += 2;
        
        Action<PacketSession, ArraySegment<byte>, ushort> action = null;
        if (_makePacketMap.TryGetValue(packetId, out action))
        {
            action.Invoke(session, buffer, packetId);
        }
    }

    void MakePacket<T>(PacketSession session, ArraySegment<byte> buffer, ushort packetId) where T: IMessage, new()
    {
        // 패킷 조립
        T packet = new T();
        packet.MergeFrom(buffer.Array, buffer.Offset + 4, buffer.Count - 4);

        if (CustomHandler != null)
        {
            CustomHandler.Invoke(session, packet, packetId);
        }
        else
        {
            Action<PacketSession, IMessage> action = null;
            if (_handlerMap.TryGetValue(packetId, out action))
            {
                #if DEBUG && !UNITY_EDITOR
                Console.WriteLine("[Received] " + packet.Descriptor.Name + " " +  packet);
                #endif
                action.Invoke(session, packet);
            }
        }
    }

    public Action<PacketSession, IMessage> GetPacketHandler(ushort packetId)
    {
        Action<PacketSession, IMessage> action = null;
        if (_handlerMap.TryGetValue(packetId, out action))
        {
            return action;
        }
        
        return null;
    }
}
