// Fill out your copyright notice in the Description page of Project Settings.

// Ŀ���� Ŭ���� ����
#include "ShooterCharacter.h"
#include "ShooterItem.h"
#include "ShooterWeapon.h"
#include "ShooterPlayerController.h"

// ������Ʈ Ŭ���� ����.
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"

// ���� �����ӿ��� ����.
#include "GameFramework/CharacterMovementComponent.h"

// ��ƿ��Ƽ ����.
#include "Curves/CurveFloat.h"
#include "Math/UnrealMathUtility.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

// ���� �Է� ����.
#include "EnhancedInputComponent.h"

/*
 * Terminator Re-Rampage Project
 * 2023. 08. 29
 * �÷��̾ �����ϴ� ĳ���� Ŭ������ CPP
*/

#pragma region Constructor

// Sets default values
AShooterCharacter::AShooterCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	/** ������Ʈ ����Ŭ���� ���� */
	// �÷��� ī�޶� ����Ŭ���� ����.
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

	// 1��Ī ���̷�Ż �Ž� ����Ŭ���� ����.
	ShooterMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("First Person Body"));
	if (ShooterMesh)
	{
		// Shooter Mesh�� ��ȿ�ϸ�, Shooter Camera ������ ���.
		// Shooter Mesh�� ��ġ(Lcoation)�� ����.
		ShooterMesh->SetupAttachment(ShooterCamera);
		ShooterMesh->SetRelativeLocation(FVector(0.0f, -10.0f, -165.0f));
	}

	/** Ŭ���� �Ķ���� ���� */
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

	// ĳ������ �ִ� �̵� �ӵ� ����. (Sprint ���°� �����Ǹ�, ���� �뵵�� ���)
	MaxMoveSpeed = GetCharacterMovement()->MaxWalkSpeed;
}
#pragma endregion Constructor

/** ���� ���� ó�� */
// Called when the game starts or when spawned
void AShooterCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Shooter Player Controller�� �����ϱ� ���� Controller�� ����ȯ.
	PlayerController = Cast<AShooterPlayerController>(GetController());
}

#pragma region CharacterActions

