#pragma once

/*
* Terminator Re-Rampage Project
* 2023. 10. 16
* 게임 플레이에 관련된 타입 열거형을 선언한다.
*/

#pragma region Character Action State
UENUM(BlueprintType)
enum class ECharacterActionState : uint8
{
	// 플레이어의 입력에 의한 변화하는 캐릭터 액션 열거형.
	ECAS_IDLE UMETA(DisplayName = "Idle"),
	ECAS_JUMP UMETA(DisplayName = "Jump"),
	ECAS_CROUCH UMETA(DisplayName = "Crouch"),
	ECAS_SPRINT UMETA(DisplayName = "Sprint"),
	ECAS_DASH UMETA(DisplayName = "Dash")
};

#pragma endregion

#pragma region Item Type
UENUM(BlueprintType)
enum class EItemType : uint8
{
	// 아이템을 세분하는 유형
	EIT_NONE UMETA(DisplayName = "None"),
	EIT_WEAPON UMETA(DisplayName = "Weapon"),
	EIT_AMMO UMETA(DisplayName = "Ammo"),
	EIT_KIT UMETA(DisplayName = "Kit"),
	EIT_SUIT UMETA(DisplayName = "Suit"),
	EIT_PART UMETA(DisplayName = "Parts")
};

#pragma endregion

#pragma region Weapon Properties
UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	// 무기 타입
	EWT_Handgun UMETA(DisplayName = "Handgun"),
	EIT_Rifle UMETA(DisplayName = "Rifle"),
	EIT_Shotgun UMETA(DisplayName = "Shotgun"),
	EIT_Launcher UMETA(DisplayName = "Launcher")
};

UENUM(BlueprintType)
enum class EAmmoType : uint8
{
	//탄약 타입
	EAT_Handgun UMETA(DisplayName = "9mm"),
	EAT_Rifle UMETA(DisplayName = "5.56mm"),
	EAT_Shotgun UMETA(DisplayName = "Shell"),
	EAT_GL UMETA(DisplayName = "Grenade"),
	EAT_RL UMETA(DisplayName = "Rocket"),
	EAT_Sniper UMETA(DisplayName = "7.62mm")
};

UENUM(BlueprintType)
enum class EShotType : uint8
{
	// 발사 타입
	EST_Single UMETA(DisplayName = "Single Shot"),
	EST_Auto UMETA(DisplayName = "Automatic Shot")
};

UENUM(BlueprintType)
enum class ESlotType : uint8
{
	// 무기의 전용 보관 공간 이름.
	ESLT_Primary UMETA(DisplayName = "First Slot"),
	ESLT_Secondary UMETA(DisplayName = "Second Slot"),
	ESLT_Special UMETA(DisplayName = "Third Slot"),
	ESLT_Special2 UMETA(DisplayName = "Forth Slot"),
	ESLT_Special3 UMETA(DisplayName = "Fifth Slot"),
	ESLT_Special4 UMETA(DisplayName = "Last Slot")
};

#pragma endregion

