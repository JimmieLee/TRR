// Fill out your copyright notice in the Description page of Project Settings.

// 커스텀 클래스 포함
#include "ShooterItem.h"
#include "ShooterCharacter.h"

// 컴포넌트 클래스 포함.
#include "Components/SkeletalMeshComponent.h"
#include "Components/SphereComponent.h"

/*
 * Terminator Re-Rampage Project
 * 2023. 09. 13
 * 캐릭터가 접촉하여 획득하거나 사용하는 아이템 베이스 클래스의 CPP
*/

#pragma region Constructor

// Sets default values
AShooterItem::AShooterItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	/** 컴포넌트 서브클래스 생성 */
	// 아이템 메시 서브클래스 생성.
	ShooterItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Item Body"));
	if (ShooterItemMesh)
	{
		// Shooter Item Mesh가 유효하면, Root Component로 설정.
		SetRootComponent(ShooterItemMesh);
	}

	// 캐릭터 접촉 감지 Sphere 서브클래스 생성.
	CharacterOverlapSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Character Overlap Sensor"));
	if (CharacterOverlapSphere)
	{
		// Root Component 하위에 등록.
		// 둘레(Radius)를 256으로 설정.
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

	// 캐릭터가 Sphere에 감지되는 지를 체크.
	CharacterOverlapSphere->OnComponentBeginOverlap.AddDynamic(this, &AShooterItem::CharacterBeginOverlap);

	// 캐릭터가 Sphere에 감지되지 않는 지를 체크.
	CharacterOverlapSphere->OnComponentEndOverlap.AddDynamic(this, &AShooterItem::CharacterEndOverlap);

#pragma endregion

}


#pragma region Character Overlap Event

// 캐릭터가 Shpere와 접촉하면, 캐릭터 클래스 참조를 통해서 아이템 획득 기능을 호출.
void AShooterItem::CharacterBeginOverlap(
	UPrimitiveComponent* OverlappedComponent, 
	AActor* OtherActor, UPrimitiveComponent* OtherComp, 
	int32 OtherBodyIndex, bool bFromSweep, 
	const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		// 캐릭터 클래스 참조를 위해 캐스팅.
		AShooterCharacter* Shooter = Cast<AShooterCharacter>(OtherActor);

		// 캐릭터 클래스 참조를 통해서 아이템 획득 기능을 호출.
		Shooter->PickUpItem(this);
	}
}

// 캐릭터가 Sphere의 감지 범위를 벗어나면 호출.
void AShooterItem::CharacterEndOverlap(
	UPrimitiveComponent* OverlappedComp, 
	AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, 
	int32 OtherBodyIndex)
{
	
}

// 캐릭터가 아이템을 획득하면, 게임 월드 상의 아이템을 제거.
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

