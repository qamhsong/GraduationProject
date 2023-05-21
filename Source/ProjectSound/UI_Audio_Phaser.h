// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI_Base.h"
#include "UI_Audio_Phaser.generated.h"

/**
 * 
 */

class UTextBlock;
class UPSButton;
class UEditableText;
class USlider;
class APS_Player;
class UCheckBox;


UCLASS(Blueprintable, BlueprintType)
class PROJECTSOUND_API UUI_Audio_Phaser : public UUI_Base
{
	GENERATED_BODY()
	
static constexpr auto REOURCE_PATH = TEXT("UI/UI_Audio_Phaser");

protected:
	
	virtual void _OnCreate();
	virtual void _OnShow();
	virtual void _OnWidgetCalledFromParent();

public:
	
	UPROPERTY()
	APS_Player* playerPawn;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* txt_phaser;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* txt_phaser_state;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* txt_phaser_wetlevel;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* txt_phaser_frequency;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* txt_phaser_feedback;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* txt_lfotype;

	UPROPERTY(meta = (BindWidget))
	UPSButton* btn_phaser_apply;

	UPROPERTY(meta = (BindWidget))
	UPSButton* btn_wetlevel_minus;

	UPROPERTY(meta = (BindWidget))
	UPSButton* btn_wetlevel_plus;

	UPROPERTY(meta = (BindWidget))
	USlider* slider_wetlevel;

	UPROPERTY(meta = (BindWidget))
	UEditableText* edtxt_wetlevel;

	UPROPERTY(meta = (BindWidget))
	UPSButton* btn_frequency_minus;

	UPROPERTY(meta = (BindWidget))
	UPSButton* btn_frequency_plus;

	UPROPERTY(meta = (BindWidget))
	USlider* slider_frequency;

	UPROPERTY(meta = (BindWidget))
	UEditableText* edtxt_frequency;

	UPROPERTY(meta = (BindWidget))
	UPSButton* btn_feedback_minus;

	UPROPERTY(meta = (BindWidget))
	UPSButton* btn_feedback_plus;

	UPROPERTY(meta = (BindWidget))
	USlider* slider_feedback;

	UPROPERTY(meta = (BindWidget))
	UEditableText* edtxt_feedback;

	UPROPERTY(meta = (BindWidget))
	UCheckBox* cb_sine;

	UPROPERTY(meta = (BindWidget))
	UCheckBox* cb_upsaw;

	UPROPERTY(meta = (BindWidget))
	UCheckBox* cb_downsaw;

	UPROPERTY(meta = (BindWidget))
	UCheckBox* cb_square;

	UPROPERTY(meta = (BindWidget))
	UCheckBox* cb_triangle;

	UPROPERTY(meta = (BindWidget))
	UCheckBox* cb_exponential;

	UPROPERTY(meta = (BindWidget))
	UCheckBox* cb_randomsamplehold;

	UPROPERTY()
	TArray<UCheckBox*> _CheckBoxWidgets;

	UPROPERTY()
	TArray<UTextBlock*> _CheckBoxTextWidgets;



	UPROPERTY()
	float _WetLevelValue;

	UPROPERTY()
	float _FrequencyValue;

	UPROPERTY()
	float _FeedbackValue;

	UPROPERTY()
	float _MinWetLevelValue = 0.f;

	UPROPERTY()
	float _MaxWetLevelValue = 1.f;

	UPROPERTY()
	float _MinFrequencyValue = 0.f;

	UPROPERTY()
	float _MaxFrequencyValue = 5.f;

	UPROPERTY()
	float _MinFeedbackValue = 0.f;

	UPROPERTY()
	float _MaxFeedbackValue = 1.f;

	UPROPERTY()
	bool _bPhaserApplyState = false;


	void SetLFOInfo(int32 checkIndex = -1);


	void SetWetLevelSliderValue(float currentValue, float minValue = 0.f, float MaxValue = 5.f);

	void SetFrequencySliderValue(float currentValue, float minValue = 0.f, float MaxValue = 5.f);

	void SetFeedbackSliderValue(float currentValue, float minValue = 0.f, float MaxValue = 5.f);

	UFUNCTION()
	void _OnApplyPhaser(UPSButton* sender);

#pragma region LFO CheckBox Delegate Bind Function
	
	UFUNCTION()
	void _OnCheckLFOOption_Sine(bool _bIsCheck);

	UFUNCTION()
	void _OnCheckLFOOption_UpSaw(bool _bIsCheck);

	UFUNCTION()
	void _OnCheckLFOOption_DownSaw(bool _bIsCheck);

	UFUNCTION()
	void _OnCheckLFOOption_Square(bool _bIsCheck);

	UFUNCTION()
	void _OnCheckLFOOption_Triangle(bool _bIsCheck);

	UFUNCTION()
	void _OnCheckLFOOption_Exponential(bool _bIsCheck);

	UFUNCTION()
	void _OnCheckLFOOption_RandomSampleHold(bool _bIsCheck);

#pragma endregion

#pragma region WetLevel Button Delegate Bind Function
	UFUNCTION()
	void _OnWetLevelSliderValueChange(float sliderValue);

	UFUNCTION()
	void _OnWetLevelTxtChangeCommit(const FText& txtValue, ETextCommit::Type commitType);

	UFUNCTION()
	void _OnWetLevelClickMinus(UPSButton* sender);

	UFUNCTION()
	void _OnWetLevelClickPlus(UPSButton* sender);

#pragma endregion


#pragma region Frequency Button Delegate Bind Function
	UFUNCTION()
	void _OnFrequencySliderValueChange(float sliderValue);

	UFUNCTION()
	void _OnFrequencyTxtChangeCommit(const FText& txtValue, ETextCommit::Type commitType);

	UFUNCTION()
	void _OnFrequencyClickMinus(UPSButton* sender);

	UFUNCTION()
	void _OnFrequencyClickPlus(UPSButton* sender);

#pragma endregion

#pragma region Feedback Button Delegate Bind Function
	UFUNCTION()
	void _OnFeedbackSliderValueChange(float sliderValue);

	UFUNCTION()
	void _OnFeedbackTxtChangeCommit(const FText& txtValue, ETextCommit::Type commitType);

	UFUNCTION()
	void _OnFeedbackClickMinus(UPSButton* sender);

	UFUNCTION()
	void _OnFeedbackClickPlus(UPSButton* sender);

#pragma endregion

	void _CheckWetLevelValue(float value);

	void _CheckFrequencyValue(float value);

	void _CheckFeedbackValue(float value);

	void _OnCheckCheckBox(int32 selectIndex);

};
