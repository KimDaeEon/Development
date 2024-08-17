#include "pch.h"
#include "ClientSession.h"
#include "Player.h"
#include "Room.h"

PacketHandlerFunc GPacketHandler[UINT16_MAX];
std::shared_ptr<Room> GRoom = std::make_shared<Room>();

atomic<int> gameId = 0; // TODO: ���� id �߱� ���� ���� �ʿ�

bool ClientPacketHandler::Handle_INVALID(PacketSessionRef& session, BYTE* buffer, int32 len)
{
	// TODO : Log
	return false;
}

bool ClientPacketHandler::Handle_C_LOGIN(PacketSessionRef& session, Protocol::C_LOGIN& pkt)
{
	ClientSessionRef clientSession = static_pointer_cast<ClientSession>(session);

	// TODO: �� ���� ���ļ� �����ϴ� ���� �߰�
	// TODO: DB ���� ���� ���� �о����
	Protocol::S_LOGIN loginPkt;

	for (int32 i = 0; i < 3; i++)
	{
		Protocol::ActorInfo* playerCharacterInfo = loginPkt.add_playercharacters();
		playerCharacterInfo->set_gameid(gameId++);

		// ��ġ �ӽ� �� ����
		Protocol::Position position;
		position.set_x(gameId * 100);
		position.set_y(gameId * 100);
		position.set_z(gameId * 100);

		// ȸ�� �ӽ� �� ����
		Protocol::Rotation rotation;
		rotation.set_x(0);
		rotation.set_y(0);
		rotation.set_z(0);

		// ������ �ӽ� �� ����
		Protocol::Scale scale;
		scale.set_x(1);
		scale.set_y(1);
		scale.set_z(1);

		Protocol::Transform transform;
		*transform.mutable_position() = position;
		*transform.mutable_rotation() = rotation;
		*transform.mutable_scale() = scale;


		*playerCharacterInfo->mutable_transform() = transform;

		auto tempPlayer = std::make_shared<Player>();
		tempPlayer->SetActorInfo(*playerCharacterInfo);
		tempPlayer->SetOwnerSession(clientSession);
		clientSession->AddPaleyrRef(tempPlayer);
	}

	loginPkt.set_success(true);

	clientSession->SendPacket(loginPkt);

	return true;
}

bool ClientPacketHandler::Handle_C_ENTER_GAME(PacketSessionRef& session, Protocol::C_ENTER_GAME& pkt)
{
	ClientSessionRef clientSession = static_pointer_cast<ClientSession>(session);

	uint64 index = pkt.playercharacterindex();


	PlayerRef player = clientSession->GetPlayerRef(index);

	if (!player)
	{
		// TODO: Packet ����� ����� �� �� �ִ� Enum �߰� �ʿ�, ex) FAILED_PLAYER_NOT_FOUND
		Protocol::S_ENTER_GAME enterGamePkt;
		enterGamePkt.set_success(false);
		clientSession->SendPacket(enterGamePkt);
		return false;
	}

	clientSession->SetCurrentPlayer(player);

	// TODO : �Ʒ� �κ� RoomManager�� ���� �������� ���� �ʿ�
	player->SetRoom(GRoom);
	auto room = player->GetRoom();
	if (room)
	{
		room->PushJob([room, clientSession, player]()
			{
				room->HandleEnterGame(clientSession, player);
			});
	}

	return true;
}

bool ClientPacketHandler::Handle_C_LEAVE_GAME(PacketSessionRef& session, Protocol::C_LEAVE_GAME& pkt)
{
	ClientSessionRef clientSession = static_pointer_cast<ClientSession>(session);

	PlayerRef player = clientSession->GetCurrentPlayer();
	if (player == nullptr)
	{
		return false;
	}

	RoomRef room = player->GetRoom();
	if (room == nullptr)
	{
		return false;
	}

	if (room)
	{
		room->PushJob([room, clientSession, player]()
			{
				room->HandleLeaveGame(clientSession, player);
			});
	}

	return true;
}

bool ClientPacketHandler::Handle_C_CHAT(PacketSessionRef& session, Protocol::C_CHAT& pkt)
{
	cout << pkt.msg() << endl;

	Protocol::S_CHAT chatPkt;
	chatPkt.set_msg(pkt.msg());

	auto sendBuffer = ClientPacketHandler::MakeSendBuffer(chatPkt);

	session->Send(sendBuffer);

	return true;
}

