// Fill out your copyright notice in the Description page of Project Settings.

#include "MyGameInstance.h"
#include "Sockets.h"
#include "Common/TcpSocketBuilder.h" // TCP 소켓을 빌더 패턴으로 만들고, 소켓 설정 간결하게 하도록 도와줌
#include "Serialization/ArrayWriter.h"
#include "SocketSubsystem.h" // 플랫폼 독립적인 소켓 시스템을 위한 인터페이스 제공
#include "PacketSession.h"
#include "MyMMOClientPlayer.h"

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
	if (CheckSocketConnection())
		return;

	Protocol::C_LEAVE_GAME leaveGamePkt;

	// Unreal 같은 경우는 로직이 싱글 스레드니.. 이 부분 문제 없겠지?
	if (GameServerSession)
	{
		GameServerSession->SendPacket(leaveGamePkt);
	}
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

void UMyGameInstance::HandleSpawn(const Protocol::ActorInfo& actor, bool isMine)
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
	if (PlayerMap.Find(id) != nullptr)
	{
		return;
	}

	FVector spawnLocation(actor.transform().position().x(), actor.transform().position().y(), actor.transform().position().z());


	if (isMine)
	{
		auto* playerController = UGameplayStatics::GetPlayerController(this, 0);
		AMyMMOClientPlayer* player = Cast<AMyMMOClientPlayer>(playerController->GetPawn());
		if (player == nullptr)
		{
			return;
		}

		player->SetActorInfo(&actor);
		MyPlayer = player;
		PlayerMap.Add(id, player);
	}
	else
	{
		AMMOClientPlayer* addedOtherPlayer = Cast<AMMOClientPlayer>(world->SpawnActor(OtherPlayerClass, &spawnLocation));

		addedOtherPlayer->SetActorInfo(&actor);
		PlayerMap.Add(id, addedOtherPlayer);
	}
}

void UMyGameInstance::HandleSpawn(const Protocol::S_ENTER_GAME& enterGamePkt)
{
	HandleSpawn(enterGamePkt.playercharacter(), true);
}

void UMyGameInstance::HandleSpawn(const Protocol::S_SPAWN& spawnPkt)
{
	for (auto& actor : spawnPkt.actors())
	{
		HandleSpawn(actor, false);
	}
}

void UMyGameInstance::HandleDespawn(uint64 gameId)
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

	AMMOClientPlayer** player = PlayerMap.Find(gameId);
	if (player == nullptr)
	{
		return;
	}

	world->DestroyActor(*player);

	PlayerMap.Remove(gameId);
}

void UMyGameInstance::HandleDespawn(const Protocol::S_DESPAWN& despawnPkt)
{
	for (auto& gameId : despawnPkt.actorids())
	{
		HandleDespawn(gameId);
	}
}

void UMyGameInstance::HandleMove(const Protocol::S_MOVE& movePkt)
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

	const uint64 gameId = movePkt.actorinfo().gameid();

	AMMOClientPlayer** player = PlayerMap.Find(gameId);
	if (player == nullptr)
	{
		return;
	}
	
	if ((*player)->IsMyPlayer())
	{
		return;
	}

	//(*player)->SetActorInfo(&movePkt.actorinfo());
	(*player)->SetTargetActorInfo(&movePkt.actorinfo());
}
