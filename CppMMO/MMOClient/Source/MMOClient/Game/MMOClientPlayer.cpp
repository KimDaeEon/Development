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

	GetCharacterMovement()->bRunPhysicsWithNoController = true;

	ActorInfo = new Protocol::ActorInfo();
	TargetActorInfo = new Protocol::ActorInfo();
}

AMMOClientPlayer::~AMMOClientPlayer()
{
	delete ActorInfo;
	ActorInfo = nullptr;

	delete TargetActorInfo;
	TargetActorInfo = nullptr;
}

void AMMOClientPlayer::SetActorInfo(const Protocol::ActorInfo* actorInfo)
{
	if (ActorInfo->gameid() != 0)
	{
		// TODO: �� üũ�� �� �ʿ����� ���߿� ����غ���.
		assert(ActorInfo.gameid() == actorInfo.gameid());
	}
	ActorInfo->CopyFrom(*actorInfo);

	FVector location = FVector(actorInfo->transform().position().x(), actorInfo->transform().position().y(), actorInfo->transform().position().z());
	SetActorLocation(location);
	UE_LOG(LogTemp, Warning, TEXT("SetActorInfo: %s"), *location.ToString());
}

void AMMOClientPlayer::SetTargetActorInfo(const Protocol::ActorInfo* actorInfo)
{
	if (ActorInfo->gameid() != 0)
	{
		assert(ActorInfo.gameid() == actorInfo.gameid());
	}

	TargetActorInfo->CopyFrom(*actorInfo);

	// TODO: ���¸� �ٷ� �����ϴµ�.. �̰� ������ �ٽ� Ȯ�� �ʿ�.
	SetMoveState(actorInfo->movestate());
}

void AMMOClientPlayer::SetMoveState(Protocol::MoveState moveState)
{
	if (ActorInfo->movestate() == moveState)
	{
		return;
	}

	ActorInfo->set_movestate(moveState);

	// TODO: SetActorInfo ó�� ���� �߰��ؾ��� ���� ������?
}

bool AMMOClientPlayer::IsMyPlayer() const
{
	return Cast<AMyMMOClientPlayer>(this) != nullptr;
}

void AMMOClientPlayer::BeginPlay()
{
	Super::BeginPlay();
	
	{
		FVector location = GetActorLocation();

		auto* position = TargetActorInfo->mutable_transform()->mutable_position();
		auto* rotation = TargetActorInfo->mutable_transform()->mutable_rotation();

		position->set_x(location.X);
		position->set_y(location.Y);
		position->set_z(location.Z);

		rotation->set_x(GetActorRotation().Roll);
		rotation->set_y(GetActorRotation().Pitch);
		rotation->set_z(GetActorRotation().Yaw);

		SetMoveState(Protocol::MoveState::MOVE_STATE_IDLE);
	}
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


	if (IsMyPlayer() == false)
	{
		// TODO: �̰� ���� ��ǥ�� Rotation ������ �Ѿ�� �ͱ��� �߰� �ʿ�
		FVector location = GetActorLocation();

		// ��ġ �� smoothing
		auto* position = TargetActorInfo->mutable_transform()->mutable_position();
		FVector targetLocation = FVector(position->x(), position->y(), position->z());

		FVector direction = targetLocation - location;
		const float distToTarget = direction.Length();
		direction.Normalize();

		// TODO: �Ʒ� 600.0f �� �ٸ� ������ �޾Ƽ� ó���ϵ��� �ؾ���.
		float movingDist = (direction * 600.0f * DeltaTime).Length();
		movingDist = FMath::Min(movingDist, distToTarget);
		FVector nextLocation = location + direction * movingDist;

		SetActorLocation(nextLocation);


		// ȸ�� �� smoothing ó��
		FRotator rotation = GetActorRotation();
		auto* targetRotation = TargetActorInfo->mutable_transform()->mutable_rotation();

		FRotator targetRotator = FRotator(targetRotation->x(), targetRotation->y(), targetRotation->z());
		FRotator nextRotation = FMath::RInterpTo(rotation, targetRotator, DeltaTime, 5.0f);
		SetActorRotation(nextRotation);

		if (ActorInfo->movestate() == Protocol::MoveState::MOVE_STATE_MOVE)
		{
			AddMovementInput(GetActorForwardVector());
		}
	}
}


