// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CommonTypes.h"

// �׻� �������� ����.
#include "ShooterItem.generated.h"

/*
 * Terminator Re-Rampage Project
 * 2023. 09. 13
 * ĳ���Ͱ� �����Ͽ� ȹ���ϰų� ����ϴ� ������ ���̽� Ŭ������ ���
*/

// Ŭ���� ����
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
	// ������ �޽� ������Ʈ (cpp���� ����)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* ShooterItemMesh;

	// ĳ���� ���� ������ ���� Sphere ������Ʈ (cpp���� ����)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component", meta = (AllowPrivateAccess = "true"))
	USphereComponent* CharacterOverlapSphere;

#pragma endregion

#pragma region Item Properties
	// ������ Ÿ��.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	EItemType ItemType = EItemType::EIT_NONE;

	// ������ ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	int32 ItemAmount = 0;

	// ȹ�� �Ϸ� ����.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	bool bCompletePickUp = false;

#pragma endregion

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

#pragma region Overlapped Event
	// ĳ������ ������ �����Ǹ� ȣ��.
	UFUNCTION()
	void CharacterBeginOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	// ĳ������ ���� ���ᰡ �����Ǹ� ȣ��.
	UFUNCTION()
	void CharacterEndOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);

	// ȹ���� �Ϸ�Ǹ� ������ ����.
	void DestroyItem();

#pragma endregion

public:	
#pragma region FORCEINLINE Functions
	// private ��� ������ �ܺ�Ŭ�������� ������ �� �ִ� �ζ��� �Լ�.
	FORCEINLINE EItemType GetItemType() const { return ItemType; }

#pragma endregion
	// ȹ�� �Ϸ� ���θ� ����. 
	void SetCompletedPickUpItem(bool bComplete) { bCompletePickUp = bComplete; }

	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
