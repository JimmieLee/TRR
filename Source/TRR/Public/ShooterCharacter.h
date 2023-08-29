// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

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

UCLASS()
class TRR_API AShooterCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AShooterCharacter();

private:
	/** 컴포넌트 선언 */
	// 플레이 카메라 컴포넌트 (cpp에서 생성)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Component, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* ShooterCamera;

	// 1인칭 스켈렉탈 메시 컴포넌트 (cpp에서 생성)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Component, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* ShooterMesh;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// 이동 입력 처리
	void Move(const FInputActionValue& Value);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
