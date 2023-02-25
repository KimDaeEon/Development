#include "pch.h"
#include <iostream>

#include "ThreadManager.h"
#include "Service.h"
#include "Session.h"

BYTE sendBuffer[] = "Hello Server";

class ServerSession : public Session
{
public:
	virtual void OnConnected() override
	{
		cout << "Connected to Server" << endl;
		Send(sendBuffer, sizeof(sendBuffer));
	}

	virtual int32 OnRecv(BYTE* buffer, int32 len) override
	{
		cout << "OnRecv Len = " << len << endl;

		//this_thread::sleep_for(1s);

		Send(sendBuffer, sizeof(sendBuffer));
		return len;
	}

	virtual void OnSend(int32 len) override
	{
		cout << "OnSend Len = " << len << endl;
	}

	virtual void OnDisconnected() override
	{
		cout << "Disconnected" << endl;
	}

	~ServerSession()
	{
		cout << "~ServerSession()" << endl;
	}
};

int main()
{
	ClientServiceRef service = myMakeShared<ClientService>(
		NetworkAddress(L"127.0.0.1", 7777),
		myMakeShared<IocpCore>(),
		myMakeShared<ServerSession>,
		100
		);

	ASSERT_CRASH(service->Start());

	for (int32 i = 0; i < 2; i++)
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

