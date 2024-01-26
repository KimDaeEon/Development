#pragma once
#include "Session.h"

class ClientSession : public PacketSession
{
	friend class ClientPacketHandler;

public:
	~ClientSession()
	{
		cout << "~ClientSession" << endl;
	}

	virtual void OnConnected() override;
	virtual void OnDisconnected() override;
	virtual void OnRecvPacket(BYTE* buffer, int32 len) override;
	virtual void OnSend(int32 len) override;

private:
	myVector<PlayerRef> _players;

	PlayerRef _currentPlayer;
	weak_ptr<class Room> _room;
};