// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_Audio_HighPassFilter.h"
#include "UIManager.h"
#include "PSButton.h"
#include "Components/Slider.h"
#include "Components/EditableText.h"
#include "Components/TextBlock.h"
#include "PS_Player.h"
#include "Kismet/GameplayStatics.h"
#include "PS_GameInstance.h"
#include "PS_Singleton.h"


void UUI_Audio_HighPassFilter::_OnCreate()
{
	Super::_OnCreate();

	SAFE_BIND_DELEGATE_LEFT_CLICKED(this, btn_highpassfilter_apply, &UUI_Audio_HighPassFilter::_OnApplyHighPassFilter);
	SAFE_BIND_DELEGATE_LEFT_CLICKED(this, btn_cutfrequency_minus, &UUI_Audio_HighPassFilter::_OnCutFrequencyClickMinus);
	SAFE_BIND_DELEGATE_LEFT_CLICKED(this, btn_cutfrequency_plus, &UUI_Audio_HighPassFilter::_OnCutFrequencyClickPlus);
	SAFE_BIND_DELEGATE_LEFT_CLICKED(this, btn_qfilter_minus, &UUI_Audio_HighPassFilter::_OnQFilterClickMinus);
	SAFE_BIND_DELEGATE_LEFT_CLICKED(this, btn_qfilter_plus, &UUI_Audio_HighPassFilter::_OnQFilterClickPlus);

	slider_cutfrequency->OnValueChanged.AddDynamic(this, &UUI_Audio_HighPassFilter::_OnCutFrequencySliderValueChange);
	slider_qfilter->OnValueChanged.AddDynamic(this, &UUI_Audio_HighPassFilter::_OnQFilterSliderValueChange);
	edtxt_cutfrequency->OnTextCommitted.AddDynamic(this, &UUI_Audio_HighPassFilter::_OnCutFrequencyTxtChangeCommit);
	edtxt_qfilter->OnTextCommitted.AddDynamic(this, &UUI_Audio_HighPassFilter::_OnQFilterTxtChangeCommit);
}

void UUI_Audio_HighPassFilter::_OnShow()
{
	Super::_OnShow();
	this->AddToViewport();
	GetUIManager()->ApplyInputMode(EUIInputMode::GameAndUI);
}

void UUI_Audio_HighPassFilter::_OnWidgetCalledFromParent()
{
	SAFE_BIND_DELEGATE_LEFT_CLICKED(this, btn_highpassfilter_apply, &UUI_Audio_HighPassFilter::_OnApplyHighPassFilter);
	SAFE_BIND_DELEGATE_LEFT_CLICKED(this, btn_cutfrequency_minus, &UUI_Audio_HighPassFilter::_OnCutFrequencyClickMinus);
	SAFE_BIND_DELEGATE_LEFT_CLICKED(this, btn_cutfrequency_plus, &UUI_Audio_HighPassFilter::_OnCutFrequencyClickPlus);
	SAFE_BIND_DELEGATE_LEFT_CLICKED(this, btn_qfilter_minus, &UUI_Audio_HighPassFilter::_OnQFilterClickMinus);
	SAFE_BIND_DELEGATE_LEFT_CLICKED(this, btn_qfilter_plus, &UUI_Audio_HighPassFilter::_OnQFilterClickPlus);

	slider_cutfrequency->OnValueChanged.AddDynamic(this, &UUI_Audio_HighPassFilter::_OnCutFrequencySliderValueChange);
	slider_qfilter->OnValueChanged.AddDynamic(this, &UUI_Audio_HighPassFilter::_OnQFilterSliderValueChange);

	edtxt_cutfrequency->OnTextCommitted.AddDynamic(this, &UUI_Audio_HighPassFilter::_OnCutFrequencyTxtChangeCommit);
	edtxt_qfilter->OnTextCommitted.AddDynamic(this, &UUI_Audio_HighPassFilter::_OnQFilterTxtChangeCommit);

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

	_CutFreqValue = 800.f;
	_QFilterValue = 10.f;
}


void UUI_Audio_HighPassFilter::SetCutFrequencySliderValue(FString optionKey, float currentValue, float minValue, float maxValue)
{
	_CutFreqValue = currentValue;
	_MinCutFreqValue = minValue;
	_MaxCutFreqValue = maxValue;

	slider_cutfrequency->SetMinValue(_MinCutFreqValue);
	slider_cutfrequency->SetMaxValue(_MaxCutFreqValue);
	slider_cutfrequency->SetValue(_CutFreqValue);

	_CheckCutFrequencyValue(_CutFreqValue);
	edtxt_cutfrequency->SetText(FText::FromString(FString::Printf(TEXT("%.3f"),  _CutFreqValue)));
	playerPawn->HighPassFilterSettings(_CutFreqValue, _QFilterValue);
}

void UUI_Audio_HighPassFilter::SetQFilterSliderValue(FString optionKey, float currentValue, float minValue, float maxValue)
{
	_QFilterValue = currentValue;
	_MinQFilterValue = minValue;
	_MaxQFilterValue = maxValue;

	slider_qfilter->SetMinValue(_MinQFilterValue);
	slider_qfilter->SetMaxValue(_MaxQFilterValue);
	slider_qfilter->SetValue(_QFilterValue);

	_CheckQFilterValue(_QFilterValue);
	edtxt_qfilter->SetText(FText::FromString(FString::Printf(TEXT("%.3f"), _QFilterValue)));
	playerPawn->HighPassFilterSettings(_CutFreqValue, _QFilterValue);
}

