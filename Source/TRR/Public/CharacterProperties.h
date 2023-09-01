#pragma once

/*
* Terminator Re-Rampage Project
* 2023. 08. 31
* 게임 플레이 상황에 따라 변화하는 캐릭터의 상태 선언.
*/

UENUM(BlueprintType)
enum class ECharacterActionState : uint8
{
	ECAS_IDLE UMETA(DisplayName = "Idle"),
	ECAS_JUMP UMETA(DisplayName = "Jump"),
	ECAS_CROUCH UMETA(DisplayName = "Crouch"),
	ECAS_SPRINT UMETA(DisplayName = "Sprint")
};

