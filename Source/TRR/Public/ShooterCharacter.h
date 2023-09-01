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
	/** 컴포넌트 */
	// 플레이 카메라 컴포넌트 (cpp에서 생성)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* ShooterCamera;

	// 1인칭 스켈렉탈 메시 컴포넌트 (cpp에서 생성)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* ShooterMesh;

	/** 캐릭터 액션 */
	// 캐릭터 액션 상태 (기본 값은 IDLE)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Action Properties", meta = (AllowPrivateAccess = "true"))
	ECharacterActionState ActionState = ECharacterActionState::ECAS_IDLE;	

	// 점프 시간 (점프 커브의 길이와 비교)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action Properties", meta = (AllowPrivateAccess = "true"))
	float JumpingTime;

	/** 캐릭터 컨트롤러 */
	// 플레이어 컨트롤러
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Controller", meta = (AllowPrivateAccess = "true"))
	AShooterPlayerController* PlayerController;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// 이동 입력 처리
	void Move(const FInputActionValue& Value);

	// 시선 입력 처리
	void Lookup(const FInputActionValue& Value);

	// 점프 액션 입력 처리.
	void BeginJump();

	// 점프 액션 종료 처리.
	void EndJump();

	// 웅크리기 액션 입력 처리.
	void Crouching();

	// 질주 액션 입력 처리.
	void Sprint();		
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