/** ĳ���� �׼� ó�� */
// Crouch ���¸� ó�� (�����Ӹ��� üũ�ϱ� ���� Tick()���� ȣ��)
void AShooterCharacter::ExecuteCrouch(float DeltaTime)
{
	if (ActionState == ECharacterActionState::ECAS_CROUCH)
	{
		// ĳ���� ���°� Crouch���
		// ���� ĳ������ Z�� ���̺��� CharacterCrouchHeight����
		// ���� �ð� ���� �� �����Ӹ��� ���� ���� ���Ѵ�.
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
		// ���� �ð� ���� �� �����Ӹ��� ���� ���� ���Ѵ�.
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

// ĳ������ ���� ���¿� ���� ���¹̳��� ������ ���� ó��. (�����Ӹ��� üũ�ϱ� ���� Tick()���� ȣ��)
void AShooterCharacter::ExecuteStamina(float DeltaTime)
{
	// ĳ������ ���°� Sprint���	
	// ���¹̳� �ѷ��� 0������ ���, ���¹̳ʸ� 0���� ����. (0���Ϸ� ���� ����.
	// ���¹̳� �ѷ��� 0������ ���, ���� �׼� ����.
	// ���¹̳� �ѷ��� 0�̻��� ���, ���¹̳��� �ѷ����� ���ҷ���ŭ ����
	if (ActionState == ECharacterActionState::ECAS_SPRINT)
	{				
		if (Stamina <= 0)
		{
			RestoreStaminaCooldown = 2.0f;
			Stamina = 0.0f;
			EndSprint();
		}
		else
		{
			RestoreStaminaCooldown = 0.0f;
			Stamina -= ReductionStamina;
		}
	}

	// ĳ������ ���°� Sprint�� �ƴ϶��, ȸ�� ���� ��� �ð��� 0���� üũ�ϰ�, 0�� ���.
	// ���¹̳ʰ� 100�̻��� �� ���, ���¹̳ʸ� 100���� ���� (100�̻� ���� ����)
	// ���¹̳ʰ� 100���� ���� ���, ���� ���¹̳��� �翡 ȸ������ŭ �ջ�.	
	// ȸ�� ��� �ð��� 0�� �ƴϸ�, ȸ�� ��� �ð��� ����.
	if (ActionState != ECharacterActionState::ECAS_SPRINT)
	{
		if (RestoreStaminaCooldown <= 0.0f)
		{			
			if (Stamina >= 100.0f)
			{
				Stamina = 100.0f;
			}
			else
			{
				Stamina += RestoreStamina;
			}
		}
		else
		{
			RestoreStaminaCooldown -= UGameplayStatics::GetWorldDeltaSeconds(GetWorld());
		}
	}
}

#pragma endregion CharacterActions


#pragma region Item
// �������� Ÿ���� ���ؼ�, Ÿ�� ����, ���� ������ Ŭ������ Pick Up �Լ��� ȣ��
// �� �Լ��� ������ Ŭ������ ���ؼ� ȣ��.
void AShooterCharacter::PickUpItem(AShooterItem* Item)
{
	// ���� ȹ�� �Ϸ� ��, ���� ���� �� ��ġ�� �ν��Ͻ��� �����ϱ� ����, �ӽ÷� ����.
	ItemTemp = Item;
	
	switch (Item->GetItemType())
	{
	case EItemType::EIT_WEAPON:
	{
		// Item�� Weapon Base Class�� ĳ����.
		// Weapon Ŭ���� ������ �̿��� Pick Up Weapon �Լ� ȣ��.
		AShooterWeapon* Weapon = Cast<AShooterWeapon>(Item);
		if (Weapon)
		{
			UE_LOG(LogTemp, Warning, TEXT("Pick Up Weapon is: %s"), *Weapon->GetName());
			Weapon->PickUpWeapon(this);			
		}		
		break;
	}

	case EItemType::EIT_AMMO:
	{
		// Item�� Ammo Base Class�� ĳ����.
		// Ammo Ŭ���� ������ �̿��� Pick Up Ammo �Լ� ȣ��.
		
		UE_LOG(LogTemp, Warning, TEXT("Pick Up Ammo"));
		Item->SetCompletedPickUpItem(true);
		break;
	}

	case EItemType::EIT_KIT:
		// Item�� Kit Base Class�� ĳ����.
		// Kit Ŭ���� ������ �̿��� Pick Up Kit �Լ� ȣ��.

		UE_LOG(LogTemp, Warning, TEXT("Pick Up Kit"));
		Item->SetCompletedPickUpItem(true);
		break;

	case EItemType::EIT_SUIT:
		// Item�� Suit Class�� ĳ����.
		// Suit Ŭ���� ������ �̿��� Pick Up Suit �Լ� ȣ��.

		UE_LOG(LogTemp, Warning, TEXT("Pick Up Suit"));
		Item->SetCompletedPickUpItem(true);
		break;

	case EItemType::EIT_PART:
		// Item�� Part Base Class�� ĳ����.
		// Parts Ŭ���� ������ �̿��� Pick Up Part �Լ� ȣ��.

		UE_LOG(LogTemp, Warning, TEXT("Pick Up Part"));
		Item->SetCompletedPickUpItem(true);
		break;

	case EItemType::EIT_NONE:
		// �������� �Ⱦ����� �ʴ´�.

		UE_LOG(LogTemp, Warning, TEXT("Pick Up Nothing"));
		Item->SetCompletedPickUpItem(true);
		break;
	}	
}

// ���⸦ ȹ������ ��, ������ ���� ������ ����ִٸ� ȣ��. (ShooterWeapon Ŭ��������)
void AShooterCharacter::AddWeaponToSlot(AShooterWeapon* Weapon)
{
	UE_LOG(LogTemp, Warning, TEXT("Add Weapon To Slot"));
	// ������ Property ��, Slot Type���� ���Ⱑ ������ ������ ����.
	// Slot Type�� ������ ���� ������ ��� ������, ���⸦ ���.
	// ����� �Ϸ�Ǹ�, ������ ������ �ӽ� �ν��Ͻ��� ����.
	switch (Weapon->GetWeaponProperty().SlotType)
	{
	case ESlotType::ESLT_Primary:
	{
		if (PrimaryWeaponSlot == nullptr)
		{			
			PrimaryWeaponSlot = Weapon;
			UE_LOG(LogTemp, Warning, TEXT("Primary Weapon Add: %s"), *PrimaryWeaponSlot->GetName());
			ItemTemp->SetCompletedPickUpItem(true);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Has Aleady Weapon"));
		}
		break;
	}
	case ESlotType::ESLT_Secondary:
	{
		if (SecondaryWeaponSlot == nullptr)
		{
			SecondaryWeaponSlot = Weapon;
			UE_LOG(LogTemp, Warning, TEXT("Secondary Weapon Add: %s"), *SecondaryWeaponSlot->GetName());
			ItemTemp->SetCompletedPickUpItem(true);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Has Aleady Weapon"));
		}
		break;
	}
	case ESlotType::ESLT_Special:
	{
		if (SpecialWeaponSlot == nullptr)
		{
			SpecialWeaponSlot = Weapon;
			UE_LOG(LogTemp, Warning, TEXT("Special Weapon Add: %s"), *SpecialWeaponSlot->GetName());
			ItemTemp->SetCompletedPickUpItem(true);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Has Aleady Weapon"));
		}
		break;
	}
	case ESlotType::ESLT_Special2:
	{
		if (Special2WeaponSlot == nullptr)
		{
			Special2WeaponSlot = Weapon;
			UE_LOG(LogTemp, Warning, TEXT("Special2 Weapon Add: %s"), *Special2WeaponSlot->GetName());
			ItemTemp->SetCompletedPickUpItem(true);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Has Aleady Weapon"));
		}
		break;
	}
	case ESlotType::ESLT_Special3:
	{
		if (Special3WeaponSlot == nullptr)
		{
			Special3WeaponSlot = Weapon;
			UE_LOG(LogTemp, Warning, TEXT("Special3 Weapon Add: %s"), *Special3WeaponSlot->GetName());
			ItemTemp->SetCompletedPickUpItem(true);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Has Aleady Weapon"));
		}
		break;
	}
	case ESlotType::ESLT_Special4:
	{
		if (Special4WeaponSlot == nullptr)
		{
			Special4WeaponSlot = Weapon;
			UE_LOG(LogTemp, Warning, TEXT("Special4 Weapon Add: %s"), *Special4WeaponSlot->GetName());
			ItemTemp->SetCompletedPickUpItem(true);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Has Aleady Weapon"));
		}
		break;
	}

	}
}

#pragma endregion

#pragma region Inputs
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
	// ĳ���Ͱ� Jump ���°� �ƴ϶��,�׼� ���¸� JUMP�� �����ϰ� ������ �����Ѵ�.
	if (ActionState != ECharacterActionState::ECAS_JUMP)
	{
		ActionState = ECharacterActionState::ECAS_JUMP;
		Jump();
	}
}

void AShooterCharacter::Landing()
{
	// ĳ������ �׼� ���°� JUMP���, ������ �����Ѵ�.
	if (ActionState == ECharacterActionState::ECAS_JUMP)
	{
		StopJumping();	
	}	
}

void AShooterCharacter::EndJump()
{	
	// ĳ���Ͱ� ���� �� ���鿡 �����ϸ�, �׼� ���¸� IDLE�� �����Ѵ�.
	ActionState = ECharacterActionState::ECAS_IDLE;	
}

// ��ũ���� �Է¿� ���� �׼� ���¸� ��ȯ.
void AShooterCharacter::ChangeActionByCrouch()
{
	// ĳ���Ͱ� ��ũ���� ���°� �ƴϰ�, ���� ���°� �ƴϸ� ��ũ���� ����.
	if (ActionState != ECharacterActionState::ECAS_CROUCH &&
		ActionState != ECharacterActionState::ECAS_JUMP)
	{
		// ĳ������ �׼� ���¸� CROUCH�� ����.
		ActionState = ECharacterActionState::ECAS_CROUCH;

		// ĳ������ �ִ� �̵� �ӵ� ����.
		GetCharacterMovement()->MaxWalkSpeed = MaxMoveSpeed * 0.5f;
	
		// ĳ���� ĸ�� ������Ʈ�� ũ�⸦ ���δ�.
		GetCapsuleComponent()->SetCapsuleHalfHeight(GetCapsuleComponent()->GetScaledCapsuleHalfHeight() * 0.5f);
	}
	else
	{
		// ĳ���Ͱ� ��ũ���� ������ ���, ��� ���·� ��ȯ.
		ActionState = ECharacterActionState::ECAS_IDLE;		
	
		// ĳ������ �ִ� �̵� �ӵ� ����
		GetCharacterMovement()->MaxWalkSpeed = MaxMoveSpeed;

		// ĳ���� ĸ�� ������Ʈ�� ũ�⸦ ����.
		GetCapsuleComponent()->SetCapsuleHalfHeight(CapsuleHalfHeight);
	}	
}

// ���� �Է� ������ ���� �׼� ����.
void AShooterCharacter::BeginSprint()
{
	if (ActionState == ECharacterActionState::ECAS_JUMP ||
		ActionState == ECharacterActionState::ECAS_CROUCH)
	{
		// ĳ������ �׼� ���°� JUMP �Ǵ� LAND �Ǵ� CROUCH��� ���� ��.
		return;
	}

	if (Stamina <= 0.0f)
	{
		// ���¹̳ʰ� 0�̶�� ���� �Ұ�.
		return;
	}
	
	// ĳ������ �׼� ���¸� SPRINT�� ����.
	ActionState = ECharacterActionState::ECAS_SPRINT;

	// ĳ������ �ִ� �̵� �ӵ� ����.
	GetCharacterMovement()->MaxWalkSpeed = MaxMoveSpeed * 1.75f;
}

// ���� �Է� ������ ���� �׼� ����.
void AShooterCharacter::EndSprint()
{
	if (ActionState == ECharacterActionState::ECAS_SPRINT)
	{
		// ĳ������ �׼� ���¸� IDLE�� ����.
		ActionState = ECharacterActionState::ECAS_IDLE;

		// ĳ������ �ִ� �̵� �ӵ� ����.
		GetCharacterMovement()->MaxWalkSpeed = MaxMoveSpeed;
	}	
}
#pragma endregion Inputs


/** ���� ������Ʈ ó�� */
// Called every frame
void AShooterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// ���� ���� ���ο� ���� ���¹̳� ����.
	ExecuteStamina(DeltaTime);

	// ������ ����� ī�޶��� ���̸� ����.
	ExecuteCrouch(DeltaTime);

	// ĳ���Ͱ� ���� �� ���鿡 �����ϸ�, �׼� ���¸� ����.
	if (ActionState == ECharacterActionState::ECAS_JUMP && !GetCharacterMovement()->IsFalling())
	{
		EndJump();
	}
}


/** ���� �Է� ���ε�*/
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
				&AShooterCharacter::Landing);			

			// ��ũ���� �׼� �Է�
			EnhancedInputComponent->BindAction(
				ShooterController->GetCrouchAction(),
				ETriggerEvent::Triggered,
				this,
				&AShooterCharacter::ChangeActionByCrouch);

			// ���� �׼� �Է�
			EnhancedInputComponent->BindAction(
				ShooterController->GetSprintAction(),
				ETriggerEvent::Ongoing,
				this,
				&AShooterCharacter::BeginSprint);

			EnhancedInputComponent->BindAction(
				ShooterController->GetSprintAction(),
				ETriggerEvent::Completed,
				this,
				&AShooterCharacter::EndSprint);
		}
	}
}


