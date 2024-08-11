#include "Network/PacketSession.h"
#include "NetworkWorker.h"
#include "ServerPacketHandler.h"

PacketSession::PacketSession(class FSocket* Socket) : Socket(Socket)
{
}

PacketSession::~PacketSession()
{
	Disconnect();
}

void PacketSession::Run()
{
	RecvWorkerThread = MakeShared<RecvWorker>(Socket, AsShared());
	SendWorkerThread = MakeShared<SendWorker>(Socket, AsShared());
}

void PacketSession::Recv()
{
}

void PacketSession::Disconnect()
{
	if (RecvWorkerThread)
	{
		RecvWorkerThread->Destroy();
		RecvWorkerThread = nullptr;
	}

	if (SendWorkerThread)
	{
		SendWorkerThread->Destroy();
		SendWorkerThread = nullptr;
	}
}

void PacketSession::HandleRecvPackets()
{
	while (true)
	{
		TArray<uint8> Packet;
		if (ReceivedPacketQueue.Dequeue(OUT Packet) == false)
		{
			break;
		}

		PacketSessionRef self = AsShared();
		ServerPacketHandler::HandlePacket(self, Packet.GetData(), Packet.Num());
	}
}

void PacketSession::RegisterSend(SendBufferRef SendBuffer)
{
	SendPacketQueue.Enqueue(SendBuffer);
}
