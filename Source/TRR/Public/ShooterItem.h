// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

// 항상 마지막에 포함.
#include "ShooterItem.generated.h"

/*
 * Terminator Re-Rampage Project
 * 2023. 09. 13
 * 캐릭터가 접촉하여 획득하거나 사용하는 아이템 베이스 클래스의 헤더
*/

#pragma region Enum Class
// 아이템의 구분.
UENUM(BlueprintType)
enum class EItemType : uint8
{
	EIT_NONE UMETA(DisplayName = "None"),
	EIT_WEAPON UMETA(DisplayName = "Weapon"),
	EIT_KIT UMETA(DisplayName = "Kit"),
	EIT_SUIT UMETA(DisplayName = "Suit"),
	EIT_PART UMETA(DisplayName = "Parts")	
};

#pragma endregion

// 클래스 선언
class USkeletalMeshComponent;
class USphereComponent;
class AShooterCharacter;

UCLASS()
class TRR_API AShooterItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AShooterItem();

private:
#pragma region Components
	// 아이템 메시 컴포넌트 (cpp에서 생성)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* ShooterItemMesh;

	// 캐릭터 접촉 감지를 위한 Sphere 컴포넌트 (cpp에서 생성)
	UPROPERTY(EditAnywhere, blueprintReadWrite, Category = "Component", meta = (AllowPrivateAccess = "true"))
	USphereComponent* CharacterOverlapSphere;

#pragma endregion

#pragma region Item Properties
	// 아이템 타입.
	UPROPERTY(EditAnywhere, blueprintReadWrite, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	EItemType ItemType = EItemType::EIT_NONE;

#pragma endregion

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

#pragma region Overlapped Event
	// 캐릭터의 접촉이 감지되면 호출.
	UFUNCTION()
	void CharacterBeginOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	// 캐릭터의 접촉 종료가 감지되면 호출.
	UFUNCTION()
	void CharacterEndOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);

#pragma endregion

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
