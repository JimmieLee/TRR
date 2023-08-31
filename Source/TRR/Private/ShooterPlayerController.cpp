// Fill out your copyright notice in the Description page of Project Settings.

// Ŀ���� Ŭ���� ����
#include "ShooterPlayerController.h"

// ���� �Է� ����
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

/**
* Terminator Re-Rampage Project
* 2023. 08. 29
* �÷��̾ �����ϴ� ĳ������ ���� ��Ʈ�ѷ� Ŭ������ CPP
*/

void AShooterPlayerController::BeginPlay()
{
	// �⺻ Ŭ������ ȣ��
	Super::BeginPlay();
		
	// �Է� ���� ���ؽ�Ʈ�� �߰�.
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(MovementMappingContext, 0);
	}	
}
