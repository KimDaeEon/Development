// Fill out your copyright notice in the Description page of Project Settings.


#include "Network/NetworkWorker.h"
#include "Sockets.h"
#include "Serialization/ArrayWriter.h"
#include "PacketSession.h"

RecvWorker::RecvWorker(FSocket* Socket, TSharedPtr<class PacketSession> Session) : Socket(Socket), SessionRef(Session)
{
	Thread = FRunnableThread::Create(this, TEXT("RecvWorkerThread"), 0, TPri_BelowNormal);
}

RecvWorker::~RecvWorker()
{
}

bool RecvWorker::Init()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("RecvWorker::Init"));
	return true;
}

uint32 RecvWorker::Run()
{
	while (Running)
	{
		TArray<uint8> Packet;
		if (ReceivePacket(OUT Packet))
		{
			if (TSharedPtr<PacketSession> Session = SessionRef.Pin())
			{
				Session->ReceivedPacketQueue.Enqueue(Packet);
			}
		}
	}

	return 0;
}

void RecvWorker::Stop()
{
	Running = false;
}

void RecvWorker::Exit()
{
}

bool RecvWorker::ReceivePacket(TArray<uint8>& OUT Packet)
{
	const int32 HeaderSize = sizeof(FPacketHeader);
	TArray<uint8> HeaderBuffer;
	HeaderBuffer.AddZeroed(HeaderSize); // ��� �����ŭ 0���� �ʱ�ȭ�ϸ� ���� Ȯ��

	if (ReceiveDesiredBytes(HeaderBuffer.GetData(), HeaderSize) == false)
	{
		return false;
	};

	// ID, Size ����
	FPacketHeader Header;
	{
		FMemoryReader Reader(HeaderBuffer);
		Reader << Header;
		UE_LOG(LogTemp, Log, TEXT("RecvWorker::ReceivePacket - ID : %d, Size : %d"), Header.PacketId, Header.PacketSize);
	}

	// ��Ŷ �ش� ����
	Packet = HeaderBuffer;

	// ��Ŷ ���� �Ľ�
	TArray<uint8> PayLoadBuffer;
	const int32 PayLoadSize = Header.PacketSize - HeaderSize;
	Packet.AddZeroed(PayLoadSize);

	if (ReceiveDesiredBytes(Packet.GetData(), PayLoadSize))
	{
		return true;
	}

	return false;
}

bool RecvWorker::ReceiveDesiredBytes(uint8* OUT Results, int32 Size)
{
	uint32 PendingDataSize = 0;
	if (Socket->HasPendingData(PendingDataSize) == false || PendingDataSize <= 0)
	{
		return false;
	}

	int32 Offset = 0;

	while (Size > 0)
	{
		int32 NumRead = 0;
		Socket->Recv(Results + Offset, Size, OUT NumRead);
		check(NumRead <= Size); // ���� �ִ� ��Ȳ, ���α׷� �ߴ�, ����� ���¶�� ����ſ��� ���⿡ ����

		if (NumRead <= 0)
		{
			return false;
		}

		Offset += NumRead;
		Size -= NumRead;
	}

	return true;
}
