using ServerCore;
using System;
using System.Collections.Generic;
using System.Net;
using System.Text;
using System.Threading;

public enum PacketID
{
    C_Chat = 1,
	S_Chat = 2,
	
}

interface IPacket
{
	ushort Protocol { get; } // PacketID인데, 같은 이름을 쓰면 enum 과 충돌로 에러가 난다.
	void Read(ArraySegment<byte> segment);
	ArraySegment<byte> Write();
}

 
class C_Chat : IPacket
{
    public ushort size;
    public ushort id;
    public string chatMessage;

    public C_Chat()
    {
        this.id = (ushort)PacketID.C_Chat;
    }

    public ushort Protocol
	{
        get
        {
			return (ushort)PacketID.C_Chat;
		}
    }

    public void Read(ArraySegment<byte> seg)
    {
        ushort readCount = 0;

        ReadOnlySpan<byte> span = new ReadOnlySpan<byte>(seg.Array, seg.Offset, seg.Count);

        //ushort size = BitConverter.ToUInt16(s.Array, s.Offset);
        readCount += sizeof(ushort);
        //ushort packetId = BitConverter.ToUInt16(s.Array, s.Offset + count);
        readCount += sizeof(ushort);

        ushort chatMessageLen = BitConverter.ToUInt16(span.Slice(readCount, span.Length - readCount));
		readCount += sizeof(ushort);
		this.chatMessage = Encoding.Unicode.GetString(span.Slice(readCount, chatMessageLen));
		readCount += chatMessageLen;
    }

    public ArraySegment<byte> Write()
    {
        ArraySegment<byte> seg = SendBufferHelper.Open(4096);

        bool success = true;
        ushort writeCount = 0;

        Span<byte> span = new Span<byte>(seg.Array, seg.Offset, seg.Count);

        // 후에 입력할 size 를 위해서 2칸을 비워둔다.
        writeCount += sizeof(ushort);

        // TryWriteBytes 로 Span 을 받게 되어 있다.
        success &= BitConverter.TryWriteBytes(span.Slice(writeCount, span.Length - writeCount), this.id);
        writeCount += sizeof(ushort);

        
		ushort chatMessageLen = (ushort)Encoding.Unicode.GetBytes(this.chatMessage, 0, this.chatMessage.Length, seg.Array, seg.Offset + writeCount + sizeof(ushort));
		success &= BitConverter.TryWriteBytes(span.Slice(writeCount, span.Length - writeCount), chatMessageLen);
		writeCount += chatMessageLen;
		writeCount += sizeof(ushort);

        // 최종 패킷 크기 입력
        success &= BitConverter.TryWriteBytes(span, writeCount);

        if (success == false)
        {
            return null;
        }

        return SendBufferHelper.Close(writeCount);
    }
}

 
class S_Chat : IPacket
{
    public ushort size;
    public ushort id;
    public int playerId;
	public string chatMessage;

    public S_Chat()
    {
        this.id = (ushort)PacketID.S_Chat;
    }

    public ushort Protocol
	{
        get
        {
			return (ushort)PacketID.S_Chat;
		}
    }

    public void Read(ArraySegment<byte> seg)
    {
        ushort readCount = 0;

        ReadOnlySpan<byte> span = new ReadOnlySpan<byte>(seg.Array, seg.Offset, seg.Count);

        //ushort size = BitConverter.ToUInt16(s.Array, s.Offset);
        readCount += sizeof(ushort);
        //ushort packetId = BitConverter.ToUInt16(s.Array, s.Offset + count);
        readCount += sizeof(ushort);

        this.playerId = BitConverter.ToInt32(span.Slice(readCount, span.Length - readCount));
		readCount += sizeof(int);
		ushort chatMessageLen = BitConverter.ToUInt16(span.Slice(readCount, span.Length - readCount));
		readCount += sizeof(ushort);
		this.chatMessage = Encoding.Unicode.GetString(span.Slice(readCount, chatMessageLen));
		readCount += chatMessageLen;
    }

    public ArraySegment<byte> Write()
    {
        ArraySegment<byte> seg = SendBufferHelper.Open(4096);

        bool success = true;
        ushort writeCount = 0;

        Span<byte> span = new Span<byte>(seg.Array, seg.Offset, seg.Count);

        // 후에 입력할 size 를 위해서 2칸을 비워둔다.
        writeCount += sizeof(ushort);

        // TryWriteBytes 로 Span 을 받게 되어 있다.
        success &= BitConverter.TryWriteBytes(span.Slice(writeCount, span.Length - writeCount), this.id);
        writeCount += sizeof(ushort);

        
		success &= BitConverter.TryWriteBytes(span.Slice(writeCount, span.Length - writeCount), this.playerId);
		writeCount += sizeof(int);
		
		ushort chatMessageLen = (ushort)Encoding.Unicode.GetBytes(this.chatMessage, 0, this.chatMessage.Length, seg.Array, seg.Offset + writeCount + sizeof(ushort));
		success &= BitConverter.TryWriteBytes(span.Slice(writeCount, span.Length - writeCount), chatMessageLen);
		writeCount += chatMessageLen;
		writeCount += sizeof(ushort);

        // 최종 패킷 크기 입력
        success &= BitConverter.TryWriteBytes(span, writeCount);

        if (success == false)
        {
            return null;
        }

        return SendBufferHelper.Close(writeCount);
    }
}


