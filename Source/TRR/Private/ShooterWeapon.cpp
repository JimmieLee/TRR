// Fill out your copyright notice in the Description page of Project Settings.

// Ŀ���� Ŭ���� ����
#include "ShooterWeapon.h"
#include "ShooterCharacter.h"

// ������Ʈ Ŭ���� ����.
#include "Components/SkeletalMeshComponent.h"

/*
 * Terminator Re-Rampage Project
 * 2023. 09. 18.
 * ĳ���Ͱ� ȹ���ϰ�, �����Ͽ� ������ ����� �����ϴ� ���� ���̽� Ŭ������ CPP
*/

#pragma region Constructor
AShooterWeapon::AShooterWeapon()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	/** ������Ʈ ����Ŭ���� ���� */
	// ���� �޽� ����Ŭ���� ����.
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

