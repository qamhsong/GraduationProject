// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI_Base.h"
#include "UI_Audio.generated.h"

/**
 * 
 */
 class UTextBlock;
 class USlider;
 class UEditableText;
 class UPSButton;

UCLASS(BlueprintType, Blueprintable)
class PROJECTSOUND_API UUI_Audio : public UUI_Base
{
	GENERATED_BODY()
	

public:
	static constexpr auto RESOURCE_PATH = TEXT("UI/UI_Audio");

	DECLARE_MULTICAST_DELEGATE_TwoParams(FPSUIOptionSlider, FString, float)
	FPSUIOptionSlider OnOptionSlider;

protected:
	virtual void _OnCreate();
	virtual void _OnShow();

public:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* OptionPitch;

	UPROPERTY(meta = (BindWidget))
	USlider* PitchValue;

	UPROPERTY(meta = (BindWidget))
	UEditableText* PitchTxtValue;

	UPROPERTY(meta = (BindWidget))
	UPSButton* pitchBtnMinus;

	UPROPERTY(meta = (BindWidget))
	UPSButton* pitchBtnPlus;

	UPROPERTY()
	FString _optionKey;

	UPROPERTY()
	float _Value;

	UPROPERTY()
	float _MinValue;

	UPROPERTY()
	float _MaxValue;

public:
	void SetSliderValue(FString optionKey, float currentValue, float minValue = 0.f, float maxValue = 5.0f);

	FString GetOptionKey() { return _optionKey; }

private:
	UFUNCTION()
	void _OnSliderValueChange(float sliderValue);

	UFUNCTION()
	void _OnTxtChangeCommit(const FText& txtValue, ETextCommit::Type commitType);

	UFUNCTION()
	void _OnClickMinus(UPSButton* sender);

	UFUNCTION()
	void _OnClickPlus(UPSButton* sender);

	void _CheckValue(float value);
};
