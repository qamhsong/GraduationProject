// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI_Base.h"
#include "UI_Audio_Volume.generated.h"

/**
 * 
 */

class UTextBlock;
class USlider;
class UEditableText;
class UPSButton;
class APS_Player;


UCLASS(BlueprintType, Blueprintable)
class PROJECTSOUND_API UUI_Audio_Volume : public UUI_Base
{
	GENERATED_BODY()
	
public:
	static constexpr auto RESOURCE_PATH = TEXT("UI/UI_Audio_Volume");

	DECLARE_MULTICAST_DELEGATE_TwoParams(FPSUIOptionSlider, FString, float)
	FPSUIOptionSlider OnOptionSlider;

protected:
	virtual void _OnCreate();
	virtual void _OnShow();
	virtual void _OnWidgetCalledFromParent();

public:

	UPROPERTY()
	APS_Player* playerPawn;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* txt_option_vol;

	UPROPERTY(meta = (BindWidget))
	USlider* slider_vol;

	UPROPERTY(meta = (BindWidget))
	UEditableText* edtxt_vol;

	UPROPERTY(meta = (BindWidget))
	UPSButton* btn_vol_minus;

	UPROPERTY(meta = (BindWidget))
	UPSButton* btn_vol_plus;

	UPROPERTY()
	FString _optionKey;

	UPROPERTY()
	float _Value;

	UPROPERTY()
	float _MinValue = 0.f;

	UPROPERTY()
	float _MaxValue = 100.f;

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
