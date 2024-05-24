#pragma once
#include "Protocol.pb.h"

// �Ʒ� define �� �ϳ��� �ִٸ� Unreal ������Ʈ ���� �ǹ�
#if UE_BUILD_DEBUG + UE_BUILD_DEVELOPMENT + UE_BUILD_TEST + UE_BUILD_SHIPPING >= 1
#include "MMOClient.h"
#include "SendBuffer.h"
#include "NetworkWorker.h"
#endif

using PacketHandlerFunc = std::function<bool(PacketSessionRef&, BYTE*, int32)>;
extern PacketHandlerFunc GPacketHandler[UINT16_MAX];


class ServerPacketHandler
{
public:
	// jinja2�� ���� parser�� �Ѱ��༭ �Ʒ��� ���� ����� �����ϴ�.
	// Django�� Blazor�� Template �������� �����ϴ�.
	enum : uint16
	{
		PKT_C_LOGIN = 1000,
		PKT_S_LOGIN = 1001,
		PKT_C_ENTER_GAME = 1002,
		PKT_S_ENTER_GAME = 1003,
		PKT_C_CHAT = 1004,
		PKT_S_CHAT = 1005,
	};

public:
	static bool Handle_INVALID(PacketSessionRef& session, BYTE* buffer, int32 len);

	// Protocol.proto�� �����Ǿ� �ڵ� ����
	static bool Handle_S_LOGIN(PacketSessionRef& session, Protocol::S_LOGIN&pkt);
	static bool Handle_S_ENTER_GAME(PacketSessionRef& session, Protocol::S_ENTER_GAME&pkt);
	static bool Handle_S_CHAT(PacketSessionRef& session, Protocol::S_CHAT&pkt);

public:
	static void Init()
	{
		for (int32 i = 0; i < UINT16_MAX; i++)
		{
			GPacketHandler[i] = Handle_INVALID;
		}

		// Protocol.proto�� �����Ǿ� �ڵ� ����
		GPacketHandler[PKT_S_LOGIN] = [](PacketSessionRef& session, BYTE* buffer, int32 len)
		{
			return HandlePacket<Protocol::S_LOGIN>(Handle_S_LOGIN, session, buffer, len);
		};
		GPacketHandler[PKT_S_ENTER_GAME] = [](PacketSessionRef& session, BYTE* buffer, int32 len)
		{
			return HandlePacket<Protocol::S_ENTER_GAME>(Handle_S_ENTER_GAME, session, buffer, len);
		};
		GPacketHandler[PKT_S_CHAT] = [](PacketSessionRef& session, BYTE* buffer, int32 len)
		{
			return HandlePacket<Protocol::S_CHAT>(Handle_S_CHAT, session, buffer, len);
		};

	}

	static bool HandlePacket(PacketSessionRef& session, BYTE* buffer, int32 len)
	{
		PacketHeader* header = reinterpret_cast<PacketHeader*>(buffer);
		return GPacketHandler[header->id](session, buffer, len);
	}

	// Protocol.proto�� �����Ǿ� �ڵ� ����
	static SendBufferRef MakeSendBuffer(Protocol::C_LOGIN& pkt) { return MakeSendBuffer(pkt, PKT_C_LOGIN); }
	static SendBufferRef MakeSendBuffer(Protocol::C_ENTER_GAME& pkt) { return MakeSendBuffer(pkt, PKT_C_ENTER_GAME); }
	static SendBufferRef MakeSendBuffer(Protocol::C_CHAT& pkt) { return MakeSendBuffer(pkt, PKT_C_CHAT); }

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

#if UE_BUILD_DEBUG + UE_BUILD_DEVELOPMENT + UE_BUILD_TEST + UE_BUILD_SHIPPING >= 1
		SendBufferRef sendBuffer = MakeShared<SendBuffer>(packetSize);
#else
		SendBufferRef sendBuffer = GSendBufferManager->Open(packetSize);
#endif

		PacketHeader* header = reinterpret_cast<PacketHeader*>(sendBuffer->Buffer());
		header->size = packetSize;
		header->id = pktId;

		pkt.SerializeToArray(&header[1], dataSize);

		sendBuffer->Close(packetSize);
		return sendBuffer;
	}
};
