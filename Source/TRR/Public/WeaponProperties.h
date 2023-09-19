#pragma once

/*
* Terminator Re-Rampage Project
* 2023. 08. 31
* 게임 플레이 상황에 따라 변화하는 캐릭터의 상태 선언.
*/

#pragma region Weapon Types
// 무기 타입
UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	EWT_Handgun UMETA(DisplayName = "Handgun"),
	EIT_Rifle UMETA(DisplayName = "Rifle"),
	EIT_Shotgun UMETA(DisplayName = "Shotgun"),
	EIT_Launcher UMETA(DisplayName = "Launcher")
};

// 탄약 타입
UENUM(BlueprintType)
enum class EAmmoType : uint8
{
	EAT_Handgun UMETA(DisplayName = "9mm"),
	EAT_Rifle UMETA(DisplayName = "5.56mm"),
	EAT_Shotgun UMETA(DisplayName = "Shell"),
	EAT_GL UMETA(DisplayName = "Grenade"),
	EAT_RL UMETA(DisplayName = "Rocket"),
	EAT_Sniper UMETA(DisplayName = "7.62mm")
};

// 탄약 타입
UENUM(BlueprintType)
enum class EShotType : uint8
{
	EST_Single UMETA(DisplayName = "Single Shot"),
	EST_Auto UMETA(DisplayName = "Automatic Shot")
};

// 무기 보관 공간.
UENUM(BlueprintType)
enum class ESlotType : uint8
{
	ESLT_Primary UMETA(DisplayName = "First Slot"),
	ESLT_Secondary UMETA(DisplayName = "Second Slot"),
	ESLT_Special UMETA(DisplayName = "Third Slot"),
	ESLT_Special2 UMETA(DisplayName = "Forth Slot"),
	ESLT_Special3 UMETA(DisplayName = "Fifth Slot"),
	ESLT_Special4 UMETA(DisplayName = "Last Slot")
};

#pragma endregion