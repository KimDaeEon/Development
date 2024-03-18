// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MyGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class MMOCLIENT_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void ConnectToGameServer();

	UFUNCTION(BlueprintCallable)
	void DisconnectFromGameServer();

public:
	// Network
	class FSocket* Socket;
	FString IpAddress = TEXT("127.0.0.1"); // TODO: ���� ���� ���� �Է¹޵��� ���� �ʿ�
	int16 Port = 7777;
	
};
