// Fill out your copyright notice in the Description page of Project Settings.

#include "Game/MMOClientPlayer.h"
#include "MMOClient.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "MyMMOClientPlayer.h"

// Sets default values
AMMOClientPlayer::AMMOClientPlayer()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	ActorInfo = new Protocol::ActorInfo();
}

AMMOClientPlayer::~AMMOClientPlayer()
{
	delete ActorInfo;
	ActorInfo = nullptr;
}

void AMMOClientPlayer::SetActorInfo(const Protocol::ActorInfo* actorInfo)
{
	if (ActorInfo->gameid() != 0)
	{
		// TODO: 이 체크가 꼭 필요할지 나중에 고민해보자.
		assert(ActorInfo.gameid() == actorInfo.gameid());
	}
	ActorInfo->CopyFrom(*actorInfo);

	FVector location = FVector(actorInfo->transform().position().x(), actorInfo->transform().position().y(), actorInfo->transform().position().z());
	SetActorLocation(location);
	UE_LOG(LogTemp, Warning, TEXT("SetActorInfo: %s"), *location.ToString());
}

bool AMMOClientPlayer::IsMyPlayer() const
{
	return Cast<AMyMMOClientPlayer>(this) != nullptr;
}

void AMMOClientPlayer::BeginPlay()
{
	Super::BeginPlay();
}

void AMMOClientPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	{
		FVector location = GetActorLocation();
		auto* transfrom = ActorInfo->mutable_transform();
		auto* position = transfrom->mutable_position();
		auto* rotation = transfrom->mutable_rotation();

		position->set_x(location.X);
		position->set_y(location.Y);
		position->set_z(location.Z);

		rotation->set_x(GetActorRotation().Roll);
		rotation->set_y(GetActorRotation().Yaw);
		rotation->set_z(GetActorRotation().Pitch);
	}
}


