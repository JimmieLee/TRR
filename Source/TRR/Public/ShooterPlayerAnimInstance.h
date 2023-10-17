// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CommonTypes.h"

// �׻� �������� ����
#include "ShooterPlayerAnimInstance.generated.h"

/*
* Terminator Re-Rampage Project
* 2023. 09 .01
* �÷��̾� ĳ������ �ִϸ��̼� ���� Ŭ������ ���
*/

// Ŭ���� ����
class AShooterCharacter;
 
UCLASS()
class TRR_API UShooterPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UShooterPlayerAnimInstance();

private:
	/** ĳ���� ���� */
	// ĳ���� Ŭ���� ����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character Reference", meta = (AllowPrivateAccess = "true"))
	AShooterCharacter* Shooter;
	
	/** ĳ���� �ڼ� ���� */
	// ĳ���Ͱ� �̵��ϴ� �ӵ� 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float MoveSpeed = 0.0f;

	// ĳ������ �׼� ����.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action", meta = (AllowPrivateAccess = "true"))
	ECharacterActionState ActionState = ECharacterActionState::ECAS_IDLE;
	
	/** ĳ���� ���� ����*/
	// ĳ���Ͱ� ���� �����ϰ� �ִ� ������ Ÿ��.

protected:
	// �ִϸ��̼� ��Ҹ� ������ �Ѵ�.
	virtual void NativeInitializeAnimation() override;

};
