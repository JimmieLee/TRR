// Fill out your copyright notice in the Description page of Project Settings.

// Ŀ���� Ŭ���� ����
#include "ShooterItem.h"
#include "ShooterCharacter.h"

// ������Ʈ Ŭ���� ����.
#include "Components/SkeletalMeshComponent.h"
#include "Components/SphereComponent.h"

/*
 * Terminator Re-Rampage Project
 * 2023. 09. 13
 * ĳ���Ͱ� �����Ͽ� ȹ���ϰų� ����ϴ� ������ ���̽� Ŭ������ CPP
*/

#pragma region Constructor

// Sets default values
AShooterItem::AShooterItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	/** ������Ʈ ����Ŭ���� ���� */
	// ������ �޽� ����Ŭ���� ����.
	ShooterItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Item Body"));
	if (ShooterItemMesh)
	{
		// Shooter Item Mesh�� ��ȿ�ϸ�, Root Component�� ����.
		SetRootComponent(ShooterItemMesh);
	}

	// ĳ���� ���� ���� Sphere ����Ŭ���� ����.
	CharacterOverlapSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Character Overlap Sensor"));
	if (CharacterOverlapSphere)
	{
		// Root Component ������ ���.
		// �ѷ�(Radius)�� 256���� ����.
		CharacterOverlapSphere->SetupAttachment(GetRootComponent());
		CharacterOverlapSphere->SetSphereRadius(256.0f);
	}
}

#pragma endregion

// Called when the game starts or when spawned
void AShooterItem::BeginPlay()
{
	Super::BeginPlay();

	
#pragma region Dynamic Character Overlap Check

	// ĳ���Ͱ� Sphere�� �����Ǵ� ���� üũ.
	CharacterOverlapSphere->OnComponentBeginOverlap.AddDynamic(this, &AShooterItem::CharacterBeginOverlap);

	// ĳ���Ͱ� Sphere�� �������� �ʴ� ���� üũ.
	CharacterOverlapSphere->OnComponentEndOverlap.AddDynamic(this, &AShooterItem::CharacterEndOverlap);

#pragma endregion

}


#pragma region Character Overlap Event

// ĳ���Ͱ� Shpere�� �����ϸ�, ĳ���� Ŭ���� ������ ���ؼ� ������ ȹ�� ����� ȣ��.
void AShooterItem::CharacterBeginOverlap(
	UPrimitiveComponent* OverlappedComponent, 
	AActor* OtherActor, UPrimitiveComponent* OtherComp, 
	int32 OtherBodyIndex, bool bFromSweep, 
	const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		// ĳ���� Ŭ���� ������ ���� ĳ����.
		AShooterCharacter* Shooter = Cast<AShooterCharacter>(OtherActor);

		// ĳ���� Ŭ���� ������ ���ؼ� ������ ȹ�� ����� ȣ��.
		Shooter->PickUpItem(this);
	}
}

// ĳ���Ͱ� Sphere�� ���� ������ ����� ȣ��.
void AShooterItem::CharacterEndOverlap(
	UPrimitiveComponent* OverlappedComp, 
	AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, 
	int32 OtherBodyIndex)
{
	
}

// ĳ���Ͱ� �������� ȹ���ϸ�, ���� ���� ���� �������� ����.
void AShooterItem::DestroyItem()
{
	if (bCompletePickUp)
	{
		Destroy();
	}
}

#pragma endregion

// Called every frame
void AShooterItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	DestroyItem();
}

