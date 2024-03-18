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
	HeaderBuffer.AddZeroed(HeaderSize); // 헤더 사이즈만큼 0으로 초기화하며 공간 확보

	if (ReceiveDesiredBytes(HeaderBuffer.GetData(), HeaderSize) == false)
	{
		return false;
	};

	// ID, Size 추출
	FPacketHeader Header;
	{
		FMemoryReader Reader(HeaderBuffer);
		Reader << Header;
		UE_LOG(LogTemp, Log, TEXT("RecvWorker::ReceivePacket - ID : %d, Size : %d"), Header.PacketId, Header.PacketSize);
	}

	// 페킷 해더 복사
	Packet = HeaderBuffer;

	// 패킷 내용 파싱
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
		check(NumRead <= Size); // 문제 있는 상황, 프로그램 중단, 디버깅 상태라면 디버거에서 여기에 정지

		if (NumRead <= 0)
		{
			return false;
		}

		Offset += NumRead;
		Size -= NumRead;
	}

	return true;
}
