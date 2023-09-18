// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ShooterItem.h"
#include "WeaponProperties.h"

// 항상 마지막에 포함.
#include "ShooterWeapon.generated.h"

/*
 * Terminator Re-Rampage Project
 * 2023. 09. 18.
 * 캐릭터가 획득하고, 장착하여 적대적 대상을 공격하는 무기 베이스 클래스의 헤더
*/

// 클래스 선언
class USkeletalMeshComponent;

// 무기의 속성 구조체
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
	// 무기 메시 컴포넌트 (cpp에서 생성)
	// 무기는 애니메이션이 가능하기 때문에 스켈렉탈 메시를 추가로 생성.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* WeaponMesh;

#pragma endregion

	// 무기 속성
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Properties", meta = (AllowPrivateAccess = "true"))
	FWeaponProperty WeaponProperty;

protected:


public:
	// 무기를 획득.
	void PickUpWeapon(class AShooterCharacter* Shooter);

};
