#include "ServerPacketHandler.h"
#include "PacketSession.h"
#include <BufferReader.h>
#include "Protocol.pb.h"
#include "MyGameInstance.h"


PacketHandlerFunc GPacketHandler[UINT16_MAX];

bool ServerPacketHandler::Handle_INVALID(PacketSessionRef& session, BYTE* buffer, int32 len)
{
	// TODO : Log
	return false;
}

bool ServerPacketHandler::Handle_S_LOGIN(PacketSessionRef& session, Protocol::S_LOGIN& pkt)
{
	// TODO: 캐릭터 목록 게임에서 나오게 하기
	for (auto& player : pkt.playercharacters())
	{
		UE_LOG(LogTemp, Warning, TEXT("Player : %s"), *FString(player.name().c_str()));
	}

	// TODO: 원하는 캐릭터 선택해서 보내는 기능 추가
	Protocol::C_ENTER_GAME enterGamePkt;
	enterGamePkt.set_playercharacterindex(0);
	session->SendPacket(enterGamePkt);

	return true;
}

bool ServerPacketHandler::Handle_S_ENTER_GAME(PacketSessionRef& session, Protocol::S_ENTER_GAME& pkt)
{
	if(auto* GameInstance = Cast<UMyGameInstance>(GWorld->GetGameInstance()))
	{
		GameInstance->HandleSpawn(pkt.playercharacter());
	}

	return true;
}

bool ServerPacketHandler::Handle_S_LEAVE_GAME(PacketSessionRef& session, Protocol::S_LEAVE_GAME& pkt)
{
	return true;
}

bool ServerPacketHandler::Handle_S_SPAWN(PacketSessionRef& session, Protocol::S_SPAWN& pkt)
{
	if (auto* GameInstance = Cast<UMyGameInstance>(GWorld->GetGameInstance()))
	{
		GameInstance->HandleSpawn(pkt);
	}

	return true;
}

bool ServerPacketHandler::Handle_S_DESPAWN(PacketSessionRef& session, Protocol::S_DESPAWN& pkt)
{
	return true;
}

bool ServerPacketHandler::Handle_S_CHAT(PacketSessionRef& session, Protocol::S_CHAT& pkt)
{
	UE_LOG(LogTemp, Warning, TEXT("S_CHAT : %s"), *FString(pkt.msg().c_str()));
	return true;
}
