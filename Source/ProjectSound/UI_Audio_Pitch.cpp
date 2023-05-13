// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_Audio_Pitch.h"
#include "UIManager.h"
#include "PSButton.h"
#include "Components/Slider.h"
#include "Components/EditableText.h"
#include "Components/TextBlock.h"
#include "PS_Player.h"
#include "Kismet/GameplayStatics.h"
#include "PS_GameInstance.h"
#include "PS_Singleton.h"

void UUI_Audio_Pitch::_OnCreate()
{
	Super::_OnCreate();

	SAFE_BIND_DELEGATE_LEFT_CLICKED(this, btn_pitch_minus, &UUI_Audio_Pitch::_OnClickMinus);
	SAFE_BIND_DELEGATE_LEFT_CLICKED(this, btn_pitch_plus, &UUI_Audio_Pitch::_OnClickPlus);

	slider_pitch->OnValueChanged.AddDynamic(this, &UUI_Audio_Pitch::_OnSliderValueChange);
	edtxt_pitch->OnTextCommitted.AddDynamic(this, &UUI_Audio_Pitch::_OnTxtChangeCommit);
}

void UUI_Audio_Pitch::_OnShow()
{
	Super::_OnShow();
	this->AddToViewport();
	GetUIManager()->ApplyInputMode(EUIInputMode::GameAndUI);
}

void UUI_Audio_Pitch::_OnWidgetCalledFromParent()
{
	SAFE_BIND_DELEGATE_LEFT_CLICKED(this, btn_pitch_minus, &UUI_Audio_Pitch::_OnClickMinus);
	SAFE_BIND_DELEGATE_LEFT_CLICKED(this, btn_pitch_plus, &UUI_Audio_Pitch::_OnClickPlus);

	slider_pitch->OnValueChanged.AddDynamic(this, &UUI_Audio_Pitch::_OnSliderValueChange);
	edtxt_pitch->OnTextCommitted.AddDynamic(this, &UUI_Audio_Pitch::_OnTxtChangeCommit);
	UPS_GameInstance* gInst = UPS_GameInstance::GetMyInstance();
	if (gInst == nullptr)
	{
		return;
	}
	UWorld* world = gInst->GetWorld();
	if (world == nullptr)
	{
		return;
	}
	playerPawn = Cast<APS_Player>(UGameplayStatics::GetPlayerPawn(world, 0));
}

void UUI_Audio_Pitch::SetSliderValue(FString optionKey, float currentValue, float minValue, float maxValue)
{
	_optionKey = optionKey;
	_Value = currentValue;
	_MinValue = minValue;
	_MaxValue = maxValue;

	slider_pitch->SetMinValue(_MinValue);
	slider_pitch->SetMaxValue(_MaxValue);
	slider_pitch->SetValue(_Value);

	_CheckValue(_Value);
	edtxt_pitch->SetText(FText::FromString(FString::Printf(TEXT("%.3f"), _Value)));
}

void UUI_Audio_Pitch::_OnSliderValueChange(float sliderValue)
{
	_Value = sliderValue;
	slider_pitch->SetValue(_Value);
	edtxt_pitch->SetText(FText::FromString(FString::Printf(TEXT("%.3f"), _Value)));

	_CheckValue(_Value);
	playerPawn->SetPitchMultiplier(_Value);
	OnOptionSlider.Broadcast(_optionKey, _Value);
}

void UUI_Audio_Pitch::_OnTxtChangeCommit(const FText& txtValue, ETextCommit::Type commitType)
{
	if (commitType != ETextCommit::Type::OnEnter)
	{
		return;
	}

	FString txt = txtValue.ToString();
	_Value = FCString::Atof(*txt.TrimQuotes());

	slider_pitch->SetValue(_Value);

	_CheckValue(_Value);
	playerPawn->SetPitchMultiplier(_Value);
	OnOptionSlider.Broadcast(_optionKey, _Value);
}

void UUI_Audio_Pitch::_OnClickMinus(UPSButton* sender)
{
	_Value -= .5f;
	slider_pitch->SetValue(_Value);
	edtxt_pitch->SetText(FText::FromString(FString::Printf(TEXT("%.3f"), _Value)));

	_CheckValue(_Value);
	playerPawn->SetPitchMultiplier(_Value);
	OnOptionSlider.Broadcast(_optionKey, _Value);
}

void UUI_Audio_Pitch::_OnClickPlus(UPSButton* sender)
{
	_Value += .5f;
	slider_pitch->SetValue(_Value);
	edtxt_pitch->SetText(FText::FromString(FString::Printf(TEXT("%.3f"), _Value)));

	_CheckValue(_Value);
	playerPawn->SetPitchMultiplier(_Value);
	OnOptionSlider.Broadcast(_optionKey, _Value);
}

void UUI_Audio_Pitch::_CheckValue(float value)
{
	if (_Value <= _MinValue)
	{
		_Value = _MinValue;
		edtxt_pitch->SetText(FText::FromString(FString::Printf(TEXT("%.3f"), _Value)));
	}

	if (_Value >= _MaxValue)
	{
		_Value = _MaxValue;
		edtxt_pitch->SetText(FText::FromString(FString::Printf(TEXT("%.3f"), _Value)));
	}
}
