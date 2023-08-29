// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

// ���� �Է� ����.
#include "InputActionValue.h"

// �׻� �������� ����.
#include "ShooterCharacter.generated.h"

/*
* Terminator Re-Rampage Project
* 2023. 08. 29
* �÷��̾ �����ϴ� ĳ���� Ŭ������ ���
*/

// Ŭ���� ����
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
	/** ������Ʈ ���� */
	// �÷��� ī�޶� ������Ʈ (cpp���� ����)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Component, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* ShooterCamera;

	// 1��Ī ���̷�Ż �޽� ������Ʈ (cpp���� ����)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Component, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* ShooterMesh;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// �̵� �Է� ó��
	void Move(const FInputActionValue& Value);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
