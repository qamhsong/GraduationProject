// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_Audio.h"
#include "UIManager.h"
#include "PSButton.h"
#include "Components/Slider.h"
#include "Components/EditableText.h"
#include "Components/TextBlock.h"

void UUI_Audio::_OnCreate()
{
	Super::_OnCreate();

	SAFE_BIND_DELEGATE_LEFT_CLICKED(this, pitchBtnMinus, &UUI_Audio::_OnClickMinus);
	SAFE_BIND_DELEGATE_LEFT_CLICKED(this, pitchBtnPlus, &UUI_Audio::_OnClickPlus);

	PitchValue->OnValueChanged.AddDynamic(this, &UUI_Audio::_OnSliderValueChange);
	PitchTxtValue->OnTextCommitted.AddDynamic(this, &UUI_Audio::_OnTxtChangeCommit);
}

void UUI_Audio::_OnShow()
{
	Super::_OnShow();
	this->AddToViewport();
	GetUIManager()->ApplyInputMode(EUIInputMode::GameAndUI);
}

void UUI_Audio::SetSliderValue(FString optionKey, float currentValue, float minValue, float maxValue)
{
	_optionKey = optionKey;
	_Value =  currentValue;
	_MinValue = minValue;
	_MaxValue =  maxValue;

	PitchValue->SetMinValue(_MinValue);
	PitchValue->SetMaxValue(_MaxValue);
	PitchValue->SetValue(_Value);

	_CheckValue(_Value);
	PitchTxtValue->SetText(FText::FromString(FString::Printf(TEXT("%.3f"), _Value)));

}

void UUI_Audio::_OnSliderValueChange(float sliderValue)
{
	_Value = sliderValue;
	PitchValue->SetValue(_Value);
	PitchTxtValue->SetText(FText::FromString(FString::Printf(TEXT("%.3f"), _Value)));

	_CheckValue(_Value);
	OnOptionSlider.Broadcast(_optionKey, _Value);
}

void UUI_Audio::_OnTxtChangeCommit(const FText& txtValue, ETextCommit::Type commitType)
{
	if (commitType != ETextCommit::Type::OnEnter)
	{
		return;
	}

	FString txt = txtValue.ToString();
	_Value = FCString::Atof(*txt.TrimQuotes());

	PitchValue->SetValue(_Value);
	
	_CheckValue(_Value);
	OnOptionSlider.Broadcast(_optionKey, _Value);
}

void UUI_Audio::_OnClickMinus(UPSButton* sender)
{
	_Value = .5f;
	PitchValue->SetValue(_Value);
	PitchTxtValue->SetText(FText::FromString(FString::Printf(TEXT("%.3f"), _Value)));

	_CheckValue(_Value);
	OnOptionSlider.Broadcast(_optionKey, _Value);
}

void UUI_Audio::_OnClickPlus(UPSButton* sender)
{
	_Value += .5f;
	PitchValue->SetValue(_Value);
	PitchTxtValue->SetText(FText::FromString(FString::Printf(TEXT("%.3f"), _Value)));

	_CheckValue(_Value);
	OnOptionSlider.Broadcast(_optionKey, _Value);
}

void UUI_Audio::_CheckValue(float value)
{
	if (_Value <= _MinValue)
	{
		_Value  = _MinValue;
		PitchTxtValue->SetText(FText::FromString(FString::Printf(TEXT("%.3f"), _Value)));
	}

	if (_Value >= _MaxValue)
	{
		_Value = _MaxValue;
		PitchTxtValue->SetText(FText::FromString(FString::Printf(TEXT("%.3f"), _Value)));
	}
}

