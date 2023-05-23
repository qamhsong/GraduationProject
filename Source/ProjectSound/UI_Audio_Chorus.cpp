// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_Audio_Chorus.h"
#include "UIManager.h"
#include "PSButton.h"
#include "Components/Slider.h"
#include "Components/EditableText.h"
#include "Components/TextBlock.h"
#include "PS_Player.h"
#include "Kismet/GameplayStatics.h"
#include "PS_GameInstance.h"
#include "PS_Singleton.h"

void UUI_Audio_Chorus::_OnCreate()
{
	Super::_OnCreate();
}

void UUI_Audio_Chorus::_OnShow()
{
	Super::_OnShow();
}

void UUI_Audio_Chorus::_OnWidgetCalledFromParent()
{
	SAFE_BIND_DELEGATE_LEFT_CLICKED(this, btn_chorus_apply, &UUI_Audio_Chorus::_OnApplyChorus);
	SAFE_BIND_DELEGATE_LEFT_CLICKED(this, btn_depth_minus, &UUI_Audio_Chorus::_OnDepthClickMinus);
	SAFE_BIND_DELEGATE_LEFT_CLICKED(this, btn_depth_plus, &UUI_Audio_Chorus::_OnDepthClickPlus);
	SAFE_BIND_DELEGATE_LEFT_CLICKED(this, btn_frequency_minus, &UUI_Audio_Chorus::_OnFrequencyClickMinus);
	SAFE_BIND_DELEGATE_LEFT_CLICKED(this, btn_frequency_plus, &UUI_Audio_Chorus::_OnFrequencyClickPlus);
	SAFE_BIND_DELEGATE_LEFT_CLICKED(this, btn_feedback_minus, &UUI_Audio_Chorus::_OnFeedbackClickMinus);
	SAFE_BIND_DELEGATE_LEFT_CLICKED(this, btn_feedback_plus, &UUI_Audio_Chorus::_OnFeedbackClickPlus);
	SAFE_BIND_DELEGATE_LEFT_CLICKED(this, btn_wetlevel_minus, &UUI_Audio_Chorus::_OnWetLevelClickMinus);
	SAFE_BIND_DELEGATE_LEFT_CLICKED(this, btn_wetlevel_plus, &UUI_Audio_Chorus::_OnWetLevelClickPlus);
	SAFE_BIND_DELEGATE_LEFT_CLICKED(this, btn_drylevel_minus, &UUI_Audio_Chorus::_OnDryLevelClickMinus);
	SAFE_BIND_DELEGATE_LEFT_CLICKED(this, btn_drylevel_plus, &UUI_Audio_Chorus::_OnDryLevelClickPlus);
	SAFE_BIND_DELEGATE_LEFT_CLICKED(this, btn_spread_minus, &UUI_Audio_Chorus::_OnSpreadClickMinus);
	SAFE_BIND_DELEGATE_LEFT_CLICKED(this, btn_spread_plus, &UUI_Audio_Chorus::_OnSpreadClickPlus);

	slider_depth->OnValueChanged.AddDynamic(this, &UUI_Audio_Chorus::_OnDepthSliderValueChange);
	slider_frequency->OnValueChanged.AddDynamic(this, &UUI_Audio_Chorus::_OnFrequencySliderValueChange);
	slider_feedback->OnValueChanged.AddDynamic(this, &UUI_Audio_Chorus::_OnFeedbackSliderValueChange);
	slider_wetlevel->OnValueChanged.AddDynamic(this, &UUI_Audio_Chorus::_OnWetLevelSliderValueChange);
	slider_drylevel->OnValueChanged.AddDynamic(this, &UUI_Audio_Chorus::_OnDryLevelSliderValueChange);
	slider_spread->OnValueChanged.AddDynamic(this, &UUI_Audio_Chorus::_OnSpreadSliderValueChange);

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

	_DepthValue = 0.f;
	_FrequencyValue = 2.f;
	_FeedbackValue = 0.3f;
	_WetLevelValue = 0.5f;
	_DryLevelValue = 0.5f;
	_SpreadValue = 1.f;

}

void UUI_Audio_Chorus::SetDepthSliderValue(float currentValue, float minValue, float maxValue)
{
	_DepthValue = currentValue;
	_MinDepthValue = minValue;
	_MaxDepthValue = maxValue;

	slider_depth->SetMinValue(_MinDepthValue);
	slider_depth->SetMaxValue(_MaxDepthValue);
	slider_depth->SetValue(_DepthValue);

	_CheckDepthValue(_DepthValue);
	playerPawn->ChorusSettings(_DepthValue, _FrequencyValue, _FeedbackValue, _WetLevelValue, _DryLevelValue, _SpreadValue);
}

