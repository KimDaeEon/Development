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
		// TODO: 캐릭터 생성창
	}

	for (auto& player : pkt.playercharacters())
	{
		// TODO: 아래부분 한글 문자열이 제대로 안나온다. 해결 필요.
		cout << "player.name : " << player.name() << endl;
	}

	Protocol::C_ENTER_GAME enterGamePkt;
	enterGamePkt.set_playercharacterindex(0); // TODO: 현재는 무조건 첫번째 캐릭터로 입장, 후에 선택하는 것으로 수정
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
