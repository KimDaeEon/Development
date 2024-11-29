using ServerCore;
using System;
using System.Collections.Generic;
using System.Net;
using System.Text;
using System.Threading;

public enum PacketID
{
    S_BroadcastEnterGame = 1,
	C_LeaveGame = 2,
	S_BroadcastLeaveGame = 3,
	S_PlayerList = 4,
	C_Move = 5,
	S_BroadcastMove = 6,
	
}

public interface IPacket
{
	ushort Protocol { get; } // PacketID인데, 같은 이름을 쓰면 enum 과 충돌로 에러가 난다.
	void Read(ArraySegment<byte> segment);
	ArraySegment<byte> Write();
}

 
public class S_BroadcastEnterGame : IPacket
{
    public ushort size;
    public ushort id;
    public int playerId;
	public float posX;
	public float posY;
	public float posZ;

    public S_BroadcastEnterGame()
    {
        this.id = (ushort)PacketID.S_BroadcastEnterGame;
    }

    public ushort Protocol
	{
        get
        {
			return (ushort)PacketID.S_BroadcastEnterGame;
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
		this.posX = BitConverter.ToSingle(span.Slice(readCount, span.Length - readCount));
		readCount += sizeof(float);
		this.posY = BitConverter.ToSingle(span.Slice(readCount, span.Length - readCount));
		readCount += sizeof(float);
		this.posZ = BitConverter.ToSingle(span.Slice(readCount, span.Length - readCount));
		readCount += sizeof(float);
    }

    public ArraySegment<byte> Write()
    {
        ArraySegment<byte> seg = SendBufferHelper.Open(65535);

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
		
		success &= BitConverter.TryWriteBytes(span.Slice(writeCount, span.Length - writeCount), this.posX);
		writeCount += sizeof(float);
		
		success &= BitConverter.TryWriteBytes(span.Slice(writeCount, span.Length - writeCount), this.posY);
		writeCount += sizeof(float);
		
		success &= BitConverter.TryWriteBytes(span.Slice(writeCount, span.Length - writeCount), this.posZ);
		writeCount += sizeof(float);

        // 최종 패킷 크기 입력
        success &= BitConverter.TryWriteBytes(span, writeCount);

        if (success == false)
        {
            return null;
        }

        return SendBufferHelper.Close(writeCount);
    }
}

 
public class C_LeaveGame : IPacket
{
    public ushort size;
    public ushort id;
    

    public C_LeaveGame()
    {
        this.id = (ushort)PacketID.C_LeaveGame;
    }

