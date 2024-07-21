#include "pch.h"
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

	// TODO: �� ���� ���ļ� �����ϴ� ���� �߰�
	// TODO: DB ���� ���� ���� �о����
	Protocol::S_LOGIN loginPkt;

	for (int32 i = 0; i < 3; i++)
	{
		Protocol::ActorInfo* playerCharacterInfo = loginPkt.add_playercharacters();

		// ��ġ �ӽ� �� ����
		Protocol::Position position;
		position.set_x(0);
		position.set_y(0);
		position.set_z(0);

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

	// TODO : �Ʒ� �κ� RoomManager�� ���� �������� ���� �ʿ�

	PlayerRef player = clientSession->GetPlayerRef(index);
	clientSession->SetCurrentPlayer(player);
	clientSession->SetRoom(std::make_shared<Room>());

	auto room = clientSession->GetRoomRef();
	if (room)
	{
		room->PushJob(&Room::Enter, player);
		Protocol::S_ENTER_GAME enterGamePkt;
		enterGamePkt.set_success(true);
		clientSession->SendPacket(enterGamePkt);
	}

	return true;
}

bool ClientPacketHandler::Handle_C_LEAVE_GAME(PacketSessionRef& session, Protocol::C_LEAVE_GAME& pkt)
{
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

