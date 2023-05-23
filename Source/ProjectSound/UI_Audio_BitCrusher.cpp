// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_Audio_BitCrusher.h"
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

void UUI_Audio_BitCrusher::_OnCreate()
{
	Super::_OnCreate();
}

void UUI_Audio_BitCrusher::_OnShow()
{
	Super::_OnShow();
}

void UUI_Audio_BitCrusher::_OnWidgetCalledFromParent()
{
	SAFE_BIND_DELEGATE_LEFT_CLICKED(this, btn_bitcrusher_apply, &UUI_Audio_BitCrusher::_OnApplyBitCrusher);
	SAFE_BIND_DELEGATE_LEFT_CLICKED(this, btn_bitdepth_minus, &UUI_Audio_BitCrusher::_OnBitDepthClickMinus);
	SAFE_BIND_DELEGATE_LEFT_CLICKED(this, btn_bitdepth_plus, &UUI_Audio_BitCrusher::_OnBitDepthClickPlus);
	SAFE_BIND_DELEGATE_LEFT_CLICKED(this, btn_samplerate_minus, &UUI_Audio_BitCrusher::_OnSampleRateClickMinus);
	SAFE_BIND_DELEGATE_LEFT_CLICKED(this, btn_samplerate_plus, &UUI_Audio_BitCrusher::_OnSampleRateClickPlus);

	slider_bitdepth->OnValueChanged.AddDynamic(this, &UUI_Audio_BitCrusher::_OnBitDepthSliderValueChange);
	slider_samplerate->OnValueChanged.AddDynamic(this, &UUI_Audio_BitCrusher::_OnSampleRateSliderValueChange);
	
	edtxt_bitdepth->OnTextCommitted.AddDynamic(this, &UUI_Audio_BitCrusher::_OnBitDepthTxtChangeCommit);
	edtxt_samplerate->OnTextCommitted.AddDynamic(this, &UUI_Audio_BitCrusher::_OnSampleRateTxtChangeCommit);


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

	_BitDepthValue = 1.f;
	_SampleRateValue = 500.f;
}

void UUI_Audio_BitCrusher::SetBitDepthSliderValue(float currentValue, float minValue, float maxValue)
{
	_BitDepthValue = currentValue;
	_MinBitDepthValue = minValue;
	_MaxBitDepthValue = maxValue;

	slider_bitdepth->SetMinValue(_MinBitDepthValue);
	slider_bitdepth->SetMaxValue(_MaxBitDepthValue);
	slider_bitdepth->SetValue(_BitDepthValue);

	_CheckBitDepthValue(_BitDepthValue);
	playerPawn->BitcrusherSettings(_SampleRateValue, _BitDepthValue);

}

void UUI_Audio_BitCrusher::SetSampleRateSliderValue(float currentValue, float minValue, float maxValue)
{
	_SampleRateValue = currentValue;
	_MinSampleRateValue = minValue;
	_MaxBitDepthValue = maxValue;

	slider_samplerate->SetMinValue(_MinSampleRateValue);
	slider_samplerate->SetMaxValue(_MaxSampleRateValue);
	slider_samplerate->SetValue(_SampleRateValue);

	_CheckSampleRateValue(_SampleRateValue);
	playerPawn->BitcrusherSettings(_SampleRateValue, _BitDepthValue);
}

void UUI_Audio_BitCrusher::_OnApplyBitCrusher(UPSButton* sender)
{
	if (playerPawn == nullptr)
	{
		return;
	}

	if (_bBitCrushApplyState == true)
	{
		playerPawn->RemoveSourceChainEffect(EEffectPreset::EBitCrusher);
		txt_bitcrusher_state->SetText(FText::FromString(FString("APPLY")));
		UE_LOG(LogTemp, Warning, TEXT("BitCrusher Removed"));
		_bBitCrushApplyState = false;
	}
	else
	{
		playerPawn->BitcrusherSettings(_SampleRateValue, _BitDepthValue);
		playerPawn->RegisterSourceChainEffect(EEffectPreset::EBitCrusher);
		txt_bitcrusher_state->SetText(FText::FromString(FString("REMOVE")));
		_bBitCrushApplyState = true;
	}
}

void UUI_Audio_BitCrusher::_OnBitDepthSliderValueChange(float sliderValue)
{
	_BitDepthValue = sliderValue;
	slider_bitdepth->SetValue(_BitDepthValue);
	edtxt_bitdepth->SetText(FText::FromString(FString::Printf(TEXT("%.3f"), _BitDepthValue)));

	_CheckBitDepthValue(_BitDepthValue);
	playerPawn->BitcrusherSettings(_SampleRateValue, _BitDepthValue);
}

