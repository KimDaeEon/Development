#include "pch.h"
#include "ClientPacketHandler.h"
#include "ClientSession.h"
#include "Player.h"
#include "Room.h"

PacketHandlerFunc GPacketHandler[UINT16_MAX];

bool ClientPacketHandler::Handle_INVALID(PacketSessionRef& session, BYTE* buffer, int32 len)
{
	// TODO : Log
	return false;
}

bool ClientPacketHandler::Handle_C_LOGIN(PacketSessionRef& session, Protocol::C_LOGIN& pkt)
{
	ClientSessionRef clientSession = static_pointer_cast<ClientSession>(session);

	// TODO : ���ռ� üũ

	Protocol::S_LOGIN loginPkt;
	loginPkt.set_success(true);

	// TODO: ID �߱� �����ʿ��Ѱ�?
	static Atomic<uint64> idGenerator = 1;

	// TODO: DB���� �÷��̾� ���� �������� + �޸𸮿� �ø���
	{
		auto player = loginPkt.add_players();
		player->set_name(u8"ss"); // TODO: C++17���� �̷��� �ص� �Ǵµ�, 20���� protobuff�� ���� ������ ��ȯ�� �ؾ��Ѵ�. C++20 ���� �߰��Ǵ� ���� ���鼭 �� �κ� ��� �� �� ���
		player->set_playertype(Protocol::PLAYER_TYPE_KNIGHT);

		PlayerRef playerRef = myMakeShared<Player>();
		playerRef->playerId = idGenerator++;
		playerRef->name = player->name();
		playerRef->type = player->playertype();
		playerRef->ownerSession = clientSession;

		clientSession->_players.push_back(playerRef);
	}

	{
		auto player = loginPkt.add_players();
		player->set_name(u8"ss");
		player->set_playertype(Protocol::PLAYER_TYPE_KNIGHT);

		PlayerRef playerRef = myMakeShared<Player>();
		playerRef->playerId = idGenerator++;
		playerRef->name = player->name();
		playerRef->type = player->playertype();
		playerRef->ownerSession = clientSession;

		clientSession->_players.push_back(playerRef);
	}

	auto sendBuffer = ClientPacketHandler::MakeSendBuffer(loginPkt);
	session->Send(sendBuffer);

	return true;
}

bool ClientPacketHandler::Handle_C_ENTER_GAME(PacketSessionRef& session, Protocol::C_ENTER_GAME& pkt)
{
	ClientSessionRef clientSession = static_pointer_cast<ClientSession>(session);

	uint64 index = pkt.playerindex();

	// TODO : ���ռ� üũ

	PlayerRef player = clientSession->_players[index];
	clientSession->_currentPlayer = player;
	clientSession->_room = GRoom;

	GRoom->PushJob(&Room::Enter, player);

	Protocol::S_ENTER_GAME enterGamePkt;
	enterGamePkt.set_success(true);
	auto sendBuffer = ClientPacketHandler::MakeSendBuffer(enterGamePkt);
	clientSession->_currentPlayer->ownerSession->Send(sendBuffer);

	return true;
}

bool ClientPacketHandler::Handle_C_CHAT(PacketSessionRef& session, Protocol::C_CHAT& pkt)
{
	cout << pkt.msg() << endl;

	Protocol::S_CHAT chatPkt;
	chatPkt.set_msg(pkt.msg());

	auto sendBuffer = ClientPacketHandler::MakeSendBuffer(chatPkt);

	session->Send(sendBuffer);
	//GRoom->PushJob(&Room::Broadcast, sendBuffer);

	return true;
}

