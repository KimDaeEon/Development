using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace PacketGenerator
{
    class PacketFormat
    {
        public static string packetLogFormat =
                "Console.WriteLine(" 
                + "\"[Received] \""
                + " + packet.Descriptor.Name +"
                + " \" \" + "
                + " packet);";

        // 패킷 매니저 포맷
        // {0} 새로운 패킷 Register 부분
        // {1} 패킷 로그 적용 포맷
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

    Dictionary<ushort, Action<PacketSession, ArraySegment<byte>, ushort>> _makePacketMap = new Dictionary<ushort, Action<PacketSession, ArraySegment<byte>, ushort>>();
    Dictionary<ushort, Action<PacketSession, IMessage>> _handlerMap = new Dictionary<ushort, Action<PacketSession, IMessage>>();

    public Action<PacketSession, IMessage, ushort> CustomHandler {{ get; set; }}

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
        if (_makePacketMap.TryGetValue(packetId, out action))
        {{
            action.Invoke(session, buffer, packetId);
        }}
    }}

    void MakePacket<T>(PacketSession session, ArraySegment<byte> buffer, ushort packetId) where T: IMessage, new()
    {{
        // 패킷 조립
        T packet = new T();
        packet.MergeFrom(buffer.Array, buffer.Offset + 4, buffer.Count - 4);

        if (CustomHandler != null)
        {{
            CustomHandler.Invoke(session, packet, packetId);
        }}
        else
        {{
            Action<PacketSession, IMessage> action = null;
            if (_handlerMap.TryGetValue(packetId, out action))
            {{
                #if DEBUG && !UNITY_EDITOR
                {1}
                #endif
                action.Invoke(session, packet);
            }}
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
@"        _makePacketMap.Add((ushort)MsgId.{0}, MakePacket<{1}>);
        _handlerMap.Add((ushort)MsgId.{0}, PacketHandler.{1}Handler);
";

        
    }
}
