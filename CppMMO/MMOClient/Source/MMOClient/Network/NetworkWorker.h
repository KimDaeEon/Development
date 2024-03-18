#pragma once

// FSocket�� Blocking���� �۵��ؼ�, thread�� ���� ���� �ش� Ŭ������ ���� Network IO�� ó���Ѵ�.
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

	// ���ۿ� ������ �о���� ���� ���
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
	bool ReceivePacket(TArray<uint8>& OUT Packet); // ��Ŷ �ϳ� �� ������ true ����
	bool ReceiveDesiredBytes(uint8* OUT Results, int32 Size);
};