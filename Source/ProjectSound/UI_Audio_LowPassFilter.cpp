// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_Audio_LowPassFilter.h"
#include "UIManager.h"
#include "PSButton.h"
#include "Components/Slider.h"
#include "Components/EditableText.h"
#include "Components/TextBlock.h"
#include "PS_Player.h"
#include "Kismet/GameplayStatics.h"
#include "PS_GameInstance.h"
#include "PS_Singleton.h"

void UUI_Audio_LowPassFilter::_OnCreate()
{
	Super::_OnCreate();

	SAFE_BIND_DELEGATE_LEFT_CLICKED(this, btn_lowpassfilter_apply, &UUI_Audio_LowPassFilter::_OnApplyLowPassFilter);
	SAFE_BIND_DELEGATE_LEFT_CLICKED(this, btn_lowpassfilter_remove, &UUI_Audio_LowPassFilter::_OnRemoveLowPassFilter);
	SAFE_BIND_DELEGATE_LEFT_CLICKED(this, btn_cutfrequency_minus, &UUI_Audio_LowPassFilter::_OnCutFrequencyClickMinus);
	SAFE_BIND_DELEGATE_LEFT_CLICKED(this, btn_cutfrequency_plus, &UUI_Audio_LowPassFilter::_OnCutFrequencyClickPlus);
	SAFE_BIND_DELEGATE_LEFT_CLICKED(this, btn_qfilter_minus, &UUI_Audio_LowPassFilter::_OnQFilterClickMinus);
	SAFE_BIND_DELEGATE_LEFT_CLICKED(this, btn_qfilter_plus, &UUI_Audio_LowPassFilter::_OnQFilterClickPlus);
	
	slider_cutfrequency->OnValueChanged.AddDynamic(this, &UUI_Audio_LowPassFilter::_OnCutFrequencySliderValueChange);
	slider_qfilter->OnValueChanged.AddDynamic(this, &UUI_Audio_LowPassFilter::_OnQFilterSliderValueChange);

}

void UUI_Audio_LowPassFilter::_OnShow()
{
	Super::_OnShow();
	this->AddToViewport();
	GetUIManager()->ApplyInputMode(EUIInputMode::GameAndUI);
}

void UUI_Audio_LowPassFilter::_OnWidgetCalledFromParent()
{
	//SAFE_BIND_DELEGATE_LEFT_CLICKED(this, btn_lowpassfilter_apply, &UUI_Audio_LowPassFilter::_OnApplyLowPassFilter);
	//SAFE_BIND_DELEGATE_LEFT_CLICKED(this, btn_lowpassfilter_remove, &UUI_Audio_LowPassFilter::_OnRemoveLowPassFilter);
	SAFE_BIND_DELEGATE_LEFT_CLICKED(this, btn_cutfrequency_minus, &UUI_Audio_LowPassFilter::_OnCutFrequencyClickMinus);
	SAFE_BIND_DELEGATE_LEFT_CLICKED(this, btn_cutfrequency_plus, &UUI_Audio_LowPassFilter::_OnCutFrequencyClickPlus);
	SAFE_BIND_DELEGATE_LEFT_CLICKED(this, btn_qfilter_minus, &UUI_Audio_LowPassFilter::_OnQFilterClickMinus);
	SAFE_BIND_DELEGATE_LEFT_CLICKED(this, btn_qfilter_plus, &UUI_Audio_LowPassFilter::_OnQFilterClickPlus);

	slider_cutfrequency->OnValueChanged.AddDynamic(this, &UUI_Audio_LowPassFilter::_OnCutFrequencySliderValueChange);
	slider_qfilter->OnValueChanged.AddDynamic(this, &UUI_Audio_LowPassFilter::_OnQFilterSliderValueChange);

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

void UUI_Audio_LowPassFilter::SetCutFrequencySliderValue(FString optionKey, float currentValue, float minValue, float maxValue)
{
	_CutFreqValue = currentValue;
	_MinCutFreqValue = minValue;
	_MaxCutFreqValue = maxValue;

	slider_cutfrequency->SetMinValue(_MinCutFreqValue);
	slider_cutfrequency->SetMaxValue(_MaxCutFreqValue);
	slider_cutfrequency->SetValue(_CutFreqValue);

	_CheckCutFrequencyValue(_CutFreqValue);
	edtxt_cutfreqency->SetText(FText::FromString(FString::Printf(TEXT("%.3f"), _CutFreqValue)));
	// �÷��̾������� ȣ��

}

void UUI_Audio_LowPassFilter::SetQFilterSliderValue(FString optionKey, float currentValue, float minValue, float maxValue)
{
	_QFilterValue = currentValue;
	_MinQFilterValue = minValue;
	_MaxQFilterValue = maxValue;

	slider_qfilter->SetMinValue(_MinQFilterValue);
	slider_qfilter->SetMaxValue(_MaxQFilterValue);
	slider_qfilter->SetValue(_QFilterValue);

	_CheckQFilterValue(_QFilterValue);
	edtxt_qfilter->SetText(FText::FromString(FString::Printf(TEXT("%.3f"), _QFilterValue)));
}


void UUI_Audio_LowPassFilter::_OnApplyLowPassFilter(UPSButton* sender)
{

}

void UUI_Audio_LowPassFilter::_OnRemoveLowPassFilter(UPSButton* sender)
{

}

void UUI_Audio_LowPassFilter::_OnCutFrequencySliderValueChange(float sliderValue)
{
}

void UUI_Audio_LowPassFilter::_OnCutFrequencyTxtChangeCommit(const FText& txtValue, ETextCommit::Type commitType)
{
}

void UUI_Audio_LowPassFilter::_OnCutFrequencyClickMinus(UPSButton* sender)
{
}

void UUI_Audio_LowPassFilter::_OnCutFrequencyClickPlus(UPSButton* sender)
{
}

void UUI_Audio_LowPassFilter::_OnQFilterSliderValueChange(float sliderValue)
{
}

void UUI_Audio_LowPassFilter::_OnQFilterTxtChangeCommit(const FText& txtValue, ETextCommit::Type commitType)
{
}

void UUI_Audio_LowPassFilter::_OnQFilterClickMinus(UPSButton* sender)
{
}

void UUI_Audio_LowPassFilter::_OnQFilterClickPlus(UPSButton* sender)
{

}

void UUI_Audio_LowPassFilter::_CheckCutFrequencyValue(float value)
{
	if (_CutFreqValue <= _MinCutFreqValue)
	{
		_CutFreqValue = _MinCutFreqValue;
		edtxt_cutfreqency->SetText(FText::FromString(FString::Printf(TEXT("%.3f"), _CutFreqValue)));
	}

	if (_CutFreqValue >= _MaxCutFreqValue)
	{
		_CutFreqValue = _MaxCutFreqValue;
		edtxt_cutfreqency->SetText(FText::FromString(FString::Printf(TEXT("%.3f"), _CutFreqValue)));
	}
}

void UUI_Audio_LowPassFilter::_CheckQFilterValue(float value)
{
	if (_QFilterValue <= _MinQFilterValue)
	{
		_QFilterValue = _MinQFilterValue;
		edtxt_qfilter->SetText(FText::FromString(FString::Printf(TEXT("%.3f"), _QFilterValue)));
	}

	if (_QFilterValue >= _MaxQFilterValue)
	{
		_QFilterValue = _MaxQFilterValue;
		edtxt_qfilter->SetText(FText::FromString(FString::Printf(TEXT("%.3f"), _QFilterValue)));
	}
}