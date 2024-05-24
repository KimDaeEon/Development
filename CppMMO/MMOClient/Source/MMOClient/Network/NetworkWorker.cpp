// Fill out your copyright notice in the Description page of Project Settings.

#include "Network/NetworkWorker.h"
#include "Network/Sendbuffer.h"
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
		TArray<uint8> packet;
		if (ReceivePacket(OUT packet))
		{
			if (TSharedPtr<PacketSession> session = SessionRef.Pin())
			{
				session->ReceivedPacketQueue.Enqueue(packet);
			}
		}
	}

	return 0;
}

void RecvWorker::Exit()
{
}

void RecvWorker::Destroy()
{
	Running = false;
}

bool RecvWorker::ReceivePacket(TArray<uint8>& OUT Packet)
{
	const int32 headerSize = sizeof(FPacketHeader);
	TArray<uint8> headerBuffer;
	headerBuffer.AddZeroed(headerSize); // 헤더 사이즈만큼 0으로 초기화하며 공간 확보

	if (ReceiveDesiredBytes(headerBuffer.GetData(), headerSize) == false)
	{
		return false;
	};

	// ID, Size 추출
	FPacketHeader header;
	{
		FMemoryReader reader(headerBuffer);
		reader << header;
		UE_LOG(LogTemp, Log, TEXT("RecvWorker::ReceivePacket - ID : %d, Size : %d"), header.PacketId, header.PacketSize);
	}

	// 페킷 해더 복사
	Packet = headerBuffer;

	// 패킷 내용 파싱
	TArray<uint8> payLoadBuffer;
	const int32 payLoadSize = header.PacketSize - headerSize;
	Packet.AddZeroed(payLoadSize);

	if (ReceiveDesiredBytes(&Packet[headerSize], payLoadSize))
	{
		return true;
	}

	return false;
}

bool RecvWorker::ReceiveDesiredBytes(uint8* OUT Results, int32 Size)
{
	uint32 pendingDataSize;
	if (Socket->HasPendingData(pendingDataSize) == false || pendingDataSize <= 0)
	{
		return false;
	}

	int32 offset = 0;

	while (Size > 0)
	{
		int32 numRead = 0;
		Socket->Recv(Results + offset, Size, OUT numRead);
		check(numRead <= Size); // 문제 있는 상황, 프로그램 중단, 디버깅 상태라면 디버거에서 여기에 정지

		if (numRead <= 0)
		{
			return false;
		}

		offset += numRead;
		Size -= numRead;
	}

	return true;
}

SendWorker::SendWorker(FSocket* Socket, TSharedPtr<class PacketSession> Session) : Socket(Socket), SessionRef(Session)
{
	Thread = FRunnableThread::Create(this, TEXT("SendWorkerThread"));
}

SendWorker::~SendWorker()
{
}

bool SendWorker::Init()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Send Worker Init")));

	return true;
}

uint32 SendWorker::Run()
{
	while (Running)
	{
		SendBufferRef sendBuffer;

		if (TSharedPtr<PacketSession> session = SessionRef.Pin())
		{
			if (session->SendPacketQueue.Dequeue(OUT sendBuffer))
			{
				SendPacket(sendBuffer);
			}
		}
		// Sleep 넣을지 고민
	}

	return 0;
}

void SendWorker::Exit()
{
}

bool SendWorker::SendPacket(SendBufferRef SendBuffer)
{
	if (SendDesiredBytes(SendBuffer->Buffer(), SendBuffer->WriteSize()) == false)
		return false;

	return true;
}

void SendWorker::Destroy()
{
	Running = false;
}

bool SendWorker::SendDesiredBytes(const uint8* Buffer, int32 Size)
{
	while (Size > 0)
	{
		int32 bytesSent = 0;
		if (Socket->Send(Buffer, Size, bytesSent) == false)
			return false;

		Size -= bytesSent;
		Buffer += bytesSent;
	}

	return true;
}
