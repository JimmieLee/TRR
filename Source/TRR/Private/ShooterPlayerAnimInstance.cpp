// Fill out your copyright notice in the Description page of Project Settings.

// Ŀ���� Ŭ���� ����
#include "ShooterPlayerAnimInstance.h"
#include "ShooterCharacter.h"

/*
 * Terminator Re-Rampage Project
 * 2023. 09 .01
 * �÷��̾� ĳ������ �ִϸ��̼� ���� Ŭ������ CPP
*/

UShooterPlayerAnimInstance::UShooterPlayerAnimInstance()
{
	//...
}

void UShooterPlayerAnimInstance::NativeInitializeAnimation()
{
	// ĳ���� Ŭ������ ����ȯ�Ͽ� ĳ���� ������ ���Ѵ�.
	Shooter = Cast<AShooterCharacter>(TryGetPawnOwner());
}
