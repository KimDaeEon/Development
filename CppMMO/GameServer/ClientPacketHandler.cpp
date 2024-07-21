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

	// TODO: 웹 서버 거쳐서 인증하는 과정 추가
	// TODO: DB 에서 계정 정보 읽어오기
	Protocol::S_LOGIN loginPkt;

	for (int32 i = 0; i < 3; i++)
	{
		Protocol::ActorInfo* playerCharacterInfo = loginPkt.add_playercharacters();

		// 위치 임시 값 세팅
		Protocol::Position position;
		position.set_x(0);
		position.set_y(0);
		position.set_z(0);

		// 회전 임시 값 세팅
		Protocol::Rotation rotation;
		rotation.set_x(0);
		rotation.set_y(0);
		rotation.set_z(0);

		// 스케일 임시 값 세팅
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

	// TODO : 아래 부분 RoomManager를 통한 로직으로 변경 필요

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

