// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI_Base.h"
#include "UI_Audio_Record.generated.h"

class UTextBlock;
class UPSButton;
class APS_Player;

UCLASS(BlueprintType, Blueprintable)
class PROJECTSOUND_API UUI_Audio_Record : public UUI_Base
{
	GENERATED_BODY()
	
static constexpr auto RESOURCE_PATH = TEXT("UI/UI_Audio_Record");

protected:

	virtual void _OnCreate();
	virtual void _OnShow();
	virtual void _OnWidgetCalledFromParent();

public:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* txt_option_audiocapture;

	UPROPERTY(meta = (BindWidget))
	UPSButton* btn_audiocapture_On;

	UPROPERTY(meta = (BindWidget))
	UPSButton* btn_audiocapture_Off;

	UPROPERTY(meta = (BindWidget))
	UPSButton* btn_playcapturedaudio;

	UPROPERTY()
	APS_Player* playerPawn;

public:
	
	UFUNCTION()
	void _OnClickAudioCaptureOn(UPSButton* sender);

	UFUNCTION()
	void _OnClickAudioCaptureOff(UPSButton* sender);

	UFUNCTION()
	void _OnClickPlayCapturedAudio(UPSButton* sender);

	bool _CheckAudioCaptureState();
};
