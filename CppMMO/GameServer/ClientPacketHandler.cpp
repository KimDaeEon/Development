#include "pch.h"
#include "ClientPacketHandler.h"
#include "..\DummyClient\ServerPacketHandler.h"

PacketHandlerFunc GPacketHandler[UINT16_MAX];

bool ClientPacketHandler::Handle_INVALID(PacketSessionRef& session, BYTE* buffer, int32 len)
{
	// TODO : Log
	return false;
}

bool ClientPacketHandler::Handle_C_TEST(PacketSessionRef& session, Protocol::C_TEST& pkt)
{
	// TODO
	return false;
}
