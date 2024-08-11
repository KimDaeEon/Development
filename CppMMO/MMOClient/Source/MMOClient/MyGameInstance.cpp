// Fill out your copyright notice in the Description page of Project Settings.

#include "MyGameInstance.h"
#include "Sockets.h"
#include "Common/TcpSocketBuilder.h" // TCP ������ ���� �������� �����, ���� ���� �����ϰ� �ϵ��� ������
#include "Serialization/ArrayWriter.h"
#include "SocketSubsystem.h" // �÷��� �������� ���� �ý����� ���� �������̽� ����
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

		// Session �߰�
		GameServerSession = MakeShared<PacketSession>(Socket);
		GameServerSession->Run();

		// TODO: �κ� + ĳ���� ����â ����
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

	// TODO: ���߿� ���ʹ� �ٸ� �б⸦ �¿��� ó�� �ʿ�
	// �ߺ� ���� ����
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
