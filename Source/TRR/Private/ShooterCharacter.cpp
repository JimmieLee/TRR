// Fill out your copyright notice in the Description page of Project Settings.

// Ŀ���� Ŭ���� ����
#include "ShooterCharacter.h"
#include "ShooterPlayerController.h"

// ������Ʈ Ŭ���� ����.
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"

// ���� �����ӿ��� ����.
#include "GameFramework/CharacterMovementComponent.h"

// ��ƿ��Ƽ ����.
#include "Curves/CurveFloat.h"
#include "Math/UnrealMathUtility.h"

// ���� �Է� ����.
#include "EnhancedInputComponent.h"

/*
 * Terminator Re-Rampage Project
 * 2023. 08. 29
 * �÷��̾ �����ϴ� ĳ���� Ŭ������ CPP
*/

/** Ŭ���� ������ */
// Sets default values
AShooterCharacter::AShooterCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	/** ������Ʈ ����Ŭ���� ���� */
	// �÷��� ī�޶� ����Ŭ���� ����. (���� �� �������Ʈ�� ����)
	ShooterCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Player Camera"));
	if (ShooterCamera)
	{
		// Shooter Camera�� ��ȿ�ϸ�, Root Component ������ ���.
		// Shooter Camera�� ��ġ(Location)�� ����.
		// Shooter Camera�� ���� ȸ�� ���� ���.
		ShooterCamera->SetupAttachment(GetRootComponent());
		ShooterCamera->SetRelativeLocation(FVector(0.0f, 0.0f, 50.0f));
		ShooterCamera->bUsePawnControlRotation = true;
	}

	// 1��Ī ���̷�Ż �Ž� ����Ŭ���� ����. (���� �� �������Ʈ�� ����)
	ShooterMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("First Person Body"));
	if (ShooterMesh)
	{
		// Shooter Mesh�� ��ȿ�ϸ�, Shooter Camera ������ ���.
		// Shooter Mesh�� ��ġ(Lcoation)�� ����.
		ShooterMesh->SetupAttachment(ShooterCamera);
		ShooterMesh->SetRelativeLocation(FVector(-10.0f, -10.0f, -150.0f));
	}

	/** ĳ���� �Ķ���� ���� */
	// ĳ���Ͱ� Controller�� Yaw ȸ���� ����ϵ��� ����.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;	

	// ĳ���Ͱ� Crouch�� ����� �� �ֵ��� ����.
	GetCharacterMovement()->bWantsToCrouch = true;

	// ĸ�� ������Ʈ�� �⺻ ���� ����.(Crouch ���°� �����Ǹ�, ���� �뵵�� ���)
	CapsuleHalfHeight = GetCapsuleComponent()->GetScaledCapsuleHalfHeight();

	// ĳ������ �⺻ ��ġ ����. (Crouch ���°� �����Ǹ�, ���� �뵵�� ���)
	ShooterCameraPosition = ShooterCamera->GetRelativeLocation();
}

/** ���� ���� ó�� */
// Called when the game starts or when spawned
void AShooterCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Shooter Player Controller�� �����ϱ� ���� Controller�� ����ȯ.
	PlayerController = Cast<AShooterPlayerController>(GetController());
}

/** ĳ���� �׼� ó�� */
// Crouch ���¸� ó�� (������ �˻�)
void AShooterCharacter::ExecuteCrouch(float DeltaTime)
{
	if (ActionState == ECharacterActionState::ECAS_CROUCH)
	{
		// ĳ���� ���°� Crouch���
		// ���� ĳ������ Z�� ���̺��� CharacterCrouchHeight����
		// 0.25�ʰ� �� �����Ӹ��� ���� ���� ���Ѵ�.
		float InterpingCrouchHeight = FMath::FInterpTo(
			ShooterCamera->GetRelativeLocation().Z,
			CrouhedCameraHeight,
			DeltaTime, 
			5.0f);

		// �� �����Ӹ��� ������ ���� ���� ĳ������ Z�� ���̿� �ݿ�.
		ShooterCamera->SetRelativeLocation(
			FVector(ShooterCameraPosition.X, 
				ShooterCameraPosition.Y,
				InterpingCrouchHeight));
	}
	else
	{
		// ĳ���� ���°� Crouch���
		// ���� ĳ������ Z�� ���̺��� Character Stand Position�� Z�� ��ġ����
		// 0.25�ʰ� �� �����Ӹ��� ���� ���� ���Ѵ�.
		float InterpingCrouchHeight = FMath::FInterpTo(
			ShooterCamera->GetRelativeLocation().Z,
			ShooterCameraPosition.Z,
			DeltaTime,
			5.0f);

		// �� �����Ӹ��� ������ ���� ���� ĳ������ Z�� ���̿� �ݿ�.
		ShooterCamera->SetRelativeLocation(
			FVector(ShooterCameraPosition.X,
				ShooterCameraPosition.Y,
				InterpingCrouchHeight));
	}
}


