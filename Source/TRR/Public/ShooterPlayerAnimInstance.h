// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CommonTypes.h"

// 항상 마지막에 포함
#include "ShooterPlayerAnimInstance.generated.h"

/*
* Terminator Re-Rampage Project
* 2023. 09 .01
* 플레이어 캐릭터의 애니메이션 정보 클래스의 헤더
*/

// 클래스 선언
class AShooterCharacter;
 
UCLASS()
class TRR_API UShooterPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UShooterPlayerAnimInstance();

private:
	/** 캐릭터 정보 */
	// 캐릭터 클래스 참조
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character Reference", meta = (AllowPrivateAccess = "true"))
	AShooterCharacter* Shooter;
	
	/** 캐릭터 자세 정보 */
	// 캐릭터가 이동하는 속도 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float MoveSpeed = 0.0f;

	// 캐릭터의 액션 상태.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action", meta = (AllowPrivateAccess = "true"))
	ECharacterActionState ActionState = ECharacterActionState::ECAS_IDLE;
	
	/** 캐릭터 무기 정보*/
	// 캐릭터가 현재 장착하고 있는 무기의 타입.

protected:
	// 애니메이션 요소를 재정의 한다.
	virtual void NativeInitializeAnimation() override;

};
