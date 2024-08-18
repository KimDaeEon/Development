// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Protocol.pb.h"
#include "MMOClientPlayer.generated.h"

class Protocol::ActorInfo;

UCLASS()
class MMOCLIENT_API AMMOClientPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	AMMOClientPlayer();
	virtual ~AMMOClientPlayer();

	void SetActorInfo(const Protocol::ActorInfo* actorInfo);
	Protocol::ActorInfo* GetActorInfo() const { return ActorInfo; };

	bool IsMyPlayer() const;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

protected:
	Protocol::ActorInfo* ActorInfo;
};
