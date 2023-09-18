// Fill out your copyright notice in the Description page of Project Settings.

// 커스텀 클래스 포함
#include "ShooterWeapon.h"
#include "ShooterCharacter.h"

// 컴포넌트 클래스 포함.
#include "Components/SkeletalMeshComponent.h"

/*
 * Terminator Re-Rampage Project
 * 2023. 09. 18.
 * 캐릭터가 획득하고, 장착하여 적대적 대상을 공격하는 무기 베이스 클래스의 CPP
*/

#pragma region Constructor
AShooterWeapon::AShooterWeapon()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	/** 컴포넌트 서브클래스 생성 */
	// 무기 메시 서브클래스 생성.
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon Body"));
	if (WeaponMesh)
	{
		WeaponMesh->SetupAttachment(GetRootComponent());
	}
}

#pragma region Weapon Management
void AShooterWeapon::PickUpWeapon(AShooterCharacter* Shooter)
{
	if (Shooter)
	{
		UE_LOG(LogTemp, Warning, TEXT("Picking Weapon!"));
	}	
}

#pragma endregion

