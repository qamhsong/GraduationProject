// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_Audio_Phaser.h"
#include "UIManager.h"
#include "PSButton.h"
#include "Components/Slider.h"
#include "Components/EditableText.h"
#include "Components/TextBlock.h"
#include "Components/CheckBox.h"
#include "PS_Player.h"
#include "Kismet/GameplayStatics.h"
#include "PS_GameInstance.h"
#include "PS_Singleton.h"


void UUI_Audio_Phaser::_OnCreate()
{
	Super::_OnCreate();
}

void UUI_Audio_Phaser::_OnShow()
{
	 Super::_OnShow();
}

void UUI_Audio_Phaser::_OnWidgetCalledFromParent()
{
	SAFE_BIND_DELEGATE_LEFT_CLICKED(this, btn_phaser_apply, &UUI_Audio_Phaser::_OnApplyPhaser);
	SAFE_BIND_DELEGATE_LEFT_CLICKED(this, btn_wetlevel_minus, &UUI_Audio_Phaser::_OnWetLevelClickMinus);
	SAFE_BIND_DELEGATE_LEFT_CLICKED(this, btn_wetlevel_plus, &UUI_Audio_Phaser::_OnWetLevelClickPlus);
	SAFE_BIND_DELEGATE_LEFT_CLICKED(this, btn_frequency_minus, &UUI_Audio_Phaser::_OnFrequencyClickMinus);
	SAFE_BIND_DELEGATE_LEFT_CLICKED(this, btn_frequency_plus, &UUI_Audio_Phaser::_OnFrequencyClickPlus);
	SAFE_BIND_DELEGATE_LEFT_CLICKED(this, btn_feedback_minus, &UUI_Audio_Phaser::_OnFeedbackClickMinus);
	SAFE_BIND_DELEGATE_LEFT_CLICKED(this, btn_feedback_plus, &UUI_Audio_Phaser::_OnFeedbackClickPlus);

	slider_wetlevel->OnValueChanged.AddDynamic(this, &UUI_Audio_Phaser::_OnWetLevelSliderValueChange);
	slider_frequency->OnValueChanged.AddDynamic(this, &UUI_Audio_Phaser::_OnFrequencySliderValueChange);
	slider_feedback->OnValueChanged.AddDynamic(this, &UUI_Audio_Phaser::_OnFeedbackSliderValueChange);

	edtxt_wetlevel->OnTextCommitted.AddDynamic(this, &UUI_Audio_Phaser::_OnWetLevelTxtChangeCommit);
	edtxt_frequency->OnTextCommitted.AddDynamic(this, &UUI_Audio_Phaser::_OnFrequencyTxtChangeCommit);
	edtxt_feedback->OnTextCommitted.AddDynamic(this, &UUI_Audio_Phaser::_OnFeedbackTxtChangeCommit);

	cb_sine->OnCheckStateChanged.AddDynamic(this, &UUI_Audio_Phaser::_OnCheckLFOOption_Sine);
	cb_upsaw->OnCheckStateChanged.AddDynamic(this, &UUI_Audio_Phaser::_OnCheckLFOOption_UpSaw);
	cb_downsaw->OnCheckStateChanged.AddDynamic(this, &UUI_Audio_Phaser::_OnCheckLFOOption_DownSaw);
	cb_square->OnCheckStateChanged.AddDynamic(this, &UUI_Audio_Phaser::_OnCheckLFOOption_Square);
	cb_triangle->OnCheckStateChanged.AddDynamic(this, &UUI_Audio_Phaser::_OnCheckLFOOption_Triangle);
	cb_exponential->OnCheckStateChanged.AddDynamic(this, &UUI_Audio_Phaser::_OnCheckLFOOption_Exponential);
	cb_randomsamplehold->OnCheckStateChanged.AddDynamic(this, &UUI_Audio_Phaser::_OnCheckLFOOption_RandomSampleHold);

	_CheckBoxWidgets.Add(cb_sine);
	_CheckBoxWidgets.Add(cb_upsaw);
	_CheckBoxWidgets.Add(cb_downsaw);
	_CheckBoxWidgets.Add(cb_square);
	_CheckBoxWidgets.Add(cb_triangle);
	_CheckBoxWidgets.Add(cb_exponential);
	_CheckBoxWidgets.Add(cb_randomsamplehold);


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

	_WetLevelValue = 0.2f;
	_FrequencyValue = 2.f;
	_FeedbackValue = 0.3f;
	
}

void UUI_Audio_Phaser::SetLFOInfo(int32 checkIndex)
{
}

void UUI_Audio_Phaser::SetWetLevelSliderValue(float currentValue, float minValue, float MaxValue)
{
	_WetLevelValue = currentValue;
	_MinWetLevelValue = minValue;
	_MaxWetLevelValue = MaxValue;

	slider_wetlevel->SetMinValue(_MinWetLevelValue);
	slider_wetlevel->SetMaxValue(_MaxWetLevelValue);
	slider_wetlevel->SetValue(_WetLevelValue);

	_CheckWetLevelValue(_WetLevelValue);
	playerPawn->PhaserSettings_float(_WetLevelValue, _FrequencyValue, _FeedbackValue);
}

void UUI_Audio_Phaser::SetFrequencySliderValue(float currentValue, float minValue, float MaxValue)
{
	_FrequencyValue = currentValue;
	_MinFrequencyValue = minValue;
	_MaxFrequencyValue = MaxValue;

	slider_frequency->SetMinValue(_MinFrequencyValue);
	slider_frequency->SetMaxValue(_MaxFrequencyValue);
	slider_frequency->SetValue(_FrequencyValue);

	_CheckFrequencyValue(_FrequencyValue);
	playerPawn->PhaserSettings_float(_WetLevelValue,_FrequencyValue,_FeedbackValue);
}

void UUI_Audio_Phaser::SetFeedbackSliderValue(float currentValue, float minValue, float MaxValue)
{
	_FeedbackValue = currentValue;
	_MinFeedbackValue = minValue;
	_MaxFrequencyValue = MaxValue;

	slider_feedback->SetMinValue(_MinFeedbackValue);
	slider_feedback->SetMaxValue(_MaxFeedbackValue);
	slider_feedback->SetValue(_FeedbackValue);

	_CheckFeedbackValue(_FeedbackValue);
	playerPawn->PhaserSettings_float(_WetLevelValue, _FrequencyValue, _FeedbackValue);
}

void UUI_Audio_Phaser::_OnApplyPhaser(UPSButton* sender)
{
	if (playerPawn == nullptr)
	{
		return;
	}

	if (_bPhaserApplyState == true)
	{
		playerPawn->RemoveSourceChainEffect(EEffectPreset::EPhaser);
		txt_phaser_state->SetText(FText::FromString(FString("APPLY")));
		UE_LOG(LogTemp, Warning, TEXT("Phaser Removed"));
		_bPhaserApplyState = false;
	}
	else
	{
		playerPawn->PhaserSettings_float(_WetLevelValue, _FrequencyValue, _FeedbackValue);
		playerPawn->RegisterSourceChainEffect(EEffectPreset::EPhaser);
		txt_phaser_state->SetText(FText::FromString(FString("REMOVE")));
		_bPhaserApplyState = true;
	}
}

void UUI_Audio_Phaser::_OnCheckLFOOption_Sine(bool _bIsCheck)
{
	_OnCheckCheckBox(0);
	UE_LOG(LogTemp, Warning, TEXT("LFO Sine Activated"));
	playerPawn->PhaserSettings_LFO(EEffectPhaserLFOType::Sine);
}

void UUI_Audio_Phaser::_OnCheckLFOOption_UpSaw(bool _bIsCheck)
{
	_OnCheckCheckBox(1);
	UE_LOG(LogTemp, Warning, TEXT("LFO UpSaw Activated"));
	playerPawn->PhaserSettings_LFO(EEffectPhaserLFOType::Upsaw);
}

void UUI_Audio_Phaser::_OnCheckLFOOption_DownSaw(bool _bIsCheck)
{
	_OnCheckCheckBox(2);
	UE_LOG(LogTemp, Warning, TEXT("LFO DownSaw Activated"));
	playerPawn->PhaserSettings_LFO(EEffectPhaserLFOType::DownSaw);
}

void UUI_Audio_Phaser::_OnCheckLFOOption_Square(bool _bIsCheck)
{
	_OnCheckCheckBox(3);
	UE_LOG(LogTemp, Warning, TEXT("LFO Square Activated"));
	playerPawn->PhaserSettings_LFO(EEffectPhaserLFOType::Square);
}

void UUI_Audio_Phaser::_OnCheckLFOOption_Triangle(bool _bIsCheck)
{
	_OnCheckCheckBox(4);
	UE_LOG(LogTemp, Warning, TEXT("LFO Triangle Activated"));
	playerPawn->PhaserSettings_LFO(EEffectPhaserLFOType::Triangle);
}

void UUI_Audio_Phaser::_OnCheckLFOOption_Exponential(bool _bIsCheck)
{
	_OnCheckCheckBox(5);
	UE_LOG(LogTemp, Warning, TEXT("LFO Exponential Activated"));
	playerPawn->PhaserSettings_LFO(EEffectPhaserLFOType::Exponential);
}

void UUI_Audio_Phaser::_OnCheckLFOOption_RandomSampleHold(bool _bIsCheck)
{
	_OnCheckCheckBox(6);
	UE_LOG(LogTemp, Warning, TEXT("LFO RandomSampleHold Activated"));
	playerPawn->PhaserSettings_LFO(EEffectPhaserLFOType::RandomSampleHold);
}


void UUI_Audio_Phaser::_OnWetLevelSliderValueChange(float sliderValue)
{
	 _WetLevelValue = sliderValue;
	 slider_wetlevel->SetValue(_WetLevelValue);
	 edtxt_wetlevel->SetText(FText::FromString(FString::Printf(TEXT("%.3f"), _WetLevelValue)));

	 _CheckWetLevelValue(_WetLevelValue);
	 playerPawn->PhaserSettings_float(_WetLevelValue,_FrequencyValue,_FeedbackValue);
}

void UUI_Audio_Phaser::_OnWetLevelTxtChangeCommit(const FText& txtValue, ETextCommit::Type commitType)
{
	if (commitType != ETextCommit::OnEnter)
	{
		return;
	}

	FString txt = txtValue.ToString();
	_WetLevelValue = FCString::Atof(*txt.TrimQuotes());

	slider_wetlevel->SetValue(_WetLevelValue);

	_CheckWetLevelValue(_WetLevelValue);
	playerPawn->PhaserSettings_float(_WetLevelValue, _FrequencyValue, _FeedbackValue);
	_OnWetLevelSliderValueChange(_WetLevelValue);
}

void UUI_Audio_Phaser::_OnWetLevelClickMinus(UPSButton* sender)
{
	_WetLevelValue -= .5f;
	slider_wetlevel->SetValue(_WetLevelValue);
	edtxt_wetlevel->SetText(FText::FromString(FString::Printf(TEXT("%.3f"), _WetLevelValue)));

	_CheckWetLevelValue(_WetLevelValue);
	playerPawn->PhaserSettings_float(_WetLevelValue, _FrequencyValue, _FeedbackValue);
}

void UUI_Audio_Phaser::_OnWetLevelClickPlus(UPSButton* sender)
{
	_WetLevelValue += .5f;
	slider_wetlevel->SetValue(_WetLevelValue);
	edtxt_wetlevel->SetText(FText::FromString(FString::Printf(TEXT("%.3f"), _WetLevelValue)));

	_CheckWetLevelValue(_WetLevelValue);
	playerPawn->PhaserSettings_float(_WetLevelValue, _FrequencyValue, _FeedbackValue);
}

void UUI_Audio_Phaser::_OnFrequencySliderValueChange(float sliderValue)
{
	_FrequencyValue = sliderValue;
	slider_frequency->SetValue(_FrequencyValue);
	edtxt_frequency->SetText(FText::FromString(FString::Printf(TEXT("%.3f"), _FrequencyValue)));

	_CheckFrequencyValue(_FrequencyValue);
	playerPawn->PhaserSettings_float(_WetLevelValue, _FrequencyValue, _FeedbackValue);
}

void UUI_Audio_Phaser::_OnFrequencyTxtChangeCommit(const FText& txtValue, ETextCommit::Type commitType)
{
	if (commitType != ETextCommit::OnEnter)
	{
		return;
	}

	FString txt = txtValue.ToString();
	_FrequencyValue = FCString::Atof(*txt.TrimQuotes());

	slider_frequency->SetValue(_FrequencyValue);

	_CheckFrequencyValue(_FrequencyValue);
	playerPawn->PhaserSettings_float(_WetLevelValue, _FrequencyValue, _FeedbackValue);
}

void UUI_Audio_Phaser::_OnFrequencyClickMinus(UPSButton* sender)
{
	_FrequencyValue -= .5f;
	slider_frequency->SetValue(_FrequencyValue);
	edtxt_frequency->SetText(FText::FromString(FString::Printf(TEXT("%.3f"), _FrequencyValue)));

	_CheckFrequencyValue(_FrequencyValue);
	playerPawn->PhaserSettings_float(_FrequencyValue, _FrequencyValue, _FeedbackValue);
}

void UUI_Audio_Phaser::_OnFrequencyClickPlus(UPSButton* sender)
{
	_FrequencyValue += .5f;
	slider_frequency->SetValue(_FrequencyValue);
	edtxt_frequency->SetText(FText::FromString(FString::Printf(TEXT("%.3f"), _FrequencyValue)));

	_CheckFrequencyValue(_FrequencyValue);
	playerPawn->PhaserSettings_float(_FrequencyValue, _FrequencyValue, _FeedbackValue);
}

void UUI_Audio_Phaser::_OnFeedbackSliderValueChange(float sliderValue)
{
	_FeedbackValue = sliderValue;
	slider_feedback->SetValue(_FeedbackValue);
	edtxt_feedback->SetText(FText::FromString(FString::Printf(TEXT("%.3f"), _FeedbackValue)));

	_CheckFeedbackValue(_FeedbackValue);
	playerPawn->PhaserSettings_float(_WetLevelValue, _FrequencyValue, _FeedbackValue);
}

void UUI_Audio_Phaser::_OnFeedbackTxtChangeCommit(const FText& txtValue, ETextCommit::Type commitType)
{
	if (commitType != ETextCommit::OnEnter)
	{
		return;
	}

	FString txt = txtValue.ToString();
	_FeedbackValue = FCString::Atof(*txt.TrimQuotes());

	slider_feedback->SetValue(_FeedbackValue);

	_CheckFeedbackValue(_FeedbackValue);
	playerPawn->PhaserSettings_float(_WetLevelValue, _FrequencyValue, _FeedbackValue);
}

void UUI_Audio_Phaser::_OnFeedbackClickMinus(UPSButton* sender)
{
	_FeedbackValue -= .5f;
	slider_feedback->SetValue(_FeedbackValue);
	edtxt_feedback->SetText(FText::FromString(FString::Printf(TEXT("%.3f"), _FeedbackValue)));

	_CheckFeedbackValue(_FeedbackValue);
	playerPawn->PhaserSettings_float(_FrequencyValue, _FrequencyValue, _FeedbackValue);
}

void UUI_Audio_Phaser::_OnFeedbackClickPlus(UPSButton* sender)
{
	_FeedbackValue += .5f;
	slider_feedback->SetValue(_FeedbackValue);
	edtxt_feedback->SetText(FText::FromString(FString::Printf(TEXT("%.3f"), _FeedbackValue)));

	_CheckFeedbackValue(_FeedbackValue);
	playerPawn->PhaserSettings_float(_FrequencyValue, _FrequencyValue, _FeedbackValue);
}

void UUI_Audio_Phaser::_CheckWetLevelValue(float value)
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

void UUI_Audio_Phaser::_CheckFrequencyValue(float value)
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

void UUI_Audio_Phaser::_CheckFeedbackValue(float value)
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

void UUI_Audio_Phaser::_OnCheckCheckBox(int32 selectIndex)
{
	for (int32 index = 0; index < _CheckBoxWidgets.Num(); index++)
	{
		if (selectIndex == index)
		{
			_CheckBoxWidgets[index]->SetCheckedState(ECheckBoxState::Checked);
		}
		else
		{
			_CheckBoxWidgets[index]->SetCheckedState(ECheckBoxState::Unchecked);
		}
	}
}