void UUI_Audio_Chorus::SetFrequencySliderValue(float currentValue, float minValue, float maxValue)
{
	_FrequencyValue = currentValue;
	_MinFrequencyValue = minValue;
	_MaxFrequencyValue = maxValue;

	slider_frequency->SetMinValue(_MinFrequencyValue);
	slider_frequency->SetMaxValue(_MaxFrequencyValue);
	slider_frequency->SetValue(_FrequencyValue);

	_CheckFrequencyValue(_FrequencyValue);
	playerPawn->ChorusSettings(_DepthValue, _FrequencyValue, _FeedbackValue, _WetLevelValue, _DryLevelValue, _SpreadValue);
}

void UUI_Audio_Chorus::SetFeedbackSliderValue(float currentValue, float minValue, float maxValue)
{
	_FeedbackValue = currentValue;
	_MinFeedbackValue = minValue;
	_MaxFeedbackValue = maxValue;

	slider_feedback->SetMinValue(_MinFeedbackValue);
	slider_feedback->SetMaxValue(_MaxFeedbackValue);
	slider_feedback->SetValue(_FeedbackValue);

	_CheckFeedbackValue(_FeedbackValue);
	playerPawn->ChorusSettings(_DepthValue, _FrequencyValue, _FeedbackValue, _WetLevelValue, _DryLevelValue, _SpreadValue);
}

void UUI_Audio_Chorus::SetWetLevelSliderValue(float currentValue, float minValue, float maxValue)
{
	_WetLevelValue = currentValue;
	_MinWetLevelValue = minValue;
	_MaxWetLevelValue = maxValue;

	slider_wetlevel->SetMinValue(_MinWetLevelValue);
	slider_wetlevel->SetMaxValue(_MaxWetLevelValue);
	slider_wetlevel->SetValue(_WetLevelValue);

	_CheckWetLevelValue(_WetLevelValue);
	playerPawn->ChorusSettings(_DepthValue, _FrequencyValue, _FeedbackValue, _WetLevelValue, _DryLevelValue, _SpreadValue);
}

void UUI_Audio_Chorus::SetDryLevelSliderValue(float currentValue, float minValue, float maxValue)
{
	_DryLevelValue = currentValue;
	_MinDryLevelValue = minValue;
	_MaxDryLevelValue = maxValue;

	slider_drylevel->SetMinValue(_MinDryLevelValue);
	slider_drylevel->SetMaxValue(_MaxDryLevelValue);
	slider_drylevel->SetValue(_DryLevelValue);

	_CheckDryLevelValue(_DryLevelValue);
	playerPawn->ChorusSettings(_DepthValue, _FrequencyValue, _FeedbackValue, _WetLevelValue, _DryLevelValue, _SpreadValue);
}

void UUI_Audio_Chorus::SetSpreadSliderValue(float currentValue, float minValue, float maxValue)
{
	_SpreadValue = currentValue;
	_MinSpreadValue = minValue;
	_MaxSpreadValue = maxValue;

	slider_spread->SetMinValue(_MinSpreadValue);
	slider_spread->SetMaxValue(_MaxSpreadValue);
	slider_spread->SetValue(_SpreadValue);

	_CheckSpreadValue(_SpreadValue);
	playerPawn->ChorusSettings(_DepthValue, _FrequencyValue, _FeedbackValue, _WetLevelValue, _DryLevelValue, _SpreadValue);
}

void UUI_Audio_Chorus::_OnApplyChorus(UPSButton* sender)
{
	if (playerPawn == nullptr)
	{
		return;
	}
	if (_bChorusApplyState == true)
	{
		playerPawn->RemoveSourceChainEffect(EEffectPreset::EChorus);
		txt_chorus_state->SetText(FText::FromString(FString("APPLY")));
		_bChorusApplyState = false;
	}
	else
	{
		playerPawn->ChorusSettings(_DepthValue, _FrequencyValue, _FeedbackValue, _WetLevelValue, _DryLevelValue, _SpreadValue);
		playerPawn->RegisterSourceChainEffect(EEffectPreset::EChorus);
		txt_chorus_state->SetText(FText::FromString(FString("REMOVE")));
		_bChorusApplyState = true;
	}
}

