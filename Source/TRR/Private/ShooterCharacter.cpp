// Fill out your copyright notice in the Description page of Project Settings.

// 커스텀 클래스 포함
#include "ShooterCharacter.h"
#include "ShooterItem.h"
#include "ShooterWeapon.h"
#include "ShooterPlayerController.h"

// 컴포넌트 클래스 포함.
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"

// 게임 프레임웍스 포함.
#include "GameFramework/CharacterMovementComponent.h"

// 유틸리티 포함.
#include "Curves/CurveFloat.h"
#include "Math/UnrealMathUtility.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

// 향상된 입력 포함.
#include "EnhancedInputComponent.h"

/*
 * Terminator Re-Rampage Project
 * 2023. 08. 29
 * 플레이어가 제어하는 캐릭터 클래스의 CPP
*/

#pragma region Constructor

// Sets default values
AShooterCharacter::AShooterCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	/** 컴포넌트 서브클래스 생성 */
	// 플레이 카메라 서브클래스 생성.
	ShooterCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Player Camera"));
	if (ShooterCamera)
	{
		// Shooter Camera가 유효하면, Root Component 하위에 등록.
		// Shooter Camera의 위치(Location)을 조정.
		// Shooter Camera의 폰의 회전 제어 사용.
		ShooterCamera->SetupAttachment(GetRootComponent());
		ShooterCamera->SetRelativeLocation(FVector(0.0f, 0.0f, 50.0f));
		ShooterCamera->bUsePawnControlRotation = true;
	}

	// 1인칭 스켈렉탈 매시 서브클래스 생성.
	ShooterMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("First Person Body"));
	if (ShooterMesh)
	{
		// Shooter Mesh가 유효하면, Shooter Camera 하위에 등록.
		// Shooter Mesh의 위치(Lcoation)을 조정.
		ShooterMesh->SetupAttachment(ShooterCamera);
		ShooterMesh->SetRelativeLocation(FVector(0.0f, -10.0f, -165.0f));
	}

	/** 클래스 파라미터 설정 */
	// 캐릭터가 Controller의 Yaw 회전을 사용하도록 설정.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;	

	// 캐릭터가 Crouch를 사용할 수 있도록 설정.
	GetCharacterMovement()->bWantsToCrouch = true;

	// 캡슐 컴포넌트의 기본 높이 저장.(Crouch 상태가 해제되면, 복원 용도로 사용)
	CapsuleHalfHeight = GetCapsuleComponent()->GetScaledCapsuleHalfHeight();

	// 캐릭터의 기본 위치 저장. (Crouch 상태가 해제되면, 복원 용도로 사용)
	ShooterCameraPosition = ShooterCamera->GetRelativeLocation();

	// 캐릭터의 최대 이동 속도 저장. (Sprint 상태가 해제되면, 복원 용도로 사용)
	MaxMoveSpeed = GetCharacterMovement()->MaxWalkSpeed;
}
#pragma endregion Constructor

/** 게임 시작 처리 */
// Called when the game starts or when spawned
void AShooterCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Shooter Player Controller에 접근하기 위해 Controller를 형변환.
	PlayerController = Cast<AShooterPlayerController>(GetController());
}

#pragma region CharacterActions

/** 캐릭터 액션 처리 */
// Crouch 상태를 처리 (프레임마다 체크하기 위해 Tick()에서 호출)
void AShooterCharacter::ExecuteCrouch(float DeltaTime)
{
	if (ActionState == ECharacterActionState::ECAS_CROUCH)
	{
		// 캐릭터 상태가 Crouch라면
		// 현재 캐릭터의 Z축 높이부터 CharacterCrouchHeight까지
		// 일정 시간 동안 매 프레임마다 높이 값을 구한다.
		float InterpingCrouchHeight = FMath::FInterpTo(
			ShooterCamera->GetRelativeLocation().Z,
			CrouhedCameraHeight,
			DeltaTime, 
			5.0f);

		// 매 프레임마다 구해진 높이 값을 캐릭터의 Z축 높이에 반영.
		ShooterCamera->SetRelativeLocation(
			FVector(ShooterCameraPosition.X, 
				ShooterCameraPosition.Y,
				InterpingCrouchHeight));
	}
	else
	{
		// 캐릭터 상태가 Crouch라면
		// 현재 캐릭터의 Z축 높이부터 Character Stand Position의 Z축 위치까지
		// 일정 시간 동안 매 프레임마다 높이 값을 구한다.
		float InterpingCrouchHeight = FMath::FInterpTo(
			ShooterCamera->GetRelativeLocation().Z,
			ShooterCameraPosition.Z,
			DeltaTime,
			5.0f);

		// 매 프레임마다 구해진 높이 값을 캐릭터의 Z축 높이에 반영.
		ShooterCamera->SetRelativeLocation(
			FVector(ShooterCameraPosition.X,
				ShooterCameraPosition.Y,
				InterpingCrouchHeight));
	}
}

