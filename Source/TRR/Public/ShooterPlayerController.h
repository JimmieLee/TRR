// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"

// �׻� �������� ����.
#include "ShooterPlayerController.generated.h"

// Ŭ���� ����
class UInputMappingContext;
class UInputAction;

/**
* Terminator Re-Rampage Project
* 2023. 08. 29
* �÷��̾ �����ϴ� ĳ������ ���� ��Ʈ�ѷ� Ŭ������ ���
*/

UCLASS()
class TRR_API AShooterPlayerController : public APlayerController
{
	GENERATED_BODY()

private:
	/** ���� �Է� */
	// �Է� ���� ���ؽ�Ʈ (�����Ϳ��� ����)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input Mapping", meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* MovementMappingContext;

	// �̵� �Է� �׼� (�� �Է�, �����Ϳ��� ����) 
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input Action", meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	// ���� �Է� �׼� (�� �Է�, �����Ϳ��� ����)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input Action", meta = (AllowPrivateAccess = "true"))
	UInputAction* LookupAction;

	// ���� �Է� �׼� (����/���� �Է�, �����Ϳ��� ����)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input Action", meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	// ��ũ���� �Է� �׼� (����/���� �Է�, �����Ϳ��� ����)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input Action", meta = (AllowPrivateAccess = "true"))
	UInputAction* CrouchAction;

	// ���� �Է� �׼� (��������/���� �Է�, �����Ϳ��� ����)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input Action", meta = (AllowPrivateAccess = "true"))
	UInputAction* SprintAction;

protected:
	// ������ ������ �� ȣ��
	virtual void BeginPlay() override;

public:	
	/** private ��� ������ ���� �ܺ� Ŭ���� ���� �ζ��� �Լ� */
	// �Է� �׼� �ζ��� �Լ�
	FORCEINLINE UInputAction* GetMoveAction() const { return MoveAction; }
	FORCEINLINE UInputAction* GetLookupAction() const { return LookupAction; }	
	FORCEINLINE UInputAction* GetJumpAction() const { return JumpAction; }
	FORCEINLINE UInputAction* GetCrouchAction() const { return CrouchAction; }
	FORCEINLINE UInputAction* GetSprintAction() const { return SprintAction; }
};
