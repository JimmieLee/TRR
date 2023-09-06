// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Engine/Canvas.h"
#include "CanvasItem.h"
#include "CharacterProperties.h"

// �׻� �������� ����
#include "ShooterHUD.generated.h"

/**
 * Terminator Re-Rampage Project
 * 2023. 09. 02
 * ������ ������ ���¸� ȭ�鿡 ǥ���ϴ�, Debug �뵵�� ����� HUD Ŭ������ ���
 */

UCLASS()
class TRR_API AShooterHUD : public AHUD
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AShooterHUD();

private:

	// HUD�� ǥ���� ������ ��Ʈ.
	UPROPERTY(Transient)
	UFont* MainFont = nullptr;

	float X = 50.0f;
	float Y = 50.0f;
	float LineHeight = 16.0f;
	float HorizontalOffset = 150.0f;

protected:	

	/** HUD�� ǥ���� ���� ó�� */
	// HUD�� Text�� �߰�.
	void AddText(const TCHAR* Title, const FText& Value);

	// HUD�� Float�� �߰�.
	void AddFloat(const TCHAR* Title, const float Value);
	
	// HUD�� Int�� �߰�.
	void AddInt(const TCHAR* Title, const int32 Value);

	// HUD�� bool ���θ� �߰�.
	void AddBool(const TCHAR* Title, const bool Value);

	// HUD�� ĳ������ �׼� ���¸� �߰�.
	void AddActionState(const TCHAR* Title, const ECharacterActionState Value);

	// String�� Text�� ��ȯ.
	FText StringToText(const TCHAR* Text);

	// Bool�� Text�� ��ȯ.
	FText BoolToText(bool Value);	

	/** HUD�� ������ ǥ�� */
	// HUD ĵ������ ���� �÷��� ���¸� ǥ��.
	void RenderStatics(const TCHAR* Title, const FText& Value, const FLinearColor& Color = FLinearColor::White);
	virtual void DrawHUD() override;
};
