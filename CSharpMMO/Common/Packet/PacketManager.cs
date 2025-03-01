using ServerCore;
using System;
using System.Collections.Generic;

class PacketManager
{
    static PacketManager _instance;

    #region Singleton
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

    Dictionary<ushort, Action<PacketSession, ArraySegment<byte>>> _onRecv = new Dictionary<ushort, Action<PacketSession, ArraySegment<byte>>>();
    Dictionary<ushort, Action<PacketSession, IPacket>> _handler = new Dictionary<ushort, Action<PacketSession, IPacket>>();
    public void Register()
    {
        _onRecv.Add((ushort)PacketID.PlayerInfoReq, MakePacket<PlayerInfoReq>);
        _handler.Add((ushort)PacketID.PlayerInfoReq, PacketHandler.PlayerInfoReqHandler);
        _onRecv.Add((ushort)PacketID.PlayerInfoReq2, MakePacket<PlayerInfoReq2>);
        _handler.Add((ushort)PacketID.PlayerInfoReq2, PacketHandler.PlayerInfoReq2Handler);

    }
    public void OnRecvPacket(PacketSession session, ArraySegment<byte> buffer)
    {
        ushort count = 0;

        ushort size = BitConverter.ToUInt16(buffer.Array, buffer.Offset);
        count += 2;

        ushort packetId = BitConverter.ToUInt16(buffer.Array, buffer.Offset + count);
        count += 2;

        Action<PacketSession, ArraySegment<byte>> action = null;
        if(_onRecv.TryGetValue(packetId, out action))
        {
            // 패킷 조립 및 처리 함수 실행
            action.Invoke(session, buffer);
        }
    }

    void MakePacket<T>(PacketSession session, ArraySegment<byte> buffer) where T: IPacket, new()
    {
        // 패킷 조립
        T packet = new T();
        packet.Read(buffer);

        // 패킷 처리
        Action<PacketSession, IPacket> action = null;
        if (_handler.TryGetValue(packet.Protocol, out action))
        {
            action.Invoke(session, packet);
        }
    }
}
