using ServerCore;
using System;
using System.Collections.Generic;
using System.Net;
using System.Text;
using System.Threading;

public enum PacketID
{
    S_PlayerInfoReq = 1,
	
}

interface IPacket
{
	ushort Protocol { get; } // PacketID인데, 같은 이름을 쓰면 enum 과 충돌로 에러가 난다.
	void Read(ArraySegment<byte> segment);
	ArraySegment<byte> Write();
}

 
class S_PlayerInfoReq : IPacket
{
    public ushort size;
    public ushort id;
    public long playerId;
	public string name;
	
	public struct SkillInfo
	{
	    public int id;
		public short level;
		public float duration;
	
	    public void Read(ReadOnlySpan<byte> span, ref ushort readCount)
	    {
	        this.id = BitConverter.ToInt32(span.Slice(readCount, span.Length - readCount));
			readCount += sizeof(int);
			this.level = BitConverter.ToInt16(span.Slice(readCount, span.Length - readCount));
			readCount += sizeof(short);
			this.duration = BitConverter.ToSingle(span.Slice(readCount, span.Length - readCount));
			readCount += sizeof(float);
	    }
	
	    public bool Write(Span<byte> span, ref ushort writeCount)
	    {
	        bool success = true;
	
	        
			success &= BitConverter.TryWriteBytes(span.Slice(writeCount, span.Length - writeCount), this.id);
			writeCount += sizeof(int);
			
			success &= BitConverter.TryWriteBytes(span.Slice(writeCount, span.Length - writeCount), this.level);
			writeCount += sizeof(short);
			
			success &= BitConverter.TryWriteBytes(span.Slice(writeCount, span.Length - writeCount), this.duration);
			writeCount += sizeof(float);
	
	        return success;
	    }
	}
	public List<SkillInfo> skillInfos = new List<SkillInfo>();
	

    public S_PlayerInfoReq()
    {
        this.id = (ushort)PacketID.S_PlayerInfoReq;
    }

    public ushort Protocol
	{
        get
        {
			return (ushort)PacketID.S_PlayerInfoReq;
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

        this.playerId = BitConverter.ToInt64(span.Slice(readCount, span.Length - readCount));
		readCount += sizeof(long);
		ushort nameLen = BitConverter.ToUInt16(span.Slice(readCount, span.Length - readCount));
		readCount += sizeof(ushort);
		this.name = Encoding.Unicode.GetString(span.Slice(readCount, nameLen));
		readCount += nameLen;
		
		this.skillInfos.Clear();
		ushort skillInfoLen = BitConverter.ToUInt16(span.Slice(readCount, span.Length - readCount));
		readCount += sizeof(ushort);
		for (int i = 0; i < skillInfoLen; i++)
		{
		    SkillInfo skillInfo = new SkillInfo();
		    skillInfo.Read(span, ref readCount);
		
		    this.skillInfos.Add(skillInfo);
		}
		
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
		writeCount += sizeof(long);
		
		ushort nameLen = (ushort)Encoding.Unicode.GetBytes(this.name, 0, this.name.Length, seg.Array, seg.Offset + writeCount + sizeof(ushort));
		success &= BitConverter.TryWriteBytes(span.Slice(writeCount, span.Length - writeCount), nameLen);
		writeCount += nameLen;
		writeCount += sizeof(ushort);
		
		success &= BitConverter.TryWriteBytes(span.Slice(writeCount, span.Length - writeCount), (ushort)this.skillInfos.Count);
		writeCount += sizeof(ushort);
		
		foreach (SkillInfo skillInfo in this.skillInfos)
		{
		    success &= skillInfo.Write(span, ref writeCount);
		}
		

        // 최종 패킷 크기 입력
        success &= BitConverter.TryWriteBytes(span, writeCount);

        if (success == false)
        {
            return null;
        }

        return SendBufferHelper.Close(writeCount);
    }
}


