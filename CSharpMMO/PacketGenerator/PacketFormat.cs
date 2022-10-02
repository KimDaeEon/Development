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
@"using ServerCore;
using System;
using System.Collections.Generic;

class PacketManager
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

    Dictionary<ushort, Action<PacketSession, ArraySegment<byte>>> _onRecv = new Dictionary<ushort, Action<PacketSession, ArraySegment<byte>>>();
    Dictionary<ushort, Action<PacketSession, IPacket>> _handler = new Dictionary<ushort, Action<PacketSession, IPacket>>();
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

        Action<PacketSession, ArraySegment<byte>> action = null;
        if(_onRecv.TryGetValue(packetId, out action))
        {{
            // 패킷 조립 및 처리 함수 실행
            action.Invoke(session, buffer);
        }}
    }}

    void MakePacket<T>(PacketSession session, ArraySegment<byte> buffer) where T: IPacket, new()
    {{
        // 패킷 조립
        T packet = new T();
        packet.Read(buffer);

        // 패킷 처리
        Action<PacketSession, IPacket> action = null;
        if (_handler.TryGetValue(packet.Protocol, out action))
        {{
            action.Invoke(session, packet);
        }}
    }}
}}
";
        // {0} 패킷 이름
        public static string registeredHandlerFormat =
@"        _onRecv.Add((ushort)PacketID.{0}, MakePacket<{0}>);
        _handler.Add((ushort)PacketID.{0}, PacketHandler.{0}Handler);";

        // {0} 패킷 enum 정보
        // {1} 패킷 정보
        public static string fileFormat =
@"using ServerCore;
using System;
using System.Collections.Generic;
using System.Net;
using System.Text;
using System.Threading;

public enum PacketID
{{
    {0}
}}

interface IPacket
{{
	ushort Protocol {{ get; }} // PacketID인데, 같은 이름을 쓰면 enum 과 충돌로 에러가 난다.
	void Read(ArraySegment<byte> segment);
	ArraySegment<byte> Write();
}}

{1}
";
        // {0} 패킷 이름
        // {1} 패킷 번호
        public static string packetEnumFormat =
@"{0} = {1},";

        // {0} 패킷 이름 
        // {1} 멤버 변수들
        // {2} 멤버 변수 Read
        // {3} 멤버 변수 Write
        public static string packetFormat =
@" 
class {0} : IPacket
{{
    public ushort size;
    public ushort id;
    {1}

    public {0}()
    {{
        this.id = (ushort)PacketID.{0};
    }}

    public ushort Protocol
	{{
        get
        {{
			return (ushort)PacketID.{0};
		}}
    }}

    public void Read(ArraySegment<byte> seg)
    {{
        ushort readCount = 0;

        ReadOnlySpan<byte> span = new ReadOnlySpan<byte>(seg.Array, seg.Offset, seg.Count);

        //ushort size = BitConverter.ToUInt16(s.Array, s.Offset);
        readCount += sizeof(ushort);
        //ushort packetId = BitConverter.ToUInt16(s.Array, s.Offset + count);
        readCount += sizeof(ushort);

        {2}
    }}

    public ArraySegment<byte> Write()
    {{
        ArraySegment<byte> seg = SendBufferHelper.Open(4096);

        bool success = true;
        ushort writeCount = 0;

        Span<byte> span = new Span<byte>(seg.Array, seg.Offset, seg.Count);

        // 후에 입력할 size 를 위해서 2칸을 비워둔다.
        writeCount += sizeof(ushort);

        // TryWriteBytes 로 Span 을 받게 되어 있다.
        success &= BitConverter.TryWriteBytes(span.Slice(writeCount, span.Length - writeCount), this.id);
        writeCount += sizeof(ushort);

        {3}

        // 최종 패킷 크기 입력
        success &= BitConverter.TryWriteBytes(span, writeCount);

        if (success == false)
        {{
            return null;
        }}

        return SendBufferHelper.Close(writeCount);
    }}
}}

";

        // {0} 변수 타입
        // {1} 변수 이름
        public static string memberFormat = 
@"public {0} {1};";

        // {0} 변수 이름
        // {1} To~ 변수 형식(ToInt32 이런 것)
        // {2} 변수 형식
        public static string readFormat =
@"this.{0} = BitConverter.{1}(span.Slice(readCount, span.Length - readCount));
readCount += sizeof({2});";

        // {0} 변수 이름
        // {1} 변수 타입
        // sbyte 를 쓰는 경우를 대비해서 캐스팅을 넣어준다.
        public static string readByteFormat =
@"this.{0} = ({1})seg.Array[seg.Offset + readCount];
readCount += sizeof({1});";

        // {0} 변수 이름
        public static string readStringFormat =
@"ushort {0}Len = BitConverter.ToUInt16(span.Slice(readCount, span.Length - readCount));
readCount += sizeof(ushort);
this.{0} = Encoding.Unicode.GetString(span.Slice(readCount, {0}Len));
readCount += {0}Len;";

        // {0} 변수 이름
        // {1} 변수 타입
        public static string writeFormat =
@"
success &= BitConverter.TryWriteBytes(span.Slice(writeCount, span.Length - writeCount), this.{0});
writeCount += sizeof({1});";

        // {0} 변수 이름
        // {1} 변수 타입
        // write 할 때에 sbyte 를 byte 로 바꿔야 한다. sbyte 를 쓸 일은 거의 없겠지만..
        public static string writeByteFormat =
@"seg.Array[seg.Offset + writeCount] = (byte)this.{0};
writeCount += sizeof({1});";

        // {0} 변수 이름
        public static string writeStringFormat =
@"
ushort {0}Len = (ushort)Encoding.Unicode.GetBytes(this.{0}, 0, this.{0}.Length, seg.Array, seg.Offset + writeCount + sizeof(ushort));
success &= BitConverter.TryWriteBytes(span.Slice(writeCount, span.Length - writeCount), {0}Len);
writeCount += {0}Len;
writeCount += sizeof(ushort);";

        // {0} 리스트 이름 [대문자]
        // {1} 리스트 이름 [소문자]
        // {2} 멤버 변수들
        // {3} 멤버 변수 Read
        // {4} 멤버 변수 Write
        public static string memberListFormat =
@"
public struct {0}
{{
    {2}

    public void Read(ReadOnlySpan<byte> span, ref ushort readCount)
    {{
        {3}
    }}

    public bool Write(Span<byte> span, ref ushort writeCount)
    {{
        bool success = true;

        {4}

        return success;
    }}
}}
public List<{0}> {1}s = new List<{0}>();
";

        // {0} 리스트 이름 [대문자]
        // {1} 리스트 이름 [소문자]
        public static string readListFormat =
@"
this.{1}s.Clear();
ushort {1}Len = BitConverter.ToUInt16(span.Slice(readCount, span.Length - readCount));
readCount += sizeof(ushort);
for (int i = 0; i < {1}Len; i++)
{{
    {0} {1} = new {0}();
    {1}.Read(span, ref readCount);

    this.{1}s.Add({1});
}}
";
        // {0} 리스트 이름 [대문자]
        // {1} 리스트 이름 [소문자]
        public static string writeListFormat =
@"
success &= BitConverter.TryWriteBytes(span.Slice(writeCount, span.Length - writeCount), (ushort)this.{1}s.Count);
writeCount += sizeof(ushort);

foreach ({0} {1} in this.{1}s)
{{
    success &= {1}.Write(span, ref writeCount);
}}
";

        
    }
}
