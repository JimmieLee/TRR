// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CommonTypes.h"

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
class AShooterPlayerController;
class AShooterItem;
class AShooterWeapon;

UCLASS()
class TRR_API AShooterCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AShooterCharacter();

private:
#pragma region Components
	// �÷��� ī�޶� ������Ʈ (cpp���� ����)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* ShooterCamera;

	// 1��Ī ���̷�Ż �޽� ������Ʈ (cpp���� ����)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* ShooterMesh;

#pragma endregion

#pragma region Action Properties
	/** ĳ���� �׼� */
	// ĳ���� �׼� ���� (�⺻ ���� IDLE)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Action Properties", meta = (AllowPrivateAccess = "true"))
	ECharacterActionState ActionState = ECharacterActionState::ECAS_IDLE;	

	// ĳ������ ĸ�� ������Ʈ �⺻ ����.
	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = "Action Properties", meta = (AllowPrivateAccess = "true"))
	float CapsuleHalfHeight;	
		
	// Stand ���¿����� ī�޶� �⺻ ��ġ.
	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = "Action Properties", meta = (AllowPrivateAccess = "true"))
	FVector ShooterCameraPosition = FVector(0.0f, 0.0f, 0.0f);

	// Crouch ���¿����� ī�޶� ����.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action Properties", meta = (AllowPrivateAccess = "true"))
	float CrouhedCameraHeight = 25.0f;

	// ĳ������ �⺻ �ִ� �̵� �ӵ�.
	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = "Action Properties", meta = (AllowPrivateAccess = "true"))
	float MaxMoveSpeed = 0.0f;

	// ĳ������ �ִ� ������ ����Ʈ
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action Properties", meta = (AllowPrivateAccess = "true"))
	float Stamina = 100.0f;

	// ������ ���ҷ� (������ ��)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action Properties", meta = (AllowPrivateAccess = "true"))
	float ReductionStamina = 0.2f;

	// ������ ȸ���� (������ ��)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action Properties", meta = (AllowPrivateAccess = "true"))
	float RestoreStamina = 0.25f;

	// ������ ȸ�� ���۱����� ��� �ð�.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action Properties", meta = (AllowPrivateAccess = "true"))
	float RestoreStaminaCooldown = 0.0f;

#pragma endregion

#pragma region Weapon Inventory
	// ���� ���� ���� 1. (First Slot)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon Inventory", meta = (AllowPrivateAccess = "true"))
	AShooterWeapon* PrimaryWeaponSlot;
	// ���� ���� ���� 2. (Second Slot)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon Inventory", meta = (AllowPrivateAccess = "true"))
	AShooterWeapon* SecondaryWeaponSlot;
	// ���� ���� ���� 3. (Third Slot)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon Inventory", meta = (AllowPrivateAccess = "true"))
	AShooterWeapon* SpecialWeaponSlot;
	// ���� ���� ���� 4. (Forth Slot)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon Inventory", meta = (AllowPrivateAccess = "true"))
	AShooterWeapon* Special2WeaponSlot;
	// ���� ���� ���� 5. (Fifth Slot)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon Inventory", meta = (AllowPrivateAccess = "true"))
	AShooterWeapon* Special3WeaponSlot;
	// ���� ���� ���� 6. (SixTh Slot)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon Inventory", meta = (AllowPrivateAccess = "true"))
	AShooterWeapon* Special4WeaponSlot;

#pragma endregion

	// ȹ���� �������� �ӽ÷� ����. (������ ȹ�� ��, ���� ���忡�� �����ϱ� ����)
	AShooterItem* ItemTemp;

#pragma region Controller
	/** ĳ���� ��Ʈ�ѷ� */
	// �÷��̾� ��Ʈ�ѷ�
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Controller", meta = (AllowPrivateAccess = "true"))
	AShooterPlayerController* PlayerController;

#pragma endregion

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

#pragma region Action Functions
	// Crouch ���� ��ȭ�� ���� ī�޶� ������ ��ȯ.
	void ExecuteCrouch(float DeltaTime);	

	// Sprint ���ο� ���� ������(Statmina) ����.
	void ExecuteStamina(float DeltaTime);

	// Dash Timer ���� �ÿ� Action State ������ ó���ϴ� Callback
	void FinishDashTimer();

#pragma endregion

#pragma region Input Functions
	// �̵� �Է� ó��
	void Move(const FInputActionValue& Value);

	// �ü� �Է� ó��
	void Lookup(const FInputActionValue& Value);

	// ���� �׼� �Է� ó��.
	void BeginJump();

	// ���� �׼� ���� ó��.
	void Landing();

	// ���� ���� ���� ���� �׼� ���� ���� ó��.
	void EndJump();

	// ��ũ���� ���� ���ο� �׼� ���� ó��.
	void ChangeActionByCrouch();

	// ���� �׼� �Է� ó��.
	void BeginSprint();		

	// ���� �׼� ���� ó��.
	void EndSprint();
#pragma endregion
	
public:	
#pragma region FORCEINLINE Functions
	// private ��� ������ �ܺ�Ŭ�������� ������ �� �ִ� �ζ��� �Լ�.
	FORCEINLINE UCameraComponent* GetShooterCamera() const { return ShooterCamera; }
	FORCEINLINE ECharacterActionState GetActionState() const { return ActionState; }
	FORCEINLINE float GetStamina() const { return Stamina; }

#pragma endregion

	// ������ �������� ȹ��.
	void PickUpItem(AShooterItem* Item);

	// ȹ���� ���⸦ ���Կ� ����.
	void AddWeaponToSlot(AShooterWeapon* Weapon);
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
