// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

// �׻� �������� ����.
#include "ShooterItem.generated.h"

/*
 * Terminator Re-Rampage Project
 * 2023. 09. 13
 * ĳ���Ͱ� �����Ͽ� ȹ���ϰų� ����ϴ� ������ ���̽� Ŭ������ ���
*/

#pragma region Enum Class
// �������� ����.
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

// Ŭ���� ����
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
	// ������ �޽� ������Ʈ (cpp���� ����)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component", meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* ShooterItemMesh;

	// ĳ���� ���� ������ ���� Sphere ������Ʈ (cpp���� ����)
	UPROPERTY(EditAnywhere, blueprintReadWrite, Category = "Component", meta = (AllowPrivateAccess = "true"))
	USphereComponent* CharacterOverlapSphere;

#pragma endregion

#pragma region Item Properties
	// ������ Ÿ��.
	UPROPERTY(EditAnywhere, blueprintReadWrite, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	EItemType ItemType = EItemType::EIT_NONE;

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

#pragma endregion

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