/** ĳ���� ���� �Է� ó�� */
// �÷��̾��� �Է¿� ���� ĳ������ �̵� ������ ����.
void AShooterCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}	
}

// �÷��̾��� �Է¿� ���� ī�޶��� ������ ����.
void AShooterCharacter::Lookup(const FInputActionValue& Value)
{
	FVector2D LookupVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddControllerYawInput(-LookupVector.X);
		AddControllerPitchInput(LookupVector.Y);
	}
}

// �÷��̾��� �Է¿� ���� ���� ����.
void AShooterCharacter::BeginJump()
{
	// ĳ���Ͱ� Jump ���°� �ƴϰ�, ���߿� �� ���� �ʴٸ�,
	// �׼� ���¸� JUMP�� �����ϰ� ������ �����Ѵ�.
	if (ActionState != ECharacterActionState::ECAS_JUMP && !GetCharacterMovement()->IsFalling())
	{
		ActionState = ECharacterActionState::ECAS_JUMP;
		Jump();
	}
}

void AShooterCharacter::EndJump()
{
	// ������ �����ϰ�, �׼� ���¸� IDLE�� �����Ѵ�.	
	StopJumping();
	ActionState = ECharacterActionState::ECAS_IDLE;
}

// ��ũ���� �Է¿� ���� �׼� ���¸� ��ȯ.
void AShooterCharacter::ChangeActionByCrouch()
{
	// ĳ���Ͱ� ��ũ���� ���°� �ƴϰ�, ���� ���°� �ƴϾ�� ��ũ���� ����.
	if (ActionState != ECharacterActionState::ECAS_CROUCH &&
		ActionState != ECharacterActionState::ECAS_JUMP)
	{
		// ĳ������ �׼� ���¸� CROUCH�� ����.
		ActionState = ECharacterActionState::ECAS_CROUCH;

		// ĳ���� ĸ�� ������Ʈ�� ũ�⸦ ���δ�.
		GetCapsuleComponent()->SetCapsuleHalfHeight(GetCapsuleComponent()->GetScaledCapsuleHalfHeight() * 0.5f);
	}
	else
	{
		// ĳ���� ĸ�� ������Ʈ�� ũ�⸦ ����.
		GetCapsuleComponent()->SetCapsuleHalfHeight(CapsuleHalfHeight);

		// ĳ���Ͱ� ��ũ���� ������ ���, ��� ���·� ��ȯ.
		ActionState = ECharacterActionState::ECAS_IDLE;
	}	
}

// �÷��̾��� �Է� ������ ���� ���� ����/ ����.
void AShooterCharacter::Sprint()
{
	// ĳ������ �׼� ���¸� SPRINT�� ����.
	ActionState = ECharacterActionState::ECAS_SPRINT;
	
	UE_LOG(LogTemp, Warning, TEXT("Sprint"));
}


/** ���� ������Ʈ ó�� */
// Called every frame
void AShooterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// ������ ����� ī�޶��� ���̸� ����.
	ExecuteCrouch(DeltaTime);
}

// Called to bind functionality to input
void AShooterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);	

	// Shooter Player Controller�� �����ϱ� ���� Controller�� ����ȯ.
	AShooterPlayerController* ShooterController = Cast<AShooterPlayerController>(GetController());
	
	if (ShooterController)
	{
		// �÷��̾� �Է� ������Ʈ�� ���� �Է� ������Ʈ�� ����ȯ�Ͽ�, ���� �Է� �׼��� ���ε��Ѵ�.
		UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
		if (EnhancedInputComponent)		
		{
			// �̵� �Է�
			EnhancedInputComponent->BindAction(
				ShooterController->GetMoveAction(),
				ETriggerEvent::Triggered, 
				this, 
				&AShooterCharacter::Move);
			
			// �ü� �Է�
			EnhancedInputComponent->BindAction(
				ShooterController->GetLookupAction(),
				ETriggerEvent::Triggered, 
				this, 
				&AShooterCharacter::Lookup);

			// Jump �׼� �Է�
			EnhancedInputComponent->BindAction(
				ShooterController->GetJumpAction(),
				ETriggerEvent::Triggered,
				this,
				&AShooterCharacter::BeginJump);
			
			EnhancedInputComponent->BindAction(
				ShooterController->GetJumpAction(),
				ETriggerEvent::Completed,
				this,
				&AShooterCharacter::EndJump);			

			// ��ũ���� �׼� �Է�
			EnhancedInputComponent->BindAction(
				ShooterController->GetCrouchAction(),
				ETriggerEvent::Triggered,
				this,
				&AShooterCharacter::Crouching);

			// ���� �׼� �Է�
			EnhancedInputComponent->BindAction(
				ShooterController->GetSprintAction(),
				ETriggerEvent::Ongoing,
				this,
				&AShooterCharacter::Sprint);
		}
	}
}

