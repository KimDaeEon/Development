#include "pch.h"
#include "ServerPacketHandler.h"
#include <BufferReader.h>
#include "Protocol.pb.h"

PacketHandlerFunc GPacketHandler[UINT16_MAX];

bool ServerPacketHandler::Handle_INVALID(PacketSessionRef& session, BYTE* buffer, int32 len)
{
	PacketHeader* header = reinterpret_cast<PacketHeader*>(buffer);
	// TODO : Log
	return false;
}

bool ServerPacketHandler::Handle_S_LOGIN(PacketSessionRef& session, Protocol::S_LOGIN& pkt)
{
	if (pkt.success() == false)
	{
		return true;
	}

	if (pkt.playercharacters().size() == 0)
	{
		// TODO: ĳ���� ����â
	}

	for (auto& player : pkt.playercharacters())
	{
		// TODO: �Ʒ��κ� �ѱ� ���ڿ��� ����� �ȳ��´�. �ذ� �ʿ�.
		cout << "player.name : " << player.name() << endl;
	}

	Protocol::C_ENTER_GAME enterGamePkt;
	enterGamePkt.set_playercharacterindex(0); // TODO: ����� ������ ù��° ĳ���ͷ� ����, �Ŀ� �����ϴ� ������ ����
	auto sendBuffer = ServerPacketHandler::MakeSendBuffer(enterGamePkt);
	session->Send(sendBuffer);

	return true;
}

bool ServerPacketHandler::Handle_S_ENTER_GAME(PacketSessionRef& session, Protocol::S_ENTER_GAME& pkt)
{
	return true;
}

bool ServerPacketHandler::Handle_S_LEAVE_GAME(PacketSessionRef& session, Protocol::S_LEAVE_GAME& pkt)
{
	return false;
}

bool ServerPacketHandler::Handle_S_SPAWN(PacketSessionRef& session, Protocol::S_SPAWN& pkt)
{
	return false;
}

bool ServerPacketHandler::Handle_S_DESPAWN(PacketSessionRef& session, Protocol::S_DESPAWN& pkt)
{
	return false;
}

bool ServerPacketHandler::Handle_S_CHAT(PacketSessionRef& session, Protocol::S_CHAT& pkt)
{
	cout << "S_CHAT : " << pkt.msg() << endl;
	return true;
}
