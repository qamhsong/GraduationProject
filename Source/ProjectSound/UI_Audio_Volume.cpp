// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_Audio_Volume.h"
#include "UIManager.h"
#include "PSButton.h"
#include "Components/Slider.h"
#include "Components/EditableText.h"
#include "Components/TextBlock.h"
#include "PS_Player.h"
#include "Kismet/GameplayStatics.h"
#include "PS_GameInstance.h"
#include "PS_Singleton.h"


void UUI_Audio_Volume::_OnCreate()
{
	Super::_OnCreate();

	SAFE_BIND_DELEGATE_LEFT_CLICKED(this, btn_vol_minus, &UUI_Audio_Volume::_OnClickMinus);
	SAFE_BIND_DELEGATE_LEFT_CLICKED(this, btn_vol_plus, &UUI_Audio_Volume::_OnClickPlus);

	slider_vol->OnValueChanged.AddDynamic(this, &UUI_Audio_Volume::_OnSliderValueChange);
	edtxt_vol->OnTextCommitted.AddDynamic(this, &UUI_Audio_Volume::_OnTxtChangeCommit);
}

void UUI_Audio_Volume::_OnShow()
{
	Super::_OnShow();
	this->AddToViewport();
	GetUIManager()->ApplyInputMode(EUIInputMode::GameAndUI);
}

void UUI_Audio_Volume::_OnWidgetCalledFromParent()
{
	SAFE_BIND_DELEGATE_LEFT_CLICKED(this, btn_vol_minus, &UUI_Audio_Volume::_OnClickMinus);
	SAFE_BIND_DELEGATE_LEFT_CLICKED(this, btn_vol_plus, &UUI_Audio_Volume::_OnClickPlus);

	slider_vol->OnValueChanged.AddDynamic(this, &UUI_Audio_Volume::_OnSliderValueChange);
	edtxt_vol->OnTextCommitted.AddDynamic(this, &UUI_Audio_Volume::_OnTxtChangeCommit);
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

void UUI_Audio_Volume::SetSliderValue(FString optionKey, float currentValue, float minValue, float maxValue)
{
	_optionKey = optionKey;
	_Value = currentValue;
	_MinValue = minValue;
	_MaxValue = maxValue;

	slider_vol->SetMinValue(_MinValue);
	slider_vol->SetMaxValue(_MaxValue);
	slider_vol->SetValue(_Value);

	_CheckValue(_Value);
	edtxt_vol->SetText(FText::FromString(FString::Printf(TEXT("%.3f"), _Value)));
}

void UUI_Audio_Volume::_OnSliderValueChange(float sliderValue)
{
	_Value = sliderValue;
	slider_vol->SetValue(_Value);
	edtxt_vol->SetText(FText::FromString(FString::Printf(TEXT("%.3f"), _Value)));

	_CheckValue(_Value);
	playerPawn->SetVolumeMultiplier(_Value);
	OnOptionSlider.Broadcast(_optionKey, _Value);
}

void UUI_Audio_Volume::_OnTxtChangeCommit(const FText& txtValue, ETextCommit::Type commitType)
{
	if (commitType != ETextCommit::Type::OnEnter)
	{
		return;
	}

	FString txt = txtValue.ToString();
	_Value = FCString::Atof(*txt.TrimQuotes());

	slider_vol->SetValue(_Value);

	_CheckValue(_Value);
	playerPawn->SetVolumeMultiplier(_Value);
	OnOptionSlider.Broadcast(_optionKey, _Value);
}

void UUI_Audio_Volume::_OnClickMinus(UPSButton* sender)
{
	_Value -= .5f;
	slider_vol->SetValue(_Value);
	edtxt_vol->SetText(FText::FromString(FString::Printf(TEXT("%.3f"), _Value)));

	_CheckValue(_Value);
	playerPawn->SetVolumeMultiplier(_Value);
	OnOptionSlider.Broadcast(_optionKey, _Value);
}

void UUI_Audio_Volume::_OnClickPlus(UPSButton* sender)
{
	_Value += .5f;
	slider_vol->SetValue(_Value);
	edtxt_vol->SetText(FText::FromString(FString::Printf(TEXT("%.3f"), _Value)));

	_CheckValue(_Value);
	playerPawn->SetVolumeMultiplier(_Value);
	OnOptionSlider.Broadcast(_optionKey, _Value);
}

void UUI_Audio_Volume::_CheckValue(float value)
{
	if (_Value <= _MinValue)
	{
		_Value = _MinValue;
		edtxt_vol->SetText(FText::FromString(FString::Printf(TEXT("%.3f"), _Value)));
	}

	if (_Value >= _MaxValue)
	{
		_Value = _MaxValue;
		edtxt_vol->SetText(FText::FromString(FString::Printf(TEXT("%.3f"), _Value)));
	}
}
