// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI_Base.h"
#include "UI_Audio_BitCrusher.generated.h"

/**
 * 
 */
class UTextBlock;
class UPSButton;
class UEditableText;
class USlider;
class APS_Player;


UCLASS(Blueprintable, BlueprintType)
class PROJECTSOUND_API UUI_Audio_BitCrusher : public UUI_Base
{
	GENERATED_BODY()

	static constexpr auto RESOURCE_PATH = TEXT("UI/UI_Audio_Bitcrusher");

protected:
	
	virtual void _OnCreate();
	virtual void _OnShow();
	virtual void _OnWidgetCalledFromParent();

public:

	UPROPERTY()
	APS_Player* playerPawn;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* txt_bitcrusher;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* txt_bitcrusher_state;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* txt_bitdepth;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* txt_samplerate;

	UPROPERTY(meta = (BindWidget))
	UPSButton* btn_bitcrusher_apply;

	UPROPERTY(meta = (BindWidget))
	UPSButton* btn_bitdepth_minus;

	UPROPERTY(meta = (BindWidget))
	UPSButton* btn_bitdepth_plus;

	UPROPERTY(meta = (BindWidget))
	UPSButton* btn_samplerate_minus;

	UPROPERTY(meta = (BindWidget))
	UPSButton* btn_samplerate_plus;

	UPROPERTY(meta = (BindWidget))
	USlider* slider_bitdepth;

	UPROPERTY(meta = (BindWidget))
	USlider* slider_samplerate;

	UPROPERTY(meta = (BindWidget))
	UEditableText* edtxt_bitdepth;

	UPROPERTY(meta = (BindWidget))
	UEditableText* edtxt_samplerate;

	UPROPERTY()
	float _BitDepthValue;

	UPROPERTY()
	float _SampleRateValue;

	UPROPERTY()
	float _MinBitDepthValue = 1.f;

	UPROPERTY()
	float _MaxBitDepthValue = 16.f;

	UPROPERTY()
	float _MinSampleRateValue = 500.f;

	UPROPERTY()
	float _MaxSampleRateValue = 16000.f;

	UPROPERTY()
	bool _bBitCrushApplyState = false;

	void SetBitDepthSliderValue(float currentValue, float minValue, float maxValue);

	void SetSampleRateSliderValue(float currentValue, float minValue, float maxValue);

	UFUNCTION()
	void _OnApplyBitCrusher(UPSButton* sender);

	UFUNCTION()
	void _OnBitDepthSliderValueChange(float sliderValue);

	UFUNCTION()
	void _OnSampleRateSliderValueChange(float sliderValue);

	UFUNCTION()
	void _OnBitDepthTxtChangeCommit(const FText& txtValue, ETextCommit::Type commitType);

	UFUNCTION()
	void _OnSampleRateTxtChangeCommit(const FText& txtValue, ETextCommit::Type commitType);

	UFUNCTION()
	void _OnBitDepthClickMinus(UPSButton* sender);

	UFUNCTION()
	void _OnBitDepthClickPlus(UPSButton* sender);

	UFUNCTION()
	void _OnSampleRateClickMinus(UPSButton* sender);

	UFUNCTION()
	void _OnSampleRateClickPlus(UPSButton* sender);

	void _CheckBitDepthValue(float value);

	void _CheckSampleRateValue(float value);

	
};
