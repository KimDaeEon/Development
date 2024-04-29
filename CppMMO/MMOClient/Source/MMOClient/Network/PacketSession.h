#pragma once

#include "CoreMinimal.h"
#include "MMOClient.h"

// TODO: �Ʒ� �� �׷��� �𸣰ڴµ� ������ �׾�����. �̰� ���� ã�Ƽ� �ذ�����.
class MMOCLIENT_API PacketSession : public TSharedFromThis<PacketSession>
{
public:
	PacketSession(class FSocket* Socket);
	~PacketSession();

	void Run();

	void Recv();

	void Disconnect();

	UFUNCTION(BlueprintCallable)
	void HandleRecvPackets();

	void SendPacket(SendBufferRef SendBuffer);

public:
	class FSocket* Socket;

	TSharedPtr<class RecvWorker> RecvWorkerThread;
	TSharedPtr<class SendWorker> SendWorkerThread;

	// Network Thread���� ��Ŷ �Ѱܹ޴� ť
	TQueue<TArray<uint8>> ReceivedPacketQueue;
	TQueue<SendBufferRef> SendPacketQueue;
};

