// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/MyMMOClientPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "MMOClient.h"
#include <Kismet/KismetMathLibrary.h>

AMyMMOClientPlayer::AMyMMOClientPlayer()
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

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

void AMyMMOClientPlayer::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// Input

void AMyMMOClientPlayer::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{

		//Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMyMMOClientPlayer::Move);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &AMyMMOClientPlayer::Move);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMyMMOClientPlayer::Look);

	}

}


void AMyMMOClientPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	// 갑작스럽게 Send를 해야할 때가 있다. ex) 방향 전환, 점프 등..
	bool SendImmediately = false;

	if (LastInputMovementVector != DesiredInputMovementVector)
	{
		SendImmediately = true;
		LastInputMovementVector = DesiredInputMovementVector;
	}

	// State 변경
	if (DesiredInputMovementVector.IsZero())
	{
		SetMoveState(Protocol::MoveState::MOVE_STATE_IDLE);
	}
	else
	{
		SetMoveState(Protocol::MoveState::MOVE_STATE_MOVE);
	}

	MovePacketSendTimer -= DeltaTime;
	if (MovePacketSendTimer <= 0.0f || SendImmediately)
	{
		MovePacketSendTimer = MOVE_PACKET_SEND_INTERVAL;
		// TODO: Send Move Packet

		Protocol::C_MOVE movePkt;
		auto* myGameInstance = Cast<UMyGameInstance>(GetGameInstance());

		// 움직임 정보 설정
		if (myGameInstance)
		{
			Protocol::ActorInfo* actorInfo = movePkt.mutable_actorinfo();
			actorInfo->CopyFrom(*ActorInfo);

			auto session = myGameInstance->GameServerSession;
			if (session)
			{
				session->SendPacket(movePkt);
			}
		}
		
	}
}


void AMyMMOClientPlayer::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D inputMovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation(); // 이 부분이 카메라 방향인 것이 중요하다. ActorRotation을 안쓰는 점을 잘 염두해서 보자.
		//const FRotator Rotation = GetActorRotation(); <- 이렇게 하면 안됨
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, inputMovementVector.Y);
		AddMovementInput(RightDirection, inputMovementVector.X);

		{
			DesiredInputMovementVector = inputMovementVector;

			DesiredMoveDirection = FVector::ZeroVector;
			DesiredMoveDirection += ForwardDirection * inputMovementVector.Y;
			DesiredMoveDirection += RightDirection * inputMovementVector.X;
			DesiredMoveDirection.Normalize();

			const FVector location = GetActorLocation();
			FRotator rotator = UKismetMathLibrary::FindLookAtRotation(location, location + DesiredMoveDirection);
			DesiredYaw = rotator.Yaw;
		}
	}
}

void AMyMMOClientPlayer::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}



