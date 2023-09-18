#pragma once

/*
* Terminator Re-Rampage Project
* 2023. 08. 31
* ���� �÷��� ��Ȳ�� ���� ��ȭ�ϴ� ĳ������ ���� ����.
*/

#pragma region Character Action State
UENUM(BlueprintType)
enum class ECharacterActionState : uint8
{
	ECAS_IDLE UMETA(DisplayName = "Idle"),
	ECAS_JUMP UMETA(DisplayName = "Jump"),
	ECAS_CROUCH UMETA(DisplayName = "Crouch"),
	ECAS_SPRINT UMETA(DisplayName = "Sprint"),
	ECAS_DASH UMETA(DisplayName = "Dash")
};

#pragma endregion

