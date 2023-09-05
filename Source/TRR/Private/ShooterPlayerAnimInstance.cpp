// Fill out your copyright notice in the Description page of Project Settings.

// 커스텀 클래스 포함
#include "ShooterPlayerAnimInstance.h"
#include "ShooterCharacter.h"

/*
 * Terminator Re-Rampage Project
 * 2023. 09 .01
 * 플레이어 캐릭터의 애니메이션 정보 클래스의 CPP
*/

UShooterPlayerAnimInstance::UShooterPlayerAnimInstance()
{
	//...
}

void UShooterPlayerAnimInstance::NativeInitializeAnimation()
{
	// 캐릭터 클래스를 형변환하여 캐릭터 참조를 구한다.
	Shooter = Cast<AShooterCharacter>(TryGetPawnOwner());
}