void UUI_Audio_Chorus::_OnDepthSliderValueChange(float sliderValue)
{
	_DepthValue = sliderValue;
	slider_depth->SetValue(_DepthValue);
	edtxt_depth->SetText(FText::FromString(FString::Printf(TEXT("%.3f"), _DepthValue)));

	_CheckDepthValue(_DepthValue);
	playerPawn->ChorusSettings(_DepthValue, _FrequencyValue, _FeedbackValue, _WetLevelValue, _DryLevelValue, _SpreadValue);
	
}

void UUI_Audio_Chorus::_OnFrequencySliderValueChange(float sliderValue)
{
	_FrequencyValue = sliderValue;
	slider_frequency->SetValue(_FrequencyValue);
	edtxt_frequency->SetText(FText::FromString(FString::Printf(TEXT("%.3f"), _FrequencyValue)));
	
	_CheckFrequencyValue(_FrequencyValue);
	playerPawn->ChorusSettings(_DepthValue, _FrequencyValue, _FeedbackValue, _WetLevelValue, _DryLevelValue, _SpreadValue);

}

void UUI_Audio_Chorus::_OnFeedbackSliderValueChange(float sliderValue)
{
	_FeedbackValue = sliderValue;
	slider_feedback->SetValue(_FeedbackValue);
	edtxt_feedback->SetText(FText::FromString(FString::Printf(TEXT("%.3f"), _FeedbackValue)));

	_CheckFeedbackValue(_FeedbackValue);
	playerPawn->ChorusSettings(_DepthValue, _FrequencyValue, _FeedbackValue, _WetLevelValue, _DryLevelValue, _SpreadValue);
}

void UUI_Audio_Chorus::_OnDryLevelSliderValueChange(float sliderValue)
{
	_DryLevelValue = sliderValue;
	slider_drylevel->SetValue(_DryLevelValue);
	edtxt_drylevel->SetText(FText::FromString(FString::Printf(TEXT("%.3f"), _DryLevelValue)));

	_CheckDryLevelValue(_DryLevelValue);
	playerPawn->ChorusSettings(_DepthValue, _FrequencyValue, _FeedbackValue, _WetLevelValue, _DryLevelValue, _SpreadValue);
}

void UUI_Audio_Chorus::_OnWetLevelSliderValueChange(float sliderValue)
{
	_WetLevelValue = sliderValue;
	slider_wetlevel->SetValue(_WetLevelValue);
	edtxt_wetlevel->SetText(FText::FromString(FString::Printf(TEXT("%.3f"), _DryLevelValue)));

	_CheckWetLevelValue(_WetLevelValue);
	playerPawn->ChorusSettings(_DepthValue, _FrequencyValue, _FeedbackValue, _WetLevelValue, _DryLevelValue, _SpreadValue);
}

void UUI_Audio_Chorus::_OnSpreadSliderValueChange(float sliderValue)
{
	_SpreadValue = sliderValue;
	slider_spread->SetValue(_SpreadValue);
	edtxt_spread->SetText(FText::FromString(FString::Printf(TEXT("%.3f"), _SpreadValue)));

	_CheckSpreadValue(_SpreadValue);
	playerPawn->ChorusSettings(_DepthValue, _FrequencyValue, _FeedbackValue, _WetLevelValue, _DryLevelValue, _SpreadValue);
}

void UUI_Audio_Chorus::_OnDepthClickMinus(UPSButton* sender)
{
	_DepthValue -= .5f;
	slider_depth->SetValue(_DepthValue);
	edtxt_depth->SetText(FText::FromString(FString::Printf(TEXT("%.3f"), _DepthValue)));

	_CheckDepthValue(_DepthValue);
	playerPawn->ChorusSettings(_DepthValue, _FrequencyValue, _FeedbackValue, _WetLevelValue, _DryLevelValue, _SpreadValue);
}

void UUI_Audio_Chorus::_OnDepthClickPlus(UPSButton* sender)
{
	_DepthValue += .5f;
	slider_depth->SetValue(_DepthValue);
	edtxt_depth->SetText(FText::FromString(FString::Printf(TEXT("%.3f"), _DepthValue)));

	_CheckDepthValue(_DepthValue);
	playerPawn->ChorusSettings(_DepthValue, _FrequencyValue, _FeedbackValue, _WetLevelValue, _DryLevelValue, _SpreadValue);
}

