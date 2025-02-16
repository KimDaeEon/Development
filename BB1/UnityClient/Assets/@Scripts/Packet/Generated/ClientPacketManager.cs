using Google.Protobuf;
using Google.Protobuf.Protocol;
using ServerCore;
using System;
using System.Collections.Generic;

public enum MsgId
{
	S_Connected = 1,
	C_Test = 2,
	C_LeaveGame = 3,
	S_LeaveGame = 4,
	C_EnterGame = 5,
	S_EnterGame = 6,
	S_Spawn = 7,
	S_Despawn = 8,
	C_Move = 9,
	S_Move = 10,
}

class PacketManager
{
	#region Singleton
	static PacketManager _instance = new PacketManager();
	public static PacketManager Instance { get { return _instance; } }
	#endregion

	PacketManager()
	{
		Register();
	}

	Dictionary<ushort, Action<PacketSession, ArraySegment<byte>, ushort>> _packetCreatorMap = new Dictionary<ushort, Action<PacketSession, ArraySegment<byte>, ushort>>();
	Dictionary<ushort, Action<PacketSession, IMessage>> _packetHandlerMap = new Dictionary<ushort, Action<PacketSession, IMessage>>();
		
	public Action<PacketSession, ushort, IMessage> CustomHandler { get; set; }

	public void Register()
	{		
		_packetCreatorMap.Add((ushort)MsgId.S_Connected, MakePacket<S_Connected>);
		_packetHandlerMap.Add((ushort)MsgId.S_Connected, PacketHandler.S_ConnectedHandler);		
		_packetCreatorMap.Add((ushort)MsgId.S_LeaveGame, MakePacket<S_LeaveGame>);
		_packetHandlerMap.Add((ushort)MsgId.S_LeaveGame, PacketHandler.S_LeaveGameHandler);		
		_packetCreatorMap.Add((ushort)MsgId.S_EnterGame, MakePacket<S_EnterGame>);
		_packetHandlerMap.Add((ushort)MsgId.S_EnterGame, PacketHandler.S_EnterGameHandler);		
		_packetCreatorMap.Add((ushort)MsgId.S_Spawn, MakePacket<S_Spawn>);
		_packetHandlerMap.Add((ushort)MsgId.S_Spawn, PacketHandler.S_SpawnHandler);		
		_packetCreatorMap.Add((ushort)MsgId.S_Despawn, MakePacket<S_Despawn>);
		_packetHandlerMap.Add((ushort)MsgId.S_Despawn, PacketHandler.S_DespawnHandler);		
		_packetCreatorMap.Add((ushort)MsgId.S_Move, MakePacket<S_Move>);
		_packetHandlerMap.Add((ushort)MsgId.S_Move, PacketHandler.S_MoveHandler);
	}

	public void OnRecvPacket(PacketSession session, ArraySegment<byte> buffer)
	{
		ushort count = 0;

		ushort size = BitConverter.ToUInt16(buffer.Array, buffer.Offset);
		count += 2;
		ushort id = BitConverter.ToUInt16(buffer.Array, buffer.Offset + count);
		count += 2;

		Action<PacketSession, ArraySegment<byte>, ushort> action = null;
		if (_packetCreatorMap.TryGetValue(id, out action))
			action.Invoke(session, buffer, id);
	}

	void MakePacket<T>(PacketSession session, ArraySegment<byte> buffer, ushort id) where T : IMessage, new()
	{
		T pkt = new T();
		pkt.MergeFrom(buffer.Array, buffer.Offset + 4, buffer.Count - 4);

		if (CustomHandler != null)
		{
			CustomHandler.Invoke(session, id, pkt);
		}
		else
		{
			Action<PacketSession, IMessage> action = null;
			if (_packetHandlerMap.TryGetValue(id, out action))
				action.Invoke(session, pkt);
		}
	}

	public Action<PacketSession, IMessage> GetPacketHandler(ushort id)
	{
		Action<PacketSession, IMessage> action = null;
		if (_packetHandlerMap.TryGetValue(id, out action))
			return action;
		return null;
	}
}