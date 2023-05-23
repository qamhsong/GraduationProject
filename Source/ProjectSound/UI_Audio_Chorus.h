// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI_Base.h"
#include "UI_Audio_Chorus.generated.h"

/**
 * 
 */

class UTextBlock;
class UPSButton;
class UEditableText;
class USlider;
class APS_Player;


UCLASS(BlueprintType, Blueprintable)
class PROJECTSOUND_API UUI_Audio_Chorus : public UUI_Base
{
	GENERATED_BODY()
	
	static constexpr auto RESOURCE_PATH = TEXT("UI/UI_Audio_Chorus");

protected:
	
	virtual void _OnCreate();
	virtual void _OnShow();
	virtual void _OnWidgetCalledFromParent();

public:

	UPROPERTY()
	APS_Player* playerPawn;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* txt_chorus;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* txt_depth;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* txt_frequency;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* txt_feedback;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* txt_wetlevel;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* txt_drylevel;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* txt_spread;

	UPROPERTY(meta = (BindWidget))
	UPSButton* btn_chorus_apply;

	UPROPERTY(meta = (BindWidget))
	UPSButton* btn_depth_minus;

	UPROPERTY(meta = (BindWidget))
	UPSButton* btn_depth_plus;

	UPROPERTY(meta = (BindWidget))
	UPSButton* btn_frequency_minus;

	UPROPERTY(meta = (BindWidget))
	UPSButton* btn_frequency_plus;

	UPROPERTY(meta = (BindWidget))
	UPSButton* btn_feedback_minus;

	UPROPERTY(meta = (BindWidget))
	UPSButton* btn_feedback_plus;

	UPROPERTY(meta = (BindWidget))
	UPSButton* btn_wetlevel_minus;

	UPROPERTY(meta = (BindWidget))
	UPSButton* btn_wetlevel_plus;

	UPROPERTY(meta = (BindWidget))
	UPSButton* btn_drylevel_minus;

	UPROPERTY(meta = (BindWidget))
	UPSButton* btn_drylevel_plus;

	UPROPERTY(meta = (BindWidget))
	UPSButton* btn_spread_minus;

	UPROPERTY(meta = (BindWidget))
	UPSButton* btn_spread_plus;

	UPROPERTY(meta = (BindWidget))
	USlider* slider_depth;

	UPROPERTY(meta = (BindWidget))
	USlider* slider_frequency;

	UPROPERTY(meta = (BindWidget))
	USlider* slider_feedback;

	UPROPERTY(meta = (BindWidget))
	USlider* slider_wetlevel;

	UPROPERTY(meta = (BindWidget))
	USlider* slider_drylevel;

	UPROPERTY(meta = (BindWidget))
	USlider* slider_spread;

	UPROPERTY()
	float _DepthValue;

	UPROPERTY()
	float _FrequencyValue;

	UPROPERTY()
	float _FeedbackValue;

	UPROPERTY()
	float _WetLevelValue;

	UPROPERTY()
	float _DryLevelValue;

	UPROPERTY()
	float _SpreadValue;

	UPROPERTY()
	float _MinDepthValue;

	UPROPERTY()
	float _MaxDepthValue;

	UPROPERTY()
	float _MinFrequencyValue;

	UPROPERTY()
	float _MaxFrequencyValue;

	UPROPERTY()
	float _MinFeedbackValue;

	UPROPERTY()
	float _MaxFeedbackValue;

	UPROPERTY()
	float _MinWetLevelValue;

	UPROPERTY()
	float _MaxWetLevelValue;

	UPROPERTY()
	float _MinDryLevelValue;

	UPROPERTY()
	float _MaxDryLevelValue;

	UPROPERTY()
	float _MinSpreadValue;

	UPROPERTY()
	float _MaxSpreadValue;

	void SetDepthSliderValue(float currentValue, float minValue, float maxValue);

	void SetFrequencySliderValue(float currentValue, float minValue, float maxValue);

	void SetFeedbackSliderValue(float currentValue, float minValue, float maxValue);

	void SetWetLevelSliderValue(float currentValue, float minValue, float maxValue);

	void SetDryLevelSliderValue(float currentValue, float minValue, float maxValue);

	void SetSpreadSliderValue(float currentValue, float minValue, float maxValue);

	UFUNCTION()
	void _OnApplyChorus(UPSButton* sender);

	UFUNCTION()
	void _OnDepthSliderValueChange(float sliderValue);

	UFUNCTION()
	void _OnFrequencySliderValueChange(float sliderValue);

	UFUNCTION()
	void _OnFeedbackSliderValueChange(float sliderValue);

	//UFUNCTION()
	//void _OnWetLevelSliderValueChange(float sliderValue);

	//UFUNCTION()
	//void _OnDryLevelSliderValueChange(float sliderValue);

	//UFUNCTION()
	//void _OnSpreadSliderValueChange(float sliderValue);

	//UFUNCTION()
	//void _OnDepthClickMinus(UPSButton* sender);

	//UFUNCTION()
	//void _OnDepthClickPlus(UPSButton* sender);

	//UFUNCTION()
	//void _OnFrequencyClickMinus(UPSButton* sender);

	//UFUNCTION()
	//void _OnFrequencyClickPlus(UPSButton* sender);

	//UFUNCTION()
	//void _OnFeedbackClickMinus(UPSButton* sender);

	//UFUNCTION()
	//void _OnFeedbackClickPlus(UPSButton* sender);

	//UFUNCTION()
	//void _OnWetLevelClickMinus(UPSButton* sender);

	//UFUNCTION()
	//void _OnWetLevelClickPlus(UPSButton* sender);

	//UFUNCTION()
	//void _OnDryLevelClickMinus(UPSButton* sender);

	//UFUNCTION()
	//void _OnDryLevelClickPus(UPSButton* sender);

	//UFUNCTION()
	//void _OnSpreadClickMinus(UPSButton* sender);

	//UFUNCTION()
	//void _OnSpreadClickPlus(UPSButton* sender);

	//UFUNCTION()
	//void _CheckDepthValue(float value);

	//UFUNCTION()
	//void _CheckFrequencyValue(float value);

	//UFUNCTION()
	//void _CheckFeedbackValue(float value);

	//UFUNCTION()
	//void _CheckWetLevelValue(float value);

	//UFUNCTION()
	//void _CheckDryLevelValue(float value);

	//UFUNCTION()
	//void _CheckSpreadValue(float value);





};
