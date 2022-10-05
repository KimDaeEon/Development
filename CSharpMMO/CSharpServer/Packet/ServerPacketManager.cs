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
    Dictionary<ushort, Func<PacketSession, ArraySegment<byte>, IPacket>> _funcMap = new Dictionary<ushort, Func<PacketSession, ArraySegment<byte>, IPacket>>();
    Dictionary<ushort, Action<PacketSession, IPacket>> _handlerMap = new Dictionary<ushort, Action<PacketSession, IPacket>>();

    public void Register()
    {
        _funcMap.Add((ushort)PacketID.C_LeaveGame, MakePacket<C_LeaveGame>);
        _handlerMap.Add((ushort)PacketID.C_LeaveGame, PacketHandler.C_LeaveGameHandler);
        _funcMap.Add((ushort)PacketID.C_Move, MakePacket<C_Move>);
        _handlerMap.Add((ushort)PacketID.C_Move, PacketHandler.C_MoveHandler);

    }
    public void OnRecvPacket(PacketSession session, ArraySegment<byte> buffer, Action<PacketSession, IPacket> onRecvCallback = null)
    {
        ushort count = 0;

        ushort size = BitConverter.ToUInt16(buffer.Array, buffer.Offset);
        count += 2;

        ushort packetId = BitConverter.ToUInt16(buffer.Array, buffer.Offset + count);
        count += 2;

        Func<PacketSession, ArraySegment<byte>, IPacket> func = null;
        if(_funcMap.TryGetValue(packetId, out func))
        {
            // 패킷 조립 함수 실행
            IPacket packet = func.Invoke(session, buffer);

            // 정해진 패킷 핸들러 말고 다른 콜백함수를 실행하고 싶을 때에 onRecvCallback 을 사용한다.
            // 여기서는 유니티가 메인 스레드에서만 GameObject 관련 상호작용이 되기에, 패킷을 메인스레드에서 처리하기 위한 함수를 넣었다.
            if(onRecvCallback != null)
            {
                onRecvCallback.Invoke(session, packet);
            }
            else
            {
                HandlePacket(session, packet);
            }
        }
    }

    T MakePacket<T>(PacketSession session, ArraySegment<byte> buffer) where T: IPacket, new()
    {
        // 패킷 조립
        T packet = new T();
        packet.Read(buffer);

        return packet;
    }

    public void HandlePacket(PacketSession session, IPacket packet)
    {
        // 패킷 처리
        Action<PacketSession, IPacket> action = null;
        if (_handlerMap.TryGetValue(packet.Protocol, out action))
        {
            action.Invoke(session, packet);
        }
    }
}
