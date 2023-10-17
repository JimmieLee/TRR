#pragma once

/*
* Terminator Re-Rampage Project
* 2023. 10. 16
* ���� �÷��̿� ���õ� Ÿ�� �������� �����Ѵ�.
*/

#pragma region Character Action State
UENUM(BlueprintType)
enum class ECharacterActionState : uint8
{
	// �÷��̾��� �Է¿� ���� ��ȭ�ϴ� ĳ���� �׼� ������.
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
	// �������� �����ϴ� ����
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
	// ���� Ÿ��
	EWT_Handgun UMETA(DisplayName = "Handgun"),
	EIT_Rifle UMETA(DisplayName = "Rifle"),
	EIT_Shotgun UMETA(DisplayName = "Shotgun"),
	EIT_Launcher UMETA(DisplayName = "Launcher")
};

UENUM(BlueprintType)
enum class EAmmoType : uint8
{
	//ź�� Ÿ��
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
	// �߻� Ÿ��
	EST_Single UMETA(DisplayName = "Single Shot"),
	EST_Auto UMETA(DisplayName = "Automatic Shot")
};

UENUM(BlueprintType)
enum class ESlotType : uint8
{
	// ������ ���� ���� ���� �̸�.
	ESLT_Primary UMETA(DisplayName = "First Slot"),
	ESLT_Secondary UMETA(DisplayName = "Second Slot"),
	ESLT_Special UMETA(DisplayName = "Third Slot"),
	ESLT_Special2 UMETA(DisplayName = "Forth Slot"),
	ESLT_Special3 UMETA(DisplayName = "Fifth Slot"),
	ESLT_Special4 UMETA(DisplayName = "Last Slot")
};

#pragma endregion

