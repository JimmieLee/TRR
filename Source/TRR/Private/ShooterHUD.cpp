// Fill out your copyright notice in the Description page of Project Settings.

// Ŀ���� Ŭ���� ����.
#include "ShooterHUD.h"
#include "ShooterCharacter.h"
#include "ShooterItem.h"

// ������Ʈ Ŭ���� ����.
#include "Camera/CameraComponent.h"

// ���� �����ӿ�ũ ����.
#include "GameFramework/CharacterMovementComponent.h"


/**
 * Terminator Re-Rampage Project
 * 2023. 09. 02
 * ������ ������ ���¸� ȭ�鿡 ǥ���ϴ�, Debug �뵵�� ����� Ŭ������ CPP
 */

AShooterHUD::AShooterHUD()
{
	// HUD�� ǥ�õǴ� ������ ��Ʈ ������Ʈ�� �����Ѵ�.
	static ConstructorHelpers::FObjectFinder<UFont> TempFont(TEXT("/Engine/EngineFonts/Roboto"));

	if (TempFont.Succeeded())
	{
		MainFont = TempFont.Object;
	}	
}

/** ǥ���� ������ �����Ѵ�. */
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

/** ������ ȭ�鿡 ǥ�� */
void AShooterHUD::RenderStatics(const TCHAR* Title, const FText& Value, const FLinearColor& Color)
{
	// ������ ������ ��ġ, ��Ʈ, ������ ����.
	FCanvasTextItem item0(FVector2D(X, Y), StringToText(Title), MainFont, FLinearColor::White);
	item0.EnableShadow(FLinearColor(0.0f, 0.0f, 0.0f));
	Canvas->DrawItem(item0);

	// ������ ���� ��ġ ��Ʈ, ������ ����.
	// ������ �����ʿ� ǥ���ϱ� ����, X�࿡ Offset�� �ջ�.
	FCanvasTextItem item1(FVector2D(X + HorizontalOffset, Y), Value, MainFont, Color);
	item1.EnableShadow(FLinearColor(0.0f, 0.0f, 0.0f));
	Canvas->DrawItem(item1);

	// Y���� �Ʒ��� �̵� (���� ������ ������������ ǥ��.
	Y += LineHeight;
}

void AShooterHUD::DrawHUD()
{
	Super::DrawHUD();

	// ������ ǥ�õ� ��ġ�� ����.
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
