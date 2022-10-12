using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace PacketGenerator
{
    class PacketFormat
    {
        // 패킷 매니저 포맷
        // {0} 새로운 패킷 Register 부분
        public static string managerFormat =
@"using Google.Protobuf;
using Google.Protobuf.Protocol;
using ServerCore;
using System;
using System.Collections.Generic;

public class PacketManager
{{
    #region Singleton
    static PacketManager _instance = new PacketManager();
    public static PacketManager Instance
    {{
        get
        {{
            if(_instance == null)
            {{
                _instance = new PacketManager();
            }}
            return _instance;
        }}
    }}
    #endregion

    PacketManager()
    {{
        Register();
    }}
    Dictionary<ushort, Action<PacketSession, ArraySegment<byte>, ushort>> _funcMap = new Dictionary<ushort, Action<PacketSession, ArraySegment<byte>, ushort>>();
    Dictionary<ushort, Action<PacketSession, IMessage>> _handlerMap = new Dictionary<ushort, Action<PacketSession, IMessage>>();

    public void Register()
    {{
{0}
    }}
    public void OnRecvPacket(PacketSession session, ArraySegment<byte> buffer)
    {{
        ushort count = 0;

        ushort size = BitConverter.ToUInt16(buffer.Array, buffer.Offset);
        count += 2;

        ushort packetId = BitConverter.ToUInt16(buffer.Array, buffer.Offset + count);
        count += 2;
        
        Action<PacketSession, ArraySegment<byte>, ushort> action = null;
        if (_funcMap.TryGetValue(packetId, out action))
        {{
            action.Invoke(session, buffer, packetId);
        }}
    }}

    void MakePacket<T>(PacketSession session, ArraySegment<byte> buffer, ushort packetId) where T: IMessage, new()
    {{
        // 패킷 조립
        T packet = new T();
        packet.MergeFrom(buffer.Array, buffer.Offset + 4, buffer.Count - 4);
        Action<PacketSession, IMessage> action = null;
        if (_handlerMap.TryGetValue(packetId, out action))
        {{
            action.Invoke(session, packet);
        }}
    }}

    public Action<PacketSession, IMessage> GetPacketHandler(ushort packetId)
    {{
        Action<PacketSession, IMessage> action = null;
        if (_handlerMap.TryGetValue(packetId, out action))
        {{
            return action;
        }}
        
        return null;
    }}
}}
";
        // {0} MsgId
        // {1} 패킷 이름
        public static string registeredHandlerFormat =
@"        _funcMap.Add((ushort)MsgId.{0}, MakePacket<{1}>);
        _handlerMap.Add((ushort)MsgId.{0}, PacketHandler.{1}Handler);
";

        
    }
}
