// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Engine/Canvas.h"
#include "CanvasItem.h"
#include "CharacterProperties.h"

// 항상 마지막에 포함
#include "ShooterHUD.generated.h"

/**
 * Terminator Re-Rampage Project
 * 2023. 09. 02
 * 설정한 게임의 상태를 화면에 표시하는, Debug 용도로 사용할 HUD 클래스의 헤더
 */

UCLASS()
class TRR_API AShooterHUD : public AHUD
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AShooterHUD();

private:

	// HUD에 표시할 정보의 폰트.
	UPROPERTY(Transient)
	UFont* MainFont = nullptr;

	float X = 50.0f;
	float Y = 50.0f;
	float LineHeight = 16.0f;
	float HorizontalOffset = 150.0f;

protected:	

	/** HUD에 표시할 정보 처리 */
	// HUD에 Text를 추가.
	void AddText(const TCHAR* Title, const FText& Value);

	// HUD에 Float를 추가.
	void AddFloat(const TCHAR* Title, const float Value);
	
	// HUD에 Int를 추가.
	void AddInt(const TCHAR* Title, const int32 Value);

	// HUD에 bool 여부를 추가.
	void AddBool(const TCHAR* Title, const bool Value);

	// HUD에 캐릭터의 액션 상태를 추가.
	void AddActionState(const TCHAR* Title, const ECharacterActionState Value);

	// String을 Text로 변환.
	FText StringToText(const TCHAR* Text);

	// Bool을 Text로 변환.
	FText BoolToText(bool Value);	

	/** HUD에 정보를 표시 */
	// HUD 캔버스에 게임 플레이 상태를 표시.
	void RenderStatics(const TCHAR* Title, const FText& Value, const FLinearColor& Color = FLinearColor::White);
	virtual void DrawHUD() override;
};
