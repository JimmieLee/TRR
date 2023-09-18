// Fill out your copyright notice in the Description page of Project Settings.

// 커스텀 클래스 포함.
#include "ShooterHUD.h"
#include "ShooterCharacter.h"
#include "ShooterItem.h"

// 컴포넌트 클래스 포함.
#include "Camera/CameraComponent.h"

// 게임 프레임워크 포함.
#include "GameFramework/CharacterMovementComponent.h"


/**
 * Terminator Re-Rampage Project
 * 2023. 09. 02
 * 설정한 게임의 상태를 화면에 표시하는, Debug 용도로 사용할 클래스의 CPP
 */

AShooterHUD::AShooterHUD()
{
	// HUD에 표시되는 정보의 폰트 오브젝트를 설정한다.
	static ConstructorHelpers::FObjectFinder<UFont> TempFont(TEXT("/Engine/EngineFonts/Roboto"));

	if (TempFont.Succeeded())
	{
		MainFont = TempFont.Object;
	}	
}

/** 표시할 정보를 포함한다. */
void AShooterHUD::AddText(const TCHAR* Title, const FText& Value){ RenderStatics(Title, Value); }

void AShooterHUD::AddFloat(const TCHAR* Title, const float Value){ RenderStatics(Title, FText::AsNumber(Value)); }

void AShooterHUD::AddInt(const TCHAR* Title, const int32 Value){ RenderStatics(Title, FText::AsNumber(Value)); }

void AShooterHUD::AddBool(const TCHAR* Title, const bool Value)
{ 
	RenderStatics(Title, BoolToText(Value), (Value == false) ? FLinearColor::Red : FLinearColor::Green); 
}

void AShooterHUD::AddActionState(const TCHAR* Title, const ECharacterActionState Value)
{
	const UEnum* StateValue = FindObject<UEnum>(ANY_PACKAGE, TEXT("ECharacterActionState"), true);

	if (StateValue)
	{
		FName EnumToName = StateValue->GetNameByValue((uint8)Value);
		RenderStatics(Title, FText(StringToText(*EnumToName.ToString())), FLinearColor::Blue);
	}		
}

FText AShooterHUD::StringToText(const TCHAR* Text){ return FText::FromString(Text); }

FText AShooterHUD::BoolToText(bool Value)
{
	return StringToText((Value == true) ? TEXT("true") : TEXT("false"));
}

/** 정보를 화면에 표시 */
void AShooterHUD::RenderStatics(const TCHAR* Title, const FText& Value, const FLinearColor& Color)
{
	// 정보의 제목의 위치, 폰트, 색상을 설정.
	FCanvasTextItem item0(FVector2D(X, Y), StringToText(Title), MainFont, FLinearColor::White);
	item0.EnableShadow(FLinearColor(0.0f, 0.0f, 0.0f));
	Canvas->DrawItem(item0);

	// 정보의 값의 위치 폰트, 색상을 설정.
	// 제목의 오른쪽에 표시하기 위해, X축에 Offset을 합산.
	FCanvasTextItem item1(FVector2D(X + HorizontalOffset, Y), Value, MainFont, Color);
	item1.EnableShadow(FLinearColor(0.0f, 0.0f, 0.0f));
	Canvas->DrawItem(item1);

	// Y축을 아래로 이동 (다음 정보를 오름차순으로 표시.
	Y += LineHeight;
}

void AShooterHUD::DrawHUD()
{
	Super::DrawHUD();

	// 정보가 표시될 위치를 설정.
	X = Y = 1000.0f;

	AShooterCharacter* Character = Cast<AShooterCharacter>(GetOwningPawn());

	if (Character)
	{
		AddFloat(L"Move Speed: ", Character->GetVelocity().Size() / 100.0f);
		AddActionState(L"Action State: ", Character->GetActionState());
		AddFloat(L"Character Height: ", Character->GetShooterCamera()->GetRelativeLocation().Z);
		AddFloat(L"Move Speed: ", Character->GetCharacterMovement()->MaxWalkSpeed);
		AddFloat(L"Stamina: ", Character->GetStamina());
		AddBool(L"IsFalling? ", Character->GetCharacterMovement()->IsFalling());
		
	}
}
