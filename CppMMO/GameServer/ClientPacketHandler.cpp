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

	// TODO : 정합성 체크

	Protocol::S_LOGIN loginPkt;
	loginPkt.set_success(true);

	// TODO: ID 발급 개선필요한가?
	static Atomic<uint64> idGenerator = 1;

	// TODO: DB에서 플레이어 정보 가져오기 + 메모리에 올리기
	{
		auto player = loginPkt.add_players();
		player->set_name(u8"ss"); // TODO: C++17까진 이렇게 해도 되는데, 20부터 protobuff와 같이 쓰려면 변환을 해야한다. C++20 이후 추가되는 문법 보면서 이 부분 어떻게 할 지 고민
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

	// TODO : 정합성 체크

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

