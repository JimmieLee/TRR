// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"

// 항상 마지막에 포함.
#include "ShooterPlayerController.generated.h"

// 클래스 선언
class UInputMappingContext;
class UInputAction;

/**
* Terminator Re-Rampage Project
* 2023. 08. 29
* 플레이어가 제어하는 캐릭터의 고유 컨트롤러 클래스의 헤더
*/

UCLASS()
class TRR_API AShooterPlayerController : public APlayerController
{
	GENERATED_BODY()

private:
	/** 향상된 입력 */
	// 입력 매핑 컨텍스트 (에디터에서 노출)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input Mapping", meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* MovementMappingContext;

	// 이동 입력 액션 (축 입력, 에디터에서 노출) 
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input Action", meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	// 시점 입력 액션 (축 입력, 에디터에서 노출)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input Action", meta = (AllowPrivateAccess = "true"))
	UInputAction* LookupAction;

	// 점프 입력 액션 (실행/해제 입력, 에디터에서 노출)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input Action", meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	// 웅크리기 입력 액션 (실행/해제 입력, 에디터에서 노출)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input Action", meta = (AllowPrivateAccess = "true"))
	UInputAction* CrouchAction;

	// 질주 입력 액션 (실행유지/해제 입력, 에디터에서 노출)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input Action", meta = (AllowPrivateAccess = "true"))
	UInputAction* SprintAction;

protected:
	// 게임이 시작할 때 호출
	virtual void BeginPlay() override;

public:	
	/** private 멤버 변수에 대한 외부 클래스 접근 인라인 함수 */
	// 입력 액션 인라인 함수
	FORCEINLINE UInputAction* GetMoveAction() const { return MoveAction; }
	FORCEINLINE UInputAction* GetLookupAction() const { return LookupAction; }	
	FORCEINLINE UInputAction* GetJumpAction() const { return JumpAction; }
	FORCEINLINE UInputAction* GetCrouchAction() const { return CrouchAction; }
	FORCEINLINE UInputAction* GetSprintAction() const { return SprintAction; }
};
