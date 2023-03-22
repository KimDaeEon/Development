#pragma once
#include "Protocol.pb.h"

using PacketHandlerFunc = std::function<bool(PacketSessionRef&, BYTE*, int32)>;
extern PacketHandlerFunc GPacketHandler[UINT16_MAX];


class ClientPacketHandler
{
public:
	// jinja2�� ���� parser�� �Ѱ��༭ �Ʒ��� ���� ����� �����ϴ�.
	// Django�� Blazor�� Template �������� �����ϴ�.
	enum : uint16
	{
		PKT_C_TEST = 1000,
		PKT_S_TEST = 1001,
	};

public:
	static bool Handle_INVALID(PacketSessionRef& session, BYTE* buffer, int32 len);

	// Protocol.proto�� �����Ǿ� �ڵ� ����
	static bool Handle_C_TEST(PacketSessionRef& session, Protocol::C_TEST&pkt);

public:
	static void Init()
	{
		for (int32 i = 0; i < UINT16_MAX; i++)
		{
			GPacketHandler[i] = Handle_INVALID;
		}

		// Protocol.proto�� �����Ǿ� �ڵ� ����
		GPacketHandler[PKT_C_TEST] = [](PacketSessionRef& session, BYTE* buffer, int32 len)
		{
			return HandlePacket<Protocol::C_TEST>(Handle_C_TEST, session, buffer, len);
		};

	}

	static bool HandlePacket(PacketSessionRef& session, BYTE* buffer, int32 len)
	{
		PacketHeader* header = reinterpret_cast<PacketHeader*>(buffer);
		return GPacketHandler[header->id](session, buffer, len);
	}

	// Protocol.proto�� �����Ǿ� �ڵ� ����
	static SendBufferRef MakeSendBuffer(Protocol::S_TEST& pkt) { return MakeSendBuffer(pkt, PKT_S_TEST); }

private:
	template<typename PacketType, typename ProcessFunc>
	static bool HandlePacket(ProcessFunc func, PacketSessionRef& session, BYTE* buffer, int32 len)
	{
		PacketType pkt;
		int headerOffset = sizeof(PacketHeader);

		if (pkt.ParseFromArray(buffer + headerOffset, len - headerOffset) == false)
		{
			return false;
		}

		return func(session, pkt);
	}

	template<typename T>
	static SendBufferRef MakeSendBuffer(T& pkt, uint16 pktId)
	{
		const uint16 dataSize = static_cast<uint16>(pkt.ByteSizeLong());
		const uint16 packetSize = dataSize + sizeof(PacketHeader);

		SendBufferRef sendBuffer = GSendBufferManager->Open(packetSize);

		PacketHeader* header = reinterpret_cast<PacketHeader*>(sendBuffer->Buffer());
		header->size = packetSize;
		header->id = pktId;

		ASSERT_CRASH(pkt.SerializeToArray(&header[1], dataSize));

		sendBuffer->Close(packetSize);
		return sendBuffer;
	}
};