void UUI_Audio_Chorus::_OnFrequencyClickMinus(UPSButton* sender)
{
	_FrequencyValue -= .5f;
	slider_frequency->SetValue(_FrequencyValue);
	edtxt_frequency->SetText(FText::FromString(FString::Printf(TEXT("%.3f"), _FrequencyValue)));

	_CheckFrequencyValue(_FrequencyValue);
	playerPawn->ChorusSettings(_DepthValue, _FrequencyValue, _FeedbackValue, _WetLevelValue, _DryLevelValue, _SpreadValue);
}

void UUI_Audio_Chorus::_OnFrequencyClickPlus(UPSButton* sender)
{
	_FrequencyValue += .5f;
	slider_frequency->SetValue(_FrequencyValue);
	edtxt_frequency->SetText(FText::FromString(FString::Printf(TEXT("%.3f"), _FrequencyValue)));

	_CheckFrequencyValue(_FrequencyValue);
	playerPawn->ChorusSettings(_DepthValue, _FrequencyValue, _FeedbackValue, _WetLevelValue, _DryLevelValue, _SpreadValue);
}

void UUI_Audio_Chorus::_OnFeedbackClickMinus(UPSButton* sender)
{
	_FeedbackValue -= .5f;
	slider_feedback->SetValue(_FeedbackValue);
	edtxt_feedback->SetText(FText::FromString(FString::Printf(TEXT("%.3f"), _FeedbackValue)));
	
	_CheckFeedbackValue(_FeedbackValue);
	playerPawn->ChorusSettings(_DepthValue, _FrequencyValue, _FeedbackValue, _WetLevelValue, _DryLevelValue, _SpreadValue);
}

void UUI_Audio_Chorus::_OnFeedbackClickPlus(UPSButton* sender)
{
	_FeedbackValue += .5f;
	slider_feedback->SetValue(_FeedbackValue);
	edtxt_feedback->SetText(FText::FromString(FString::Printf(TEXT("%.3f"), _FeedbackValue)));

	_CheckFeedbackValue(_FeedbackValue);
	playerPawn->ChorusSettings(_DepthValue, _FrequencyValue, _FeedbackValue, _WetLevelValue, _DryLevelValue, _SpreadValue);
}

void UUI_Audio_Chorus::_OnWetLevelClickMinus(UPSButton* sender)
{
	_WetLevelValue -= .5f;
	slider_wetlevel->SetValue(_WetLevelValue);
	edtxt_wetlevel->SetText(FText::FromString(FString::Printf(TEXT("%.3f"), _WetLevelValue)));

	_CheckWetLevelValue(_WetLevelValue);
	playerPawn->ChorusSettings(_DepthValue, _FrequencyValue, _FeedbackValue, _WetLevelValue, _DryLevelValue, _SpreadValue);
}

void UUI_Audio_Chorus::_OnWetLevelClickPlus(UPSButton* sender)
{
	_WetLevelValue += .5f;
	slider_wetlevel->SetValue(_WetLevelValue);
	edtxt_wetlevel->SetText(FText::FromString(FString::Printf(TEXT("%.3f"), _WetLevelValue)));

	_CheckWetLevelValue(_WetLevelValue);
	playerPawn->ChorusSettings(_DepthValue, _FrequencyValue, _FeedbackValue, _WetLevelValue, _DryLevelValue, _SpreadValue);
}

void UUI_Audio_Chorus::_OnDryLevelClickMinus(UPSButton* sender)
{
	_DryLevelValue -= .5f;
	slider_drylevel->SetValue(_DryLevelValue);
	edtxt_drylevel->SetText(FText::FromString(FString::Printf(TEXT("%.3f"), _DryLevelValue)));

	_CheckWetLevelValue(_DryLevelValue);
	playerPawn->ChorusSettings(_DepthValue, _FrequencyValue, _FeedbackValue, _WetLevelValue, _DryLevelValue, _SpreadValue);
}

void UUI_Audio_Chorus::_OnDryLevelClickPlus(UPSButton* sender)
{
	_DryLevelValue += .5f;
	slider_drylevel->SetValue(_DryLevelValue);
	edtxt_drylevel->SetText(FText::FromString(FString::Printf(TEXT("%.3f"), _DryLevelValue)));

	_CheckWetLevelValue(_DryLevelValue);
	playerPawn->ChorusSettings(_DepthValue, _FrequencyValue, _FeedbackValue, _WetLevelValue, _DryLevelValue, _SpreadValue);
}

void UUI_Audio_Chorus::_OnSpreadClickMinus(UPSButton* sender)
{
	_SpreadValue -= .5f;
	slider_spread->SetValue(_SpreadValue);
	edtxt_spread->SetText(FText::FromString(FString::Printf(TEXT("%.3f"), _SpreadValue)));

	_CheckSpreadValue(_SpreadValue);
	playerPawn->ChorusSettings(_DepthValue, _FrequencyValue, _FeedbackValue, _WetLevelValue, _DryLevelValue, _SpreadValue);
}

void UUI_Audio_Chorus::_OnSpreadClickPlus(UPSButton* sender)
{
	_SpreadValue += .5f;
	slider_spread->SetValue(_SpreadValue);
	edtxt_spread->SetText(FText::FromString(FString::Printf(TEXT("%.3f"), _SpreadValue)));

	_CheckSpreadValue(_SpreadValue);
	playerPawn->ChorusSettings(_DepthValue, _FrequencyValue, _FeedbackValue, _WetLevelValue, _DryLevelValue, _SpreadValue);
}

void UUI_Audio_Chorus::_CheckDepthValue(float value)
{
	if (_DepthValue <= _MinDepthValue)
	{
		_DepthValue = _MinDepthValue;
		edtxt_depth->SetText(FText::FromString(FString::Printf(TEXT("%.3f"), _DepthValue)));
	}
	if (_DepthValue >= _MaxDepthValue)
	{
		_DepthValue = _MaxDepthValue;
		edtxt_depth->SetText(FText::FromString(FString::Printf(TEXT("%.3f"), _DepthValue)));
	}
}

void UUI_Audio_Chorus::_CheckFrequencyValue(float value)
{
	if (_FrequencyValue <= _MinFrequencyValue)
	{
		_FrequencyValue = _MinFrequencyValue;
		edtxt_frequency->SetText(FText::FromString(FString::Printf(TEXT("%.3f"), _FrequencyValue)));
	}
	if (_FrequencyValue >= _MaxFrequencyValue)
	{
		_FrequencyValue = _MaxFrequencyValue;
		edtxt_frequency->SetText(FText::FromString(FString::Printf(TEXT("%.3f"), _FrequencyValue)));
	}
}

void UUI_Audio_Chorus::_CheckFeedbackValue(float value)
{
	if (_FeedbackValue <= _MinFeedbackValue)
	{
		_FeedbackValue = _MinFeedbackValue;
		edtxt_feedback->SetText(FText::FromString(FString::Printf(TEXT("%.3f"), _FeedbackValue)));
	}

	if (_FeedbackValue >= _MaxFeedbackValue)
	{
		_FeedbackValue = _MaxFeedbackValue;
		edtxt_feedback->SetText(FText::FromString(FString::Printf(TEXT("%.3f"), _FeedbackValue)));
	}
}

void UUI_Audio_Chorus::_CheckWetLevelValue(float value)
{
	if (_WetLevelValue <= _MinWetLevelValue)
	{
		_WetLevelValue = _MinWetLevelValue;
		edtxt_wetlevel->SetText(FText::FromString(FString::Printf(TEXT("%.3f"), _WetLevelValue)));
	}
	if (_WetLevelValue >= _MaxWetLevelValue)
	{
		_WetLevelValue = _MaxWetLevelValue;
		edtxt_wetlevel->SetText(FText::FromString(FString::Printf(TEXT("%.3f"), _WetLevelValue)));
	}
}

void UUI_Audio_Chorus::_CheckDryLevelValue(float value)
{
	if (_DryLevelValue <= _MinDryLevelValue)
	{
		_DryLevelValue = _MinDryLevelValue;
		edtxt_drylevel->SetText(FText::FromString(FString::Printf(TEXT("%.3f"), _DryLevelValue)));
	}
	if (_DryLevelValue >= _MaxDryLevelValue)
	{
		_DryLevelValue = _MaxDryLevelValue;
		edtxt_drylevel->SetText(FText::FromString(FString::Printf(TEXT("%.3f"), _DryLevelValue)));
	}
}

void UUI_Audio_Chorus::_CheckSpreadValue(float value)
{
	if (_SpreadValue <= _MinSpreadValue)
	{
		_SpreadValue = _MinSpreadValue;
		edtxt_spread->SetText(FText::FromString(FString::Printf(TEXT("%.3f"), _SpreadValue)));
	}
	if (_SpreadValue >= _MaxSpreadValue)
	{
		_SpreadValue = _MaxSpreadValue;
		edtxt_spread->SetText(FText::FromString(FString::Printf(TEXT("%.3f"), _SpreadValue)));
	}
}