// 캐릭터의 질수 상태에 따른 스태미너의 증가와 감소 처리. (프레임마다 체크하기 위해 Tick()에서 호출)
void AShooterCharacter::ExecuteStamina(float DeltaTime)
{
	// 캐릭터의 상태가 Sprint라면	
	// 스태미너 총량이 0이하일 경우, 스태미너를 0으로 고정. (0이하로 감소 방지.
	// 스태미너 총량이 0이하일 경우, 질주 액션 종료.
	// 스태미너 총량이 0이상일 경우, 스태미너의 총량에서 감소량만큼 감산
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

	// 캐릭터의 상태가 Sprint가 아니라면, 회복 시작 대기 시간이 0인지 체크하고, 0이 라면.
	// 스태미너가 100이상이 될 경우, 스태미너를 100으로 고정 (100이상 증가 감지)
	// 스태미너가 100보다 작은 경우, 현재 스태미너의 양에 회복량만큼 합산.	
	// 회복 대기 시간이 0이 아니면, 회복 대기 시간을 감소.
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
// 아이템의 타입을 구해서, 타입 별로, 서브 아이템 클래스의 Pick Up 함수를 호출
// 이 함수는 아이템 클래스에 의해서 호출.
void AShooterCharacter::PickUpItem(AShooterItem* Item)
{
	// 무기 획득 완료 후, 게임 월드 상에 배치된 인스턴스를 제거하기 위해, 임시로 저장.
	ItemTemp = Item;
	
	switch (Item->GetItemType())
	{
	case EItemType::EIT_WEAPON:
	{
		// Item을 Weapon Base Class로 캐스팅.
		// Weapon 클래스 참조를 이용해 Pick Up Weapon 함수 호출.
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
		// Item을 Ammo Base Class로 캐스팅.
		// Ammo 클래스 참조를 이용해 Pick Up Ammo 함수 호출.
		
		UE_LOG(LogTemp, Warning, TEXT("Pick Up Ammo"));
		Item->SetCompletedPickUpItem(true);
		break;
	}

	case EItemType::EIT_KIT:
		// Item을 Kit Base Class로 캐스팅.
		// Kit 클래스 참조를 이용해 Pick Up Kit 함수 호출.

		UE_LOG(LogTemp, Warning, TEXT("Pick Up Kit"));
		Item->SetCompletedPickUpItem(true);
		break;

	case EItemType::EIT_SUIT:
		// Item을 Suit Class로 캐스팅.
		// Suit 클래스 참조를 이용해 Pick Up Suit 함수 호출.

		UE_LOG(LogTemp, Warning, TEXT("Pick Up Suit"));
		Item->SetCompletedPickUpItem(true);
		break;

	case EItemType::EIT_PART:
		// Item을 Part Base Class로 캐스팅.
		// Parts 클래스 참조를 이용해 Pick Up Part 함수 호출.

		UE_LOG(LogTemp, Warning, TEXT("Pick Up Part"));
		Item->SetCompletedPickUpItem(true);
		break;

	case EItemType::EIT_NONE:
		// 아이템을 픽업하지 않는다.

		UE_LOG(LogTemp, Warning, TEXT("Pick Up Nothing"));
		Item->SetCompletedPickUpItem(true);
		break;
	}	
}

// 무기를 획득했을 때, 무기의 보관 공간이 비어있다면 호출. (ShooterWeapon 클래스에서)
void AShooterCharacter::AddWeaponToSlot(AShooterWeapon* Weapon)
{
	UE_LOG(LogTemp, Warning, TEXT("Add Weapon To Slot"));
	// 무기의 Property 중, Slot Type으로 무기가 보관할 공간을 구분.
	// Slot Type에 적합한 무기 슬롯이 비어 있으면, 무기를 등록.
	// 등록이 완료되면, 저장한 아이템 임시 인스턴스를 제거.
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
/** 캐릭터 제어 입력 처리 */
// 플레이어의 입력에 따라 캐릭터의 이동 방향을 설정.
void AShooterCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}	
}

// 플레이어의 입력에 따라 카메라의 방향을 설정.
void AShooterCharacter::Lookup(const FInputActionValue& Value)
{
	FVector2D LookupVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddControllerYawInput(-LookupVector.X);
		AddControllerPitchInput(LookupVector.Y);
	}
}

// 플레이어의 입력에 의해 점프 실행.
void AShooterCharacter::BeginJump()
{
	// 캐릭터가 Jump 상태가 아니라면,액션 상태를 JUMP로 변경하고 점프를 실행한다.
	if (ActionState != ECharacterActionState::ECAS_JUMP)
	{
		ActionState = ECharacterActionState::ECAS_JUMP;
		Jump();
	}
}

void AShooterCharacter::Landing()
{
	// 캐릭터의 액션 상태가 JUMP라면, 점프를 중지한다.
	if (ActionState == ECharacterActionState::ECAS_JUMP)
	{
		StopJumping();	
	}	
}

void AShooterCharacter::EndJump()
{	
	// 캐릭터가 점프 후 지면에 도달하면, 액션 상태를 IDLE로 변경한다.
	ActionState = ECharacterActionState::ECAS_IDLE;	
}

// 웅크리기 입력에 따른 액션 상태를 전환.
void AShooterCharacter::ChangeActionByCrouch()
{
	// 캐릭터가 웅크리기 상태가 아니고, 점프 상태가 아니면 웅크리기 가능.
	if (ActionState != ECharacterActionState::ECAS_CROUCH &&
		ActionState != ECharacterActionState::ECAS_JUMP)
	{
		// 캐릭터의 액션 상태를 CROUCH로 변경.
		ActionState = ECharacterActionState::ECAS_CROUCH;

		// 캐릭터의 최대 이동 속도 감소.
		GetCharacterMovement()->MaxWalkSpeed = MaxMoveSpeed * 0.5f;
	
		// 캐릭터 캡슐 컴포넌트의 크기를 줄인다.
		GetCapsuleComponent()->SetCapsuleHalfHeight(GetCapsuleComponent()->GetScaledCapsuleHalfHeight() * 0.5f);
	}
	else
	{
		// 캐릭터가 웅크리기 상태인 경우, 대기 상태로 전환.
		ActionState = ECharacterActionState::ECAS_IDLE;		
	
		// 캐릭터의 최대 이동 속도 복구
		GetCharacterMovement()->MaxWalkSpeed = MaxMoveSpeed;

		// 캐릭터 캡슐 컴포넌트의 크기를 복원.
		GetCapsuleComponent()->SetCapsuleHalfHeight(CapsuleHalfHeight);
	}	
}

// 질주 입력 유지에 의해 액션 실행.
void AShooterCharacter::BeginSprint()
{
	if (ActionState == ECharacterActionState::ECAS_JUMP ||
		ActionState == ECharacterActionState::ECAS_CROUCH)
	{
		// 캐릭터의 액션 상태가 JUMP 또는 LAND 또는 CROUCH라면 질주 블가.
		return;
	}

	if (Stamina <= 0.0f)
	{
		// 스태미너가 0이라면 질주 불가.
		return;
	}
	
	// 캐릭터의 액션 상태를 SPRINT로 변경.
	ActionState = ECharacterActionState::ECAS_SPRINT;

	// 캐릭터의 최대 이동 속도 증가.
	GetCharacterMovement()->MaxWalkSpeed = MaxMoveSpeed * 1.75f;
}

// 질주 입력 해제에 의해 액션 종료.
void AShooterCharacter::EndSprint()
{
	if (ActionState == ECharacterActionState::ECAS_SPRINT)
	{
		// 캐릭터의 액션 상태를 IDLE로 변경.
		ActionState = ECharacterActionState::ECAS_IDLE;

		// 캐릭터의 최대 이동 속도 복원.
		GetCharacterMovement()->MaxWalkSpeed = MaxMoveSpeed;
	}	
}
#pragma endregion Inputs


/** 게임 업데이트 처리 */
// Called every frame
void AShooterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 질주 실행 여부에 따라 스태미너 관라.
	ExecuteStamina(DeltaTime);

	// 보간을 사용해 카메라의 높이를 변경.
	ExecuteCrouch(DeltaTime);

	// 캐릭터가 점프 후 지면에 도달하면, 액션 상태를 변경.
	if (ActionState == ECharacterActionState::ECAS_JUMP && !GetCharacterMovement()->IsFalling())
	{
		EndJump();
	}
}


/** 게임 입력 바인딩*/
// Called to bind functionality to input
void AShooterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);	

	// Shooter Player Controller에 접근하기 위해 Controller를 형변환.
	AShooterPlayerController* ShooterController = Cast<AShooterPlayerController>(GetController());
	
	if (ShooterController)
	{
		// 플레이어 입력 컴포넌트를 향상된 입력 컴포넌트로 형변환하여, 향상된 입력 액션을 바인딩한다.
		UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
		if (EnhancedInputComponent)		
		{
			// 이동 입력
			EnhancedInputComponent->BindAction(
				ShooterController->GetMoveAction(),
				ETriggerEvent::Triggered, 
				this, 
				&AShooterCharacter::Move);
			
			// 시선 입력
			EnhancedInputComponent->BindAction(
				ShooterController->GetLookupAction(),
				ETriggerEvent::Triggered, 
				this, 
				&AShooterCharacter::Lookup);

			// Jump 액션 입력
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

			// 웅크리기 액션 입력
			EnhancedInputComponent->BindAction(
				ShooterController->GetCrouchAction(),
				ETriggerEvent::Triggered,
				this,
				&AShooterCharacter::ChangeActionByCrouch);

			// 질주 액션 입력
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