    public ushort Protocol
	{
        get
        {
			return (ushort)PacketID.C_LeaveGame;
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

        
    }

    public ArraySegment<byte> Write()
    {
        ArraySegment<byte> seg = SendBufferHelper.Open(65535);

        bool success = true;
        ushort writeCount = 0;

        Span<byte> span = new Span<byte>(seg.Array, seg.Offset, seg.Count);

        // 후에 입력할 size 를 위해서 2칸을 비워둔다.
        writeCount += sizeof(ushort);

        // TryWriteBytes 로 Span 을 받게 되어 있다.
        success &= BitConverter.TryWriteBytes(span.Slice(writeCount, span.Length - writeCount), this.id);
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

 
public class S_BroadcastLeaveGame : IPacket
{
    public ushort size;
    public ushort id;
    public int playerId;

    public S_BroadcastLeaveGame()
    {
        this.id = (ushort)PacketID.S_BroadcastLeaveGame;
    }

    public ushort Protocol
	{
        get
        {
			return (ushort)PacketID.S_BroadcastLeaveGame;
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
    }

    public ArraySegment<byte> Write()
    {
        ArraySegment<byte> seg = SendBufferHelper.Open(65535);

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

        // 최종 패킷 크기 입력
        success &= BitConverter.TryWriteBytes(span, writeCount);

        if (success == false)
        {
            return null;
        }

        return SendBufferHelper.Close(writeCount);
    }
}

 
public class S_PlayerList : IPacket
{
    public ushort size;
    public ushort id;
    
	public struct Player
	{
	    public bool isSelf;
		public int playerId;
		public float posX;
		public float posY;
		public float posZ;
	
	    public void Read(ReadOnlySpan<byte> span, ref ushort readCount)
	    {
	        this.isSelf = BitConverter.ToBoolean(span.Slice(readCount, span.Length - readCount));
			readCount += sizeof(bool);
			this.playerId = BitConverter.ToInt32(span.Slice(readCount, span.Length - readCount));
			readCount += sizeof(int);
			this.posX = BitConverter.ToSingle(span.Slice(readCount, span.Length - readCount));
			readCount += sizeof(float);
			this.posY = BitConverter.ToSingle(span.Slice(readCount, span.Length - readCount));
			readCount += sizeof(float);
			this.posZ = BitConverter.ToSingle(span.Slice(readCount, span.Length - readCount));
			readCount += sizeof(float);
	    }
	
	    public bool Write(Span<byte> span, ref ushort writeCount)
	    {
	        bool success = true;
	
	        
			success &= BitConverter.TryWriteBytes(span.Slice(writeCount, span.Length - writeCount), this.isSelf);
			writeCount += sizeof(bool);
			
			success &= BitConverter.TryWriteBytes(span.Slice(writeCount, span.Length - writeCount), this.playerId);
			writeCount += sizeof(int);
			
			success &= BitConverter.TryWriteBytes(span.Slice(writeCount, span.Length - writeCount), this.posX);
			writeCount += sizeof(float);
			
			success &= BitConverter.TryWriteBytes(span.Slice(writeCount, span.Length - writeCount), this.posY);
			writeCount += sizeof(float);
			
			success &= BitConverter.TryWriteBytes(span.Slice(writeCount, span.Length - writeCount), this.posZ);
			writeCount += sizeof(float);
	
	        return success;
	    }
	}
	public List<Player> players = new List<Player>();
	

    public S_PlayerList()
    {
        this.id = (ushort)PacketID.S_PlayerList;
    }

    public ushort Protocol
	{
        get
        {
			return (ushort)PacketID.S_PlayerList;
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

        
		this.players.Clear();
		ushort playerLen = BitConverter.ToUInt16(span.Slice(readCount, span.Length - readCount));
		readCount += sizeof(ushort);
		for (int i = 0; i < playerLen; i++)
		{
		    Player player = new Player();
		    player.Read(span, ref readCount);
		
		    this.players.Add(player);
		}
		
    }

    public ArraySegment<byte> Write()
    {
        ArraySegment<byte> seg = SendBufferHelper.Open(65535);

        bool success = true;
        ushort writeCount = 0;

        Span<byte> span = new Span<byte>(seg.Array, seg.Offset, seg.Count);

        // 후에 입력할 size 를 위해서 2칸을 비워둔다.
        writeCount += sizeof(ushort);

        // TryWriteBytes 로 Span 을 받게 되어 있다.
        success &= BitConverter.TryWriteBytes(span.Slice(writeCount, span.Length - writeCount), this.id);
        writeCount += sizeof(ushort);

        
		success &= BitConverter.TryWriteBytes(span.Slice(writeCount, span.Length - writeCount), (ushort)this.players.Count);
		writeCount += sizeof(ushort);
		
		foreach (Player player in this.players)
		{
		    success &= player.Write(span, ref writeCount);
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

 
public class C_Move : IPacket
{
    public ushort size;
    public ushort id;
    public float posX;
	public float posY;
	public float posZ;

    public C_Move()
    {
        this.id = (ushort)PacketID.C_Move;
    }

    public ushort Protocol
	{
        get
        {
			return (ushort)PacketID.C_Move;
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

        this.posX = BitConverter.ToSingle(span.Slice(readCount, span.Length - readCount));
		readCount += sizeof(float);
		this.posY = BitConverter.ToSingle(span.Slice(readCount, span.Length - readCount));
		readCount += sizeof(float);
		this.posZ = BitConverter.ToSingle(span.Slice(readCount, span.Length - readCount));
		readCount += sizeof(float);
    }

    public ArraySegment<byte> Write()
    {
        ArraySegment<byte> seg = SendBufferHelper.Open(65535);

        bool success = true;
        ushort writeCount = 0;

        Span<byte> span = new Span<byte>(seg.Array, seg.Offset, seg.Count);

        // 후에 입력할 size 를 위해서 2칸을 비워둔다.
        writeCount += sizeof(ushort);

        // TryWriteBytes 로 Span 을 받게 되어 있다.
        success &= BitConverter.TryWriteBytes(span.Slice(writeCount, span.Length - writeCount), this.id);
        writeCount += sizeof(ushort);

        
		success &= BitConverter.TryWriteBytes(span.Slice(writeCount, span.Length - writeCount), this.posX);
		writeCount += sizeof(float);
		
		success &= BitConverter.TryWriteBytes(span.Slice(writeCount, span.Length - writeCount), this.posY);
		writeCount += sizeof(float);
		
		success &= BitConverter.TryWriteBytes(span.Slice(writeCount, span.Length - writeCount), this.posZ);
		writeCount += sizeof(float);

        // 최종 패킷 크기 입력
        success &= BitConverter.TryWriteBytes(span, writeCount);

        if (success == false)
        {
            return null;
        }

        return SendBufferHelper.Close(writeCount);
    }
}

 
public class S_BroadcastMove : IPacket
{
    public ushort size;
    public ushort id;
    public int playerId;
	public float posX;
	public float posY;
	public float posZ;

    public S_BroadcastMove()
    {
        this.id = (ushort)PacketID.S_BroadcastMove;
    }

    public ushort Protocol
	{
        get
        {
			return (ushort)PacketID.S_BroadcastMove;
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
		this.posX = BitConverter.ToSingle(span.Slice(readCount, span.Length - readCount));
		readCount += sizeof(float);
		this.posY = BitConverter.ToSingle(span.Slice(readCount, span.Length - readCount));
		readCount += sizeof(float);
		this.posZ = BitConverter.ToSingle(span.Slice(readCount, span.Length - readCount));
		readCount += sizeof(float);
    }

    public ArraySegment<byte> Write()
    {
        ArraySegment<byte> seg = SendBufferHelper.Open(65535);

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
		
		success &= BitConverter.TryWriteBytes(span.Slice(writeCount, span.Length - writeCount), this.posX);
		writeCount += sizeof(float);
		
		success &= BitConverter.TryWriteBytes(span.Slice(writeCount, span.Length - writeCount), this.posY);
		writeCount += sizeof(float);
		
		success &= BitConverter.TryWriteBytes(span.Slice(writeCount, span.Length - writeCount), this.posZ);
		writeCount += sizeof(float);

        // 최종 패킷 크기 입력
        success &= BitConverter.TryWriteBytes(span, writeCount);

        if (success == false)
        {
            return null;
        }

        return SendBufferHelper.Close(writeCount);
    }
}