void UUI_Audio_BitCrusher::_OnSampleRateSliderValueChange(float sliderValue)
{
	_SampleRateValue = sliderValue;
	slider_samplerate->SetValue(_SampleRateValue);
	edtxt_samplerate->SetText(FText::FromString(FString::Printf(TEXT("%.3f"), _SampleRateValue)));

	_CheckSampleRateValue(_SampleRateValue);
	playerPawn->BitcrusherSettings(_SampleRateValue, _BitDepthValue);

}

void UUI_Audio_BitCrusher::_OnBitDepthTxtChangeCommit(const FText& txtValue, ETextCommit::Type commitType)
{
	if (commitType != ETextCommit::OnEnter)
	{
		return;
	}

	FString txt = txtValue.ToString();
	_BitDepthValue = FCString::Atof(*txt.TrimQuotes());

	slider_bitdepth->SetValue(_BitDepthValue);

	_CheckBitDepthValue(_BitDepthValue);
	playerPawn->BitcrusherSettings(_SampleRateValue, _BitDepthValue);
}

void UUI_Audio_BitCrusher::_OnSampleRateTxtChangeCommit(const FText& txtValue, ETextCommit::Type commitType)
{
	if (commitType != ETextCommit::OnEnter)
	{
		return;
	}

	FString txt = txtValue.ToString();
	_SampleRateValue = FCString::Atof(*txt.TrimQuotes());

	slider_samplerate->SetValue(_SampleRateValue);

	_CheckSampleRateValue(_SampleRateValue);
	playerPawn->BitcrusherSettings(_SampleRateValue, _BitDepthValue);
}

void UUI_Audio_BitCrusher::_OnBitDepthClickMinus(UPSButton* sender)
{
	_BitDepthValue -= .5f;
	slider_bitdepth->SetValue(_BitDepthValue);
	edtxt_bitdepth->SetText(FText::FromString(FString::Printf(TEXT("%.3f"), _BitDepthValue)));

	_CheckBitDepthValue(_BitDepthValue);
	playerPawn->BitcrusherSettings(_SampleRateValue, _BitDepthValue);
}

void UUI_Audio_BitCrusher::_OnBitDepthClickPlus(UPSButton* sender)
{
	_BitDepthValue += .5f;
	slider_bitdepth->SetValue(_BitDepthValue);
	edtxt_bitdepth->SetText(FText::FromString(FString::Printf(TEXT("%.3f"), _BitDepthValue)));

	_CheckBitDepthValue(_BitDepthValue);
	playerPawn->BitcrusherSettings(_SampleRateValue, _BitDepthValue);
}

void UUI_Audio_BitCrusher::_OnSampleRateClickMinus(UPSButton* sender)
{
	_SampleRateValue -= .5f;
	slider_samplerate->SetValue(_SampleRateValue);
	edtxt_samplerate->SetText(FText::FromString(FString::Printf(TEXT("%.3f"), _SampleRateValue)));

	_CheckSampleRateValue(_SampleRateValue);
	playerPawn->BitcrusherSettings(_SampleRateValue, _BitDepthValue);
}

void UUI_Audio_BitCrusher::_OnSampleRateClickPlus(UPSButton* sender)
{
	_SampleRateValue += .5f;
	slider_samplerate->SetValue(_SampleRateValue);
	edtxt_samplerate->SetText(FText::FromString(FString::Printf(TEXT("%.3f"), _SampleRateValue)));

	_CheckSampleRateValue(_SampleRateValue);
	playerPawn->BitcrusherSettings(_SampleRateValue, _BitDepthValue);
}

void UUI_Audio_BitCrusher::_CheckBitDepthValue(float value)
{
	if (_BitDepthValue <= _MinBitDepthValue)
	{
		_BitDepthValue = _MinBitDepthValue;
		edtxt_bitdepth->SetText(FText::FromString(FString::Printf(TEXT("%.3f"), _BitDepthValue)));
	}

	if (_BitDepthValue >= _MaxBitDepthValue)
	{
		_BitDepthValue = _MaxBitDepthValue;
		edtxt_bitdepth->SetText(FText::FromString(FString::Printf(TEXT("%.3f"), _BitDepthValue)));
	}
}

void UUI_Audio_BitCrusher::_CheckSampleRateValue(float value)
{
	if (_SampleRateValue <= _MinSampleRateValue)
	{
		_SampleRateValue = _MinSampleRateValue;
		edtxt_samplerate->SetText(FText::FromString(FString::Printf(TEXT("%.3f"), _MinSampleRateValue)));
	}

	if (_SampleRateValue >= _MaxSampleRateValue)
	{
		_SampleRateValue = _MaxSampleRateValue;
		edtxt_samplerate->SetText(FText::FromString(FString::Printf(TEXT("%.3f"), _SampleRateValue)));
	}
}
