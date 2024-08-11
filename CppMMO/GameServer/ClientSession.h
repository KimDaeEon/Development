#pragma once
#include "Session.h"
#include "ClientPacketHandler.h"

class ClientSession : public PacketSession
{
public:
	~ClientSession()
	{
		cout << "~ClientSession" << endl;
	}

	virtual void OnConnected() override;
	virtual void OnDisconnected() override;
	virtual void OnRecvPacket(BYTE* buffer, int32 len) override;
	virtual void OnSend(int32 len) override;

	template<typename T>
	void SendPacket(T pkt)
	{
		SendBufferRef sendBuffer = ClientPacketHandler::MakeSendBuffer(pkt);
		Send(sendBuffer);
	}

	void AddPaleyrRef(PlayerRef player)
	{
		_players.push_back(player);
	}

	PlayerRef GetPlayerRef(int32 idx)
	{
		return _players[idx];
	}

	void SetCurrentPlayer(PlayerRef player)
	{
		_currentPlayer = player;
	}

	PlayerRef GetCurrentPlayer()
	{
		return _currentPlayer;
	}

private:
	myVector<PlayerRef> _players;
	PlayerRef _currentPlayer;
};