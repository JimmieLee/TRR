// Fill out your copyright notice in the Description page of Project Settings.

// 커스텀 클래스 포함
#include "ShooterCharacter.h"
#include "ShooterPlayerController.h"

// 컴포넌트 클래스 포함.
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"

// 게임 프레임웍스 포함.
#include "GameFramework/CharacterMovementComponent.h"

// 유틸리티 포함.
#include "Curves/CurveFloat.h"
#include "Math/UnrealMathUtility.h"

// 향상된 입력 포함.
#include "EnhancedInputComponent.h"

/*
 * Terminator Re-Rampage Project
 * 2023. 08. 29
 * 플레이어가 제어하는 캐릭터 클래스의 CPP
*/

/** 클래스 생성자 */
// Sets default values
AShooterCharacter::AShooterCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	/** 컴포넌트 서브클래스 생성 */
	// 플레이 카메라 서브클래스 생성. (액터 및 블루프린트에 노출)
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

	// 1인칭 스켈렉탈 매시 서브클래스 생성. (액터 및 블루프린트에 노출)
	ShooterMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("First Person Body"));
	if (ShooterMesh)
	{
		// Shooter Mesh가 유효하면, Shooter Camera 하위에 등록.
		// Shooter Mesh의 위치(Lcoation)을 조정.
		ShooterMesh->SetupAttachment(ShooterCamera);
		ShooterMesh->SetRelativeLocation(FVector(-10.0f, -10.0f, -150.0f));
	}

	/** 캐릭터 파라미터 설정 */
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
}

/** 게임 시작 처리 */
// Called when the game starts or when spawned
void AShooterCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Shooter Player Controller에 접근하기 위해 Controller를 형변환.
	PlayerController = Cast<AShooterPlayerController>(GetController());
}

/** 캐릭터 액션 처리 */
// Crouch 상태를 처리 (프레임 검색)
void AShooterCharacter::ExecuteCrouch(float DeltaTime)
{
	if (ActionState == ECharacterActionState::ECAS_CROUCH)
	{
		// 캐릭터 상태가 Crouch라면
		// 현재 캐릭터의 Z축 높이부터 CharacterCrouchHeight까지
		// 0.25초간 매 프레임마다 높이 값을 구한다.
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
		// 0.25초간 매 프레임마다 높이 값을 구한다.
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
	// 캐릭터가 Jump 상태가 아니고, 공중에 떠 있지 않다면,
	// 액션 상태를 JUMP로 변경하고 점프를 실행한다.
	if (ActionState != ECharacterActionState::ECAS_JUMP && !GetCharacterMovement()->IsFalling())
	{
		ActionState = ECharacterActionState::ECAS_JUMP;
		Jump();
	}
}

void AShooterCharacter::EndJump()
{
	// 점프를 중지하고, 액션 상태를 IDLE로 변경한다.	
	StopJumping();
	ActionState = ECharacterActionState::ECAS_IDLE;
}

// 웅크리기 입력에 따른 액션 상태를 전환.
void AShooterCharacter::ChangeActionByCrouch()
{
	// 캐릭터가 웅크리기 상태가 아니고, 점프 상태가 아니어야 웅크리기 가능.
	if (ActionState != ECharacterActionState::ECAS_CROUCH &&
		ActionState != ECharacterActionState::ECAS_JUMP)
	{
		// 캐릭터의 액션 상태를 CROUCH로 변경.
		ActionState = ECharacterActionState::ECAS_CROUCH;

		// 캐릭터 캡슐 컴포넌트의 크기를 줄인다.
		GetCapsuleComponent()->SetCapsuleHalfHeight(GetCapsuleComponent()->GetScaledCapsuleHalfHeight() * 0.5f);
	}
	else
	{
		// 캐릭터 캡슐 컴포넌트의 크기를 복구.
		GetCapsuleComponent()->SetCapsuleHalfHeight(CapsuleHalfHeight);

		// 캐릭터가 웅크리기 상태인 경우, 대기 상태로 전환.
		ActionState = ECharacterActionState::ECAS_IDLE;
	}	
}

// 플레이어의 입력 유지에 의해 질주 실행/ 해제.
void AShooterCharacter::Sprint()
{
	// 캐릭터의 액션 상태를 SPRINT로 변경.
	ActionState = ECharacterActionState::ECAS_SPRINT;
	
	UE_LOG(LogTemp, Warning, TEXT("Sprint"));
}


/** 게임 업데이트 처리 */
// Called every frame
void AShooterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 보간을 사용해 카메라의 높이를 변경.
	ExecuteCrouch(DeltaTime);
}

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
				&AShooterCharacter::EndJump);			

			// 웅크리기 액션 입력
			EnhancedInputComponent->BindAction(
				ShooterController->GetCrouchAction(),
				ETriggerEvent::Triggered,
				this,
				&AShooterCharacter::Crouching);

			// 질주 액션 입력
			EnhancedInputComponent->BindAction(
				ShooterController->GetSprintAction(),
				ETriggerEvent::Ongoing,
				this,
				&AShooterCharacter::Sprint);
		}
	}
}