void UUI_Audio_HighPassFilter::_OnApplyHighPassFilter(UPSButton* sender)
{
	if (playerPawn == nullptr)
	{
		return;
	}
	if (_bHighPassFilterApplyState == true)
	{
		playerPawn->RemoveSourceChainEffect(EEffectPreset::EFilterHigh);
		txt_highpassfilter_state->SetText(FText::FromString(FString("APPLY")));
		UE_LOG(LogTemp, Warning, TEXT("HighPass Filter Removed"));
		_bHighPassFilterApplyState = false;
	}
	else
	{
		playerPawn->HighPassFilterSettings(_CutFreqValue, _QFilterValue);
		playerPawn->RegisterSourceChainEffect(EEffectPreset::EFilterHigh);
		txt_highpassfilter_state->SetText(FText::FromString(FString("REMOVE")));
		_bHighPassFilterApplyState = true;
	}
}


void UUI_Audio_HighPassFilter::_OnCutFrequencySliderValueChange(float sliderValue)
{
	_CutFreqValue = sliderValue;
	slider_cutfrequency->SetValue(_CutFreqValue);
	edtxt_cutfrequency->SetText(FText::FromString(FString::Printf(TEXT("%.3f"), _CutFreqValue)));

	_CheckCutFrequencyValue(_CutFreqValue);
	playerPawn->HighPassFilterSettings(_CutFreqValue,_QFilterValue);
}

void UUI_Audio_HighPassFilter::_OnCutFrequencyTxtChangeCommit(const FText& txtValue, ETextCommit::Type commitType)
{
	if (commitType != ETextCommit::OnEnter)
	{
		return;
	}

	FString txt = txtValue.ToString();
	_CutFreqValue = FCString::Atof(*txt.TrimQuotes());

	slider_cutfrequency->SetValue(_CutFreqValue);

	_CheckCutFrequencyValue(_CutFreqValue);
	playerPawn->HighPassFilterSettings(_CutFreqValue, _QFilterValue);

}

void UUI_Audio_HighPassFilter::_OnCutFrequencyClickMinus(UPSButton* sender)
{
	_CutFreqValue -=.5f;
	slider_cutfrequency->SetValue(_CutFreqValue);
	edtxt_cutfrequency->SetText(FText::FromString(FString::Printf(TEXT("%.3f"), _CutFreqValue)));

	_CheckCutFrequencyValue(_CutFreqValue);
	playerPawn->HighPassFilterSettings(_CutFreqValue, _QFilterValue);
}

void UUI_Audio_HighPassFilter::_OnCutFrequencyClickPlus(UPSButton* sender)
{
	_CutFreqValue += .5f;
	slider_cutfrequency->SetValue(_CutFreqValue);
	edtxt_cutfrequency->SetText(FText::FromString(FString::Printf(TEXT("%.3f"), _CutFreqValue)));

	_CheckCutFrequencyValue(_CutFreqValue);
	playerPawn->HighPassFilterSettings(_CutFreqValue, _QFilterValue);
}

void UUI_Audio_HighPassFilter::_OnQFilterSliderValueChange(float sliderValue)
{
	_QFilterValue = sliderValue;
	slider_qfilter->SetValue(_QFilterValue);
	edtxt_qfilter->SetText(FText::FromString(FString::Printf(TEXT("%.3f"), _QFilterValue)));

	_CheckQFilterValue(_QFilterValue);
	playerPawn->LowPassFilterSettings(_CutFreqValue, _QFilterValue);
}

void UUI_Audio_HighPassFilter::_OnQFilterTxtChangeCommit(const FText& txtValue, ETextCommit::Type commitType)
{
	if (commitType != ETextCommit::OnEnter)
	{
		return;
	}

	FString txt = txtValue.ToString();
	_QFilterValue = FCString::Atof(*txt.TrimQuotes());

	slider_qfilter->SetValue(_QFilterValue);

	_CheckQFilterValue(_QFilterValue);
	playerPawn->LowPassFilterSettings(_CutFreqValue, _QFilterValue);
}

void UUI_Audio_HighPassFilter::_OnQFilterClickMinus(UPSButton* sender)
{
	_QFilterValue -= .5f;
	slider_qfilter->SetValue(_CutFreqValue);
	edtxt_qfilter->SetText(FText::FromString(FString::Printf(TEXT("%.3f"), _QFilterValue)));

	_CheckQFilterValue(_QFilterValue);
	playerPawn->LowPassFilterSettings(_CutFreqValue, _QFilterValue);
}

void UUI_Audio_HighPassFilter::_OnQFilterClickPlus(UPSButton* sender)
{
	_QFilterValue += .5f;
	slider_qfilter->SetValue(_CutFreqValue);
	edtxt_qfilter->SetText(FText::FromString(FString::Printf(TEXT("%.3f"), _QFilterValue)));

	_CheckQFilterValue(_QFilterValue);
	playerPawn->LowPassFilterSettings(_CutFreqValue, _QFilterValue);
}

void UUI_Audio_HighPassFilter::_CheckCutFrequencyValue(float value)
{
	if (_CutFreqValue <= _MinCutFreqValue)
	{
		_CutFreqValue = _MinCutFreqValue;
		edtxt_cutfrequency->SetText(FText::FromString(FString::Printf(TEXT("%.3f"), _CutFreqValue)));
	}

	if (_CutFreqValue >= _MaxCutFreqValue)
	{
		_CutFreqValue = _MaxCutFreqValue;
		edtxt_cutfrequency->SetText(FText::FromString(FString::Printf(TEXT("%.3f"), _CutFreqValue)));
	}
}

void UUI_Audio_HighPassFilter::_CheckQFilterValue(float value)
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
