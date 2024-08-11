#include "pch.h"
#include "ClientSession.h"
#include "ClientSessionManager.h"
#include "Room.h"
#include "Player.h"

void ClientSession::OnConnected()
{
	GClientSessionManager.Add(static_pointer_cast<ClientSession>(shared_from_this()));
}

void ClientSession::OnDisconnected()
{
	GClientSessionManager.Remove(static_pointer_cast<ClientSession>(shared_from_this()));

	if (_currentPlayer)
	{
		if (auto room = _currentPlayer->GetRoom())
		{
			room->PushJob(&Room::Leave, _currentPlayer);
		}
	}

	_currentPlayer = nullptr;
	_players.clear();
}

void ClientSession::OnRecvPacket(BYTE* buffer, int32 len)
{
	PacketSessionRef session = GetPacketSessionRef();
	ClientPacketHandler::HandlePacket(session, buffer, len);
}

void ClientSession::OnSend(int32 len)
{
}
