// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI_Base.h"
#include "UI_Audio_HighPassFilter.generated.h"

/**
 * 
 */
 class UTextBlock;
 class UPSButton;
 class UEditableText;
 class USlider;
 class APS_Player;




UCLASS(Blueprintable, BlueprintType)
class PROJECTSOUND_API UUI_Audio_HighPassFilter : public UUI_Base
{
	GENERATED_BODY()
	

	static constexpr auto RESOURCE_PATH = TEXT("UI/UI_Audio_HighPassFilter");

protected:

	virtual void _OnCreate();
	virtual void _OnShow();
	virtual void _OnWidgetCalledFromParent();

public:

	UPROPERTY()
	APS_Player* playerPawn;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* txt_highpassfilter;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* txt_highpassfilter_state;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* txt_cutfreq;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* txt_qfilter;

	UPROPERTY(meta = (BindWidget))
	UPSButton* btn_highpassfilter_apply;

	UPROPERTY(meta = (BindWidget))
	UPSButton* btn_cutfrequency_minus;

	UPROPERTY(meta = (BindWidget))
	UPSButton* btn_cutfrequency_plus;

	UPROPERTY(meta = (BindWidget))
	USlider* slider_cutfrequency;

	UPROPERTY(meta = (BindWidget))
	UEditableText* edtxt_cutfrequency;

	UPROPERTY(meta = (BindWidget))
	UPSButton* btn_qfilter_minus;

	UPROPERTY(meta = (BindWidget))
	UPSButton* btn_qfilter_plus;

	UPROPERTY(meta = (BindWidget))
	USlider* slider_qfilter;

	UPROPERTY(meta = (BindWidget))
	UEditableText* edtxt_qfilter;

	UPROPERTY()
	float _CutFreqValue;

	UPROPERTY()
	float _QFilterValue;

	UPROPERTY()
	float _MinCutFreqValue = 20.f;

	UPROPERTY()
	float _MaxCutFreqValue = 20000.f;

	UPROPERTY()
	float _MinQFilterValue = 0.5f;

	UPROPERTY()
	float _MaxQFilterValue = 10.f;

	UPROPERTY()
	bool _bHighPassFilterApplyState = false;


	void SetCutFrequencySliderValue(FString optionKey, float currentValue, float minValue = 0.f, float maxValue = 5.0f);

	void SetQFilterSliderValue(FString optionKey, float currentValue, float minValue = 0.f, float maxValue = 5.0f);


	UFUNCTION()
	void _OnApplyHighPassFilter(UPSButton* sender);

	//UFUNCTION()
	//void _OnRemoveLowPassFilter(UPSButton* sender);

	UFUNCTION()
	void _OnCutFrequencySliderValueChange(float sliderValue);

	UFUNCTION()
	void _OnCutFrequencyTxtChangeCommit(const FText& txtValue, ETextCommit::Type commitType);

	UFUNCTION()
	void _OnCutFrequencyClickMinus(UPSButton* sender);

	UFUNCTION()
	void _OnCutFrequencyClickPlus(UPSButton* sender);

	UFUNCTION()
	void _OnQFilterSliderValueChange(float sliderValue);

	UFUNCTION()
	void _OnQFilterTxtChangeCommit(const FText& txtValue, ETextCommit::Type commitType);

	UFUNCTION()
	void _OnQFilterClickMinus(UPSButton* sender);

	UFUNCTION()
	void _OnQFilterClickPlus(UPSButton* sender);

	void _CheckCutFrequencyValue(float value);

	void _CheckQFilterValue(float value);
};
