// Fill out your copyright notice in the Description page of Project Settings.

// Ŀ���� Ŭ���� ����
#include "ShooterCharacter.h"
#include "ShooterPlayerController.h"

// ������Ʈ Ŭ���� ����.
#include "Camera/CameraComponent.h"

// ���� �Է� ����.
#include "EnhancedInputComponent.h"

/*
* Terminator Re-Rampage Project
* 2023. 08. 29
* �÷��̾ �����ϴ� ĳ���� Ŭ������ CPP
*/

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

	// Ŭ������ Controller�� Yaw ȸ���� ����ϵ��� ����.
	this->bUseControllerRotationPitch = false;
	this->bUseControllerRotationYaw = true;
	this->bUseControllerRotationRoll = false;
}

/** ���� ���� ó�� */
// Called when the game starts or when spawned
void AShooterCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}


/** ĳ���� ���� �Է� ó�� */
// �÷��̾��� �Է¿� ���� ĳ������ �̵� ������ ����.
void AShooterCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		/*
		// ��Ʈ�ѷ��� ȸ�� ��, Yaw ȸ���� ����� ���ο� ȸ���ڸ� ����.
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// ���� ���͸� ���Ѵ�.
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// ���� ���͸� ���Ѵ�.
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		*/

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
void AShooterCharacter::Jump()
{
	UE_LOG(LogTemp, Warning, TEXT("Jump"));
}

// �÷��̾��� �Է¿� ���� ��ũ���� ����/ ����
void AShooterCharacter::Crouch()
{
	UE_LOG(LogTemp, Warning, TEXT("Crouch"));
}

// �÷��̾��� �Է� ������ ���� ���� ����/ ����.
void AShooterCharacter::Sprint()
{
	UE_LOG(LogTemp, Warning, TEXT("Sprint"));
}


/** ���� ������Ʈ ó�� */
// Called every frame
void AShooterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


// Called to bind functionality to input
void AShooterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	
	// Shooter Player Controller�� �����ϱ� ���� Controller�� ����ȯ.
	AShooterPlayerController* PlayerController = Cast<AShooterPlayerController>(GetController());
	if (PlayerController)
	{
		// Shooter Player Controller�� ��ȿ�ϸ�, PlayerInputComponent�� ���� �Է� ������Ʈ�� ����ȯ.

		UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
		if (EnhancedInputComponent)
		{
			// ���� �Է� ������Ʈ�� ��ȿ�ϸ�,
			EnhancedInputComponent->BindAction(
				PlayerController->GetMoveAction(), 
				ETriggerEvent::Triggered, 
				this, 
				&AShooterCharacter::Move);

			EnhancedInputComponent->BindAction(
				PlayerController->GetLookupAction(), 
				ETriggerEvent::Triggered, 
				this, 
				&AShooterCharacter::Lookup);

			EnhancedInputComponent->BindAction(
				PlayerController->GetJumpAction(),
				ETriggerEvent::Triggered,
				this,
				&AShooterCharacter::Jump);

			EnhancedInputComponent->BindAction(
				PlayerController->GetCrouchAction(),
				ETriggerEvent::Triggered,
				this,
				&AShooterCharacter::Crouch);

			EnhancedInputComponent->BindAction(
				PlayerController->GetSprintAction(),
				ETriggerEvent::Ongoing,
				this,
				&AShooterCharacter::Sprint);
		}
	}
}

