// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CharacterProperties.h"

// 향상된 입력 포함.
#include "InputActionValue.h"

// 항상 마지막에 포함.
#include "ShooterCharacter.generated.h"

/*
 * Terminator Re-Rampage Project
 * 2023. 08. 29
 * 플레이어가 제어하는 캐릭터 클래스의 헤더
*/

// 클래스 선언
class USkeletalMeshComponent;
class UCameraComponent;
class AShooterPlayerController;

UCLASS()
class TRR_API AShooterCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AShooterCharacter();

private:
#pragma region Components
	// 플레이 카메라 컴포넌트 (cpp에서 생성)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* ShooterCamera;

	// 1인칭 스켈렉탈 메시 컴포넌트 (cpp에서 생성)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* ShooterMesh;

#pragma endregion

#pragma region Action Properties
	/** 캐릭터 액션 */
	// 캐릭터 액션 상태 (기본 값은 IDLE)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Action Properties", meta = (AllowPrivateAccess = "true"))
	ECharacterActionState ActionState = ECharacterActionState::ECAS_IDLE;	

	// 점프 시간 (점프 커브의 길이와 비교)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action Properties", meta = (AllowPrivateAccess = "true"))
	float JumpingTime;

	// 캐릭터의 캡슐 컴포넌트 기본 높이.
	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = "Action Properties", meta = (AllowPrivateAccess = "true"))
	float CapsuleHalfHeight;	
		
	// Stand 상태에서의 카메라 기본 위치.
	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = "Action Properties", meta = (AllowPrivateAccess = "true"))
	FVector ShooterCameraPosition = FVector(0.0f, 0.0f, 0.0f);

	// Crouch 상태에서의 카메라 높이.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action Properties", meta = (AllowPrivateAccess = "true"))
	float CrouhedCameraHeight = 25.0f;

	// 캐릭터의 기본 최대 이동 속도.
	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = "Action Properties", meta = (AllowPrivateAccess = "true"))
	float MaxMoveSpeed = 0.0f;

	// 캐릭터의 최대 지구력 포인트
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action Properties", meta = (AllowPrivateAccess = "true"))
	float Stamina = 100.0f;

	// 지구력 감소량 (프레임 당)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action Properties", meta = (AllowPrivateAccess = "true"))
	float ReductionStamina = 0.2f;

	// 지구력 회복량 (프레임 당)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action Properties", meta = (AllowPrivateAccess = "true"))
	float RestoreStamina = 0.25f;

	// 지구력 회복 시작까지의 대기 시간.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action Properties", meta = (AllowPrivateAccess = "true"))
	float RestoreStaminaCooldown = 0.0f;

#pragma endregion

#pragma region Controller
	/** 캐릭터 컨트롤러 */
	// 플레이어 컨트롤러
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Controller", meta = (AllowPrivateAccess = "true"))
	AShooterPlayerController* PlayerController;

#pragma endregion

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

#pragma region Action Functions
	// Crouch 상태 변화에 따른 카메라 높이의 전환.
	void ExecuteCrouch(float DeltaTime);	

	// Sprint 여부에 따른 지구력(Statmina) 관리.
	void ExecuteStamina(float DeltaTime);

	// Dash Timer 종료 시에 Action State 변경을 처리하는 Callback
	void FinishDashTimer();

#pragma endregion


#pragma region Input Functions
	// 이동 입력 처리
	void Move(const FInputActionValue& Value);

	// 시선 입력 처리
	void Lookup(const FInputActionValue& Value);

	// 점프 액션 입력 처리.
	void BeginJump();

	// 점프 액션 종료 처리.
	void Landing();

	// 점프 상태 종료 이후 액션 상태 변경 처리.
	void EndJump();

	// 웅크리기 실행 여부에 액션 상태 처리.
	void ChangeActionByCrouch();

	// 질주 액션 입력 처리.
	void BeginSprint();		

	// 질주 액션 종료 처리.
	void EndSprint();
#pragma endregion
	
public:	
#pragma region FORCEINLINE Functions
	// private 멤버 변수에 외부클래스에서 접근할 수 있는 인라인 함수.
	FORCEINLINE UCameraComponent* GetShooterCamera() const { return ShooterCamera; }
	FORCEINLINE ECharacterActionState GetActionState() const { return ActionState; }
	FORCEINLINE float GetStamina() const { return Stamina; }
#pragma endregion
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
