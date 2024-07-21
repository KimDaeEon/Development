#include "pch.h"
#include <iostream>

#include "ThreadManager.h"
#include "Service.h"
#include "Session.h"
#include "BufferReader.h"
#include "ServerPacketHandler.h"
#include "ServerSession.h"

BYTE sendData[] = "Hello Server";

int main()
{
	ServerPacketHandler::Init();

	this_thread::sleep_for(1s);

	ClientServiceRef service = myMakeShared<ClientService>(
		NetworkAddress(L"127.0.0.1", 7777),
		myMakeShared<IocpCore>(),
		myMakeShared<ServerSession>,
		50
		);

	ASSERT_CRASH(service->Start());

	for (int32 i = 0; i < 5; i++)
	{
		GThreadManager->Launch([=]()
			{
				while (true)
				{
					service->GetIocpCore()->Dispatch();
				}
			});
	}

	Protocol::C_CHAT chatPkt;
	chatPkt.set_msg("Hello World");
	auto sendBuffer = ServerPacketHandler::MakeSendBuffer(chatPkt);

	while (true)
	{
		service->Broadcast(sendBuffer);
		this_thread::sleep_for(33ms);
	}

	GThreadManager->Join();
	return 0;
}

