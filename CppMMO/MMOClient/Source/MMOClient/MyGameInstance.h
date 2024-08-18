// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MMOClient.h"
#include "MyGameInstance.generated.h"

class AMMOClientPlayer;

/**
 * 
 */
UCLASS()
class MMOCLIENT_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	virtual void Init() override;

	bool CheckSocketConnection();

	UFUNCTION(BlueprintCallable)
	void ConnectToGameServer();

	UFUNCTION(BlueprintCallable)
	void DisconnectFromGameServer();

	UFUNCTION(BlueprintCallable)
	void HandleRecvPacket();

	void SendPacket(SendBufferRef SendBuf);

public:
	void HandleSpawn(const Protocol::ActorInfo& player, bool isMine);
	void HandleSpawn(const Protocol::S_ENTER_GAME& enterGamePkt);
	void HandleSpawn(const Protocol::S_SPAWN& spawnPkt);

	void HandleDespawn(uint64 gameId);
	void HandleDespawn(const Protocol::S_DESPAWN& despawnPkt);

	void HandleMove(const Protocol::S_MOVE& movePkt);


public:
	// 아래처럼 해줘야 블루 프린트에서 골라서 넣어줄 수 있다고 한다.
	UPROPERTY(EditAnywhere)
	TSubclassOf<AMMOClientPlayer> OtherPlayerClass;
	TMap<uint64, AMMOClientPlayer*> PlayerMap; // TODO: 몬스터 맵도 추가 필요할 듯?
	AMMOClientPlayer* MyPlayer;


public:
	// Network
	class FSocket* Socket;
	FString IpAddress = TEXT("127.0.0.1"); // TODO: 추후 게임 서버 입력받도록 변경 필요
	int16 Port = 7777;
	TSharedPtr<class PacketSession> GameServerSession;
};
