// Fill out your copyright notice in the Description page of Project Settings.

// 커스텀 클래스 포함
#include "ShooterPlayerController.h"

// 향상된 입력 포함
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

/**
* Terminator Re-Rampage Project
* 2023. 08. 29
* 플레이어가 제어하는 캐릭터의 고유 컨트롤러 클래스의 CPP
*/

void AShooterPlayerController::BeginPlay()
{
	// 기본 클래스를 호출
	Super::BeginPlay();
		
	// 입력 매핑 컨텍스트를 추가.
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(MovementMappingContext, 0);
	}	
}
