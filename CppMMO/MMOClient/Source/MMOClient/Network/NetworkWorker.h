#pragma once

// FSocket이 Blocking으로 작동해서, thread를 따로 빼고 해당 클래스를 통해 Network IO를 처리한다.
#include "CoreMinimal.h"

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
	virtual void Stop() override;
	virtual void Exit() override;

protected:
	FRunnableThread* Thread = nullptr;
	bool Running = true;
	FSocket* Socket = nullptr;
	TWeakPtr<class PacketSession> SessionRef;

private:
	bool ReceivePacket(TArray<uint8>& OUT Packet); // 패킷 하나 다 받으면 true 리턴
	bool ReceiveDesiredBytes(uint8* OUT Results, int32 Size);
};