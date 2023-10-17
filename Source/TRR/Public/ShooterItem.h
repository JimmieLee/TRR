// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CommonTypes.h"

// 항상 마지막에 포함.
#include "ShooterItem.generated.h"

/*
 * Terminator Re-Rampage Project
 * 2023. 09. 13
 * 캐릭터가 접촉하여 획득하거나 사용하는 아이템 베이스 클래스의 헤더
*/

// 클래스 선언
class UStaticMeshComponent;
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
	UStaticMeshComponent* ShooterItemMesh;

	// 캐릭터 접촉 감지를 위한 Sphere 컴포넌트 (cpp에서 생성)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component", meta = (AllowPrivateAccess = "true"))
	USphereComponent* CharacterOverlapSphere;

#pragma endregion

#pragma region Item Properties
	// 아이템 타입.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	EItemType ItemType = EItemType::EIT_NONE;

	// 아이템 수량
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	int32 ItemAmount = 0;

	// 획득 완료 여부.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	bool bCompletePickUp = false;

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

	// 획득이 완료되면 아이템 제거.
	void DestroyItem();

#pragma endregion

public:	
#pragma region FORCEINLINE Functions
	// private 멤버 변수에 외부클래스에서 접근할 수 있는 인라인 함수.
	FORCEINLINE EItemType GetItemType() const { return ItemType; }

#pragma endregion
	// 획득 완료 여부를 설정. 
	void SetCompletedPickUpItem(bool bComplete) { bCompletePickUp = bComplete; }

	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
