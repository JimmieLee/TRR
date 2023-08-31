// Fill out your copyright notice in the Description page of Project Settings.

// 커스텀 클래스 포함
#include "ShooterCharacter.h"
#include "ShooterPlayerController.h"

// 컴포넌트 클래스 포함.
#include "Camera/CameraComponent.h"

// 향상된 입력 포함.
#include "EnhancedInputComponent.h"

/*
* Terminator Re-Rampage Project
* 2023. 08. 29
* 플레이어가 제어하는 캐릭터 클래스의 CPP
*/

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

	// 클래스가 Controller의 Yaw 회전을 사용하도록 설정.
	this->bUseControllerRotationPitch = false;
	this->bUseControllerRotationYaw = true;
	this->bUseControllerRotationRoll = false;
}

/** 게임 시작 처리 */
// Called when the game starts or when spawned
void AShooterCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}


/** 캐릭터 제어 입력 처리 */
// 플레이어의 입력에 따라 캐릭터의 이동 방향을 설정.
void AShooterCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		/*
		// 컨트롤러의 회전 중, Yaw 회전만 적용된 새로운 회전자를 생성.
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// 전면 벡터를 구한다.
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// 측면 벡터를 구한다.
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		*/

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
void AShooterCharacter::Jump()
{
	UE_LOG(LogTemp, Warning, TEXT("Jump"));
}

// 플레이어의 입력에 의해 웅크리기 실행/ 해제
void AShooterCharacter::Crouch()
{
	UE_LOG(LogTemp, Warning, TEXT("Crouch"));
}

// 플레이어의 입력 유지에 의해 질주 실행/ 해제.
void AShooterCharacter::Sprint()
{
	UE_LOG(LogTemp, Warning, TEXT("Sprint"));
}


/** 게임 업데이트 처리 */
// Called every frame
void AShooterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


// Called to bind functionality to input
void AShooterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	
	// Shooter Player Controller에 접근하기 위해 Controller를 형변환.
	AShooterPlayerController* PlayerController = Cast<AShooterPlayerController>(GetController());
	if (PlayerController)
	{
		// Shooter Player Controller가 유효하면, PlayerInputComponent를 향상된 입력 컴포넌트로 형변환.

		UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
		if (EnhancedInputComponent)
		{
			// 향상된 입력 컴포넌트가 유효하면,
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

