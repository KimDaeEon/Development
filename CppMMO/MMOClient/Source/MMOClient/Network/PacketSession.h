#pragma once

#include "CoreMinimal.h"

// TODO: 아래 왜 그런지 모르겠는데 빨간줄 그어진다. 이거 원인 찾아서 해결하자.
class MMOCLIENT_API PacketSession : public TSharedFromThis<PacketSession>
{
public:
	PacketSession(class FSocket* Socket);
	~PacketSession();

	void Run();

	void Recv();

	void Disconnect();

public:
	class FSocket* Socket;

	TSharedPtr<class RecvWorker> RecvWorkerThread;
	TSharedPtr<class SendWorker> SendWorkerThread;

	// Network Thread에서 패킷 넘겨받는 큐
	TQueue<TArray<uint8>> ReceivedPacketQueue;
};

