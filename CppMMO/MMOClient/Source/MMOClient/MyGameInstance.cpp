// Fill out your copyright notice in the Description page of Project Settings.

#include "MyGameInstance.h"
#include "Sockets.h"
#include "Common/TcpSocketBuilder.h" // TCP 소켓을 빌더 패턴으로 만들고, 소켓 설정 간결하게 하도록 도와줌
#include "Serialization/ArrayWriter.h"
#include "SocketSubsystem.h" // 플랫폼 독립적인 소켓 시스템을 위한 인터페이스 제공
#include "PacketSession.h"

void UMyGameInstance::Init()
{
	Super::Init();

	ServerPacketHandler::Init();
}

bool UMyGameInstance::CheckSocketConnection()
{
	return Socket == nullptr || GameServerSession == nullptr;
}

void UMyGameInstance::ConnectToGameServer()
{
	Socket = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(TEXT("Stream"), TEXT("Client Socket"));

	FIPv4Address ip;
	FIPv4Address::Parse(IpAddress, ip);

	TSharedRef<FInternetAddr> internetAddr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	internetAddr->SetIp(ip.Value);
	internetAddr->SetPort(Port);

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Connecting to %s:%d"), *IpAddress, Port));

	bool connected = Socket->Connect(*internetAddr);

	if (connected)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Connection Success"));

		// Session 추가
		GameServerSession = MakeShared<PacketSession>(Socket);
		GameServerSession->Run();

		// TODO: 로비 + 캐릭터 선택창 구현
		{
			Protocol::C_LOGIN loginPkt;
			SendBufferRef sendBuffer = ServerPacketHandler::MakeSendBuffer(loginPkt);
			SendPacket(sendBuffer);
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Failed to connect to server"));
	}
}

void UMyGameInstance::DisconnectFromGameServer()
{
}

void UMyGameInstance::HandleRecvPacket()
{
	if (CheckSocketConnection())
		return;

	GameServerSession->HandleRecvPackets();
}

void UMyGameInstance::SendPacket(SendBufferRef SendBuf)
{
	if (CheckSocketConnection())
		return;

	GameServerSession->RegisterSend(SendBuf);
}

void UMyGameInstance::HandleSpawn(const Protocol::ActorInfo& actor)
{
	if (CheckSocketConnection())
	{
		return;
	}

	auto* world = GetWorld();
	if (world == nullptr)
	{
		return;
	}	

	const uint64 id = actor.gameid();

	// TODO: 나중에 몬스터는 다른 분기를 태워서 처리 필요
	// 중복 생성 방지
	if(PlayerMap.Find(id) != nullptr)
	{
		return;
	}

	FVector spawnLocation(actor.transform().position().x(), actor.transform().position().y(), actor.transform().position().z());
	AActor* addedActor = world->SpawnActor(PlayerClass, &spawnLocation); 
	PlayerMap.Add(actor.gameid(), addedActor);
}

void UMyGameInstance::HandleSpawn(const Protocol::S_ENTER_GAME& enterGamePkt)
{
	HandleSpawn(enterGamePkt.playercharacter());
}

void UMyGameInstance::HandleSpawn(const Protocol::S_SPAWN& spawnPkt)
{
	for (auto& actor : spawnPkt.actors())
	{
		HandleSpawn(actor);
	}
}
