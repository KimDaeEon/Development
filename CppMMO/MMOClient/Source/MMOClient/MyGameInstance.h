// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MMOClient.h"
#include "MyGameInstance.generated.h"

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
	void HandleSpawn(const Protocol::ActorInfo& player);
	void HandleSpawn(const Protocol::S_ENTER_GAME& enterGamePkt);
	void HandleSpawn(const Protocol::S_SPAWN& spawnPkt);

	void HandleDespawn(uint64 gameId);
	void HandleDespawn(const Protocol::S_DESPAWN& despawnPkt);


public:
	// �Ʒ�ó�� ����� ��� ����Ʈ���� ��� �־��� �� �ִٰ� �Ѵ�.
	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> PlayerClass;
	TMap<uint64, AActor*> PlayerMap;

public:
	// Network
	class FSocket* Socket;
	FString IpAddress = TEXT("127.0.0.1"); // TODO: ���� ���� ���� �Է¹޵��� ���� �ʿ�
	int16 Port = 7777;
	TSharedPtr<class PacketSession> GameServerSession;
};
