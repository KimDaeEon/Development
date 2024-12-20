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
        _makePacketMap.Add((ushort)MsgId.CHeartBeat, MakePacket<C_HeartBeat>);
        _handlerMap.Add((ushort)MsgId.CHeartBeat, PacketHandler.C_HeartBeatHandler);
        _makePacketMap.Add((ushort)MsgId.CLoginDummy, MakePacket<C_LoginDummy>);
        _handlerMap.Add((ushort)MsgId.CLoginDummy, PacketHandler.C_LoginDummyHandler);
        _makePacketMap.Add((ushort)MsgId.CLogin, MakePacket<C_Login>);
        _handlerMap.Add((ushort)MsgId.CLogin, PacketHandler.C_LoginHandler);
        _makePacketMap.Add((ushort)MsgId.CEnterGame, MakePacket<C_EnterGame>);
        _handlerMap.Add((ushort)MsgId.CEnterGame, PacketHandler.C_EnterGameHandler);
        _makePacketMap.Add((ushort)MsgId.CCreatePlayer, MakePacket<C_CreatePlayer>);
        _handlerMap.Add((ushort)MsgId.CCreatePlayer, PacketHandler.C_CreatePlayerHandler);
        _makePacketMap.Add((ushort)MsgId.CMove, MakePacket<C_Move>);
        _handlerMap.Add((ushort)MsgId.CMove, PacketHandler.C_MoveHandler);
        _makePacketMap.Add((ushort)MsgId.CSkill, MakePacket<C_Skill>);
        _handlerMap.Add((ushort)MsgId.CSkill, PacketHandler.C_SkillHandler);
        _makePacketMap.Add((ushort)MsgId.CEquipItem, MakePacket<C_EquipItem>);
        _handlerMap.Add((ushort)MsgId.CEquipItem, PacketHandler.C_EquipItemHandler);

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
