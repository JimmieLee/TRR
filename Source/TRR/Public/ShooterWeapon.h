// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ShooterItem.h"
#include "WeaponProperties.h"

// �׻� �������� ����.
#include "ShooterWeapon.generated.h"

/*
 * Terminator Re-Rampage Project
 * 2023. 09. 18.
 * ĳ���Ͱ� ȹ���ϰ�, �����Ͽ� ������ ����� �����ϴ� ���� ���̽� Ŭ������ ���
*/

// Ŭ���� ����
class USkeletalMeshComponent;

// ������ �Ӽ� ����ü
USTRUCT(BlueprintType)
struct FWeaponProperty
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	EWeaponType WeaponType;

	UPROPERTY(EditAnywhere)
	EAmmoType AmmoType;

	UPROPERTY(EditAnywhere)
	EShotType ShotType;
	
	UPROPERTY(EditAnywhere)
	int32 MagazineSize;

	UPROPERTY(EditAnywhere)
	int32 HitPointNum;

	UPROPERTY(EditAnywhere)
	FVector2D Recoil;

	UPROPERTY(EditAnywhere)
	float HitDistance;

	UPROPERTY(EditAnywhere)
	float FireRate;
};


UCLASS()
class TRR_API AShooterWeapon : public AShooterItem
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AShooterWeapon();

private:
#pragma region Components
	// ���� �޽� ������Ʈ (cpp���� ����)
	// ����� �ִϸ��̼��� �����ϱ� ������ ���̷�Ż �޽ø� �߰��� ����.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* WeaponMesh;

#pragma endregion

	// ���� �Ӽ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Properties", meta = (AllowPrivateAccess = "true"))
	FWeaponProperty WeaponProperty;

protected:


public:
	// ���⸦ ȹ��.
	void PickUpWeapon(class AShooterCharacter* Shooter);

};
