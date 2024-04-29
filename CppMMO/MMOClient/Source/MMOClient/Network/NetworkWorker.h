#pragma once

// FSocket이 Blocking으로 작동해서, thread를 따로 빼고 해당 클래스를 통해 Network IO를 처리한다.
#include "CoreMinimal.h"
#include "MMOClient.h"

class FSocket;

struct MMOCLIENT_API FPacketHeader
{
	FPacketHeader() : PacketSize(0), PacketId(0)
	{
	}

	FPacketHeader(uint16 PacketSize, uint16 PacketId) : PacketSize(PacketSize), PacketId(PacketId)
	{
	}

	// 버퍼에 데이터 밀어넣을 때에 사용
	friend FArchive& operator<<(FArchive& Ar, FPacketHeader& PacketHeader)
	{
		Ar << PacketHeader.PacketSize;
		Ar << PacketHeader.PacketId;
		return Ar;
	}

	uint16 PacketSize;
	uint16 PacketId;
};

class MMOCLIENT_API RecvWorker : public FRunnable
{
public:
	RecvWorker(FSocket* Socket, TSharedPtr<class PacketSession> Session);
	~RecvWorker();

	virtual bool Init() override;
	virtual uint32 Run() override;
	virtual void Exit() override;

	void Destroy();

protected:
	FRunnableThread* Thread = nullptr;
	bool Running = true;
	FSocket* Socket = nullptr;
	TWeakPtr<class PacketSession> SessionRef;

private:
	bool ReceivePacket(TArray<uint8>& OUT Packet); // 패킷 하나 다 받으면 true 리턴
	bool ReceiveDesiredBytes(uint8* OUT Results, int32 Size);
};

class MMOCLIENT_API SendWorker : public FRunnable
{
public:
	SendWorker(FSocket* Socket, TSharedPtr<class PacketSession> Session);
	~SendWorker();

	virtual bool Init() override;
	virtual uint32 Run() override;
	virtual void Exit() override;

	bool SendPacket(SendBufferRef SendBuffer);

	void Destroy();

private:
	bool SendDesiredBytes(const uint8* Buffer, int32 Size);

protected:
	FRunnableThread* Thread = nullptr;
	bool Running = true;
	FSocket* Socket;
	TWeakPtr<class PacketSession> SessionRef;
};