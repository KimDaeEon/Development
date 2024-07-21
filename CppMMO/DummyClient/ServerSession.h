#pragma once
#include "Session.h"

class ServerSession : public PacketSession
{
public:
	virtual void OnConnected() override
	{
		//cout << "Connected to Server" << endl;

		Protocol::C_LOGIN pkt;
		auto sendBuffer = ServerPacketHandler::MakeSendBuffer(pkt);
		Send(sendBuffer);
	}

	virtual void OnRecvPacket(BYTE* buffer, int32 len) override
	{
		PacketSessionRef session = GetPacketSessionRef();
		ServerPacketHandler::HandlePacket(session, buffer, len);
	}

	virtual void OnSend(int32 len) override
	{
		//cout << "OnSend Len = " << len << endl;
	}

	virtual void OnDisconnected() override
	{
		//cout << "Disconnected" << endl;
	}

	template<typename T>
	void SendPacket(T pkt)
	{
		SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(pkt);
		Send(sendBuffer);
	}

	~ServerSession()
	{
		cout << "~ServerSession()" << endl;
	}
};