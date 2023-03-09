#include "pch.h"
#include <iostream>

#include "ThreadManager.h"
#include "Service.h"
#include "Session.h"
#include "BufferReader.h"

BYTE sendData[] = "Hello Server";

class ServerSession : public PacketSession
{
public:
	virtual void OnConnected() override
	{
		//cout << "Connected to Server" << endl;

		SendBufferRef sendBuffer = GSendBufferManager->Open(4096);
		::memcpy(sendBuffer->Buffer(), sendData, sizeof(sendData));
		sendBuffer->Close(sizeof(sendData));

		Send(sendBuffer);
	}

	virtual int32 OnRecvPacket(BYTE* buffer, int32 len) override
	{
		BufferReader br(buffer, len);
		PacketHeader header;
		br >> header;

		uint32 temp1;
		uint32 temp2;
		uint32 temp3;

		br >> temp1 >> temp2 >> temp3;

		cout << "temp1: " << temp1 << endl;
		cout << "temp2: " << temp2 << endl;
		cout << "temp3: " << temp3 << endl;

		char recvBuffer[4096];
		br.Read(recvBuffer, header.size - br.ReadSize());
		cout << recvBuffer << endl;
		return len;
	}

	virtual void OnSend(int32 len) override
	{
		//cout << "OnSend Len = " << len << endl;
	}

	virtual void OnDisconnected() override
	{
		//cout << "Disconnected" << endl;
	}

	~ServerSession()
	{
		cout << "~ServerSession()" << endl;
	}
};

int main()
{
	this_thread::sleep_for(1s);

	ClientServiceRef service = myMakeShared<ClientService>(
		NetworkAddress(L"127.0.0.1", 7777),
		myMakeShared<IocpCore>(),
		myMakeShared<ServerSession>,
		1000
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

	GThreadManager->Join();
	return 0;
}

