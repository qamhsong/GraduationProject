// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_Audio_Record.h"
#include "PS_Player.h"
#include "UIManager.h"
#include "PSButton.h"
#include "Kismet/GameplayStatics.h"
#include "PS_GameInstance.h"
#include "PS_Singleton.h"
#include "Components/TextBlock.h"

void UUI_Audio_Record::_OnCreate()
{
	SAFE_BIND_DELEGATE_LEFT_CLICKED(this, btn_audiocapture_On, &UUI_Audio_Record::_OnClickAudioCaptureOn);
	//SAFE_BIND_DELEGATE_LEFT_CLICKED(this, btn_audiocapture_Off, &UUI_Audio_Record::_OnClickAudioCaptureOff);
	SAFE_BIND_DELEGATE_LEFT_CLICKED(this, btn_playcapturedaudio, &UUI_Audio_Record::_OnClickPlayCapturedAudio);
	SAFE_BIND_DELEGATE_LEFT_CLICKED(this, btn_audiocapture_realtime, &UUI_Audio_Record::_OnClickAudioCaptureRealTime);

	Super::_OnCreate();
	UE_LOG(LogTemp, Warning, TEXT("_OnCreate AUDIO_RECORD"));
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


}

void UUI_Audio_Record::_OnShow()
{
	Super::_OnShow();
	this->AddToViewport();
	GetUIManager()->ApplyInputMode(EUIInputMode::GameAndUI);

}

void UUI_Audio_Record::_OnWidgetCalledFromParent()
{
	SAFE_BIND_DELEGATE_LEFT_CLICKED(this, btn_audiocapture_On, &UUI_Audio_Record::_OnClickAudioCaptureOn);
	//SAFE_BIND_DELEGATE_LEFT_CLICKED(this, btn_audiocapture_Off, &UUI_Audio_Record::_OnClickAudioCaptureOff);
	SAFE_BIND_DELEGATE_LEFT_CLICKED(this, btn_playcapturedaudio, &UUI_Audio_Record::_OnClickPlayCapturedAudio);
	SAFE_BIND_DELEGATE_LEFT_CLICKED(this, btn_audiocapture_realtime, &UUI_Audio_Record::_OnClickAudioCaptureRealTime);


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
	btn_audiocapture_realtime->SetColorAndOpacity(FLinearColor::Red);
}

void UUI_Audio_Record::_OnClickAudioCaptureRealTime(UPSButton* sender)
{
	if (playerPawn == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("No playerpawn"));
		return;
	}
	if (playerPawn->GetAudioCaptureState() == false)
	{
		playerPawn->ActivateRealTimeVoiceCapture();
		btn_audiocapture_realtime->SetColorAndOpacity(FLinearColor::Green);
	}
	else
	{
		playerPawn->DeActivateRealTimeVoiceCapture();
		btn_audiocapture_realtime->SetColorAndOpacity(FLinearColor::Red);
	}
	
}

void UUI_Audio_Record::_OnClickAudioCaptureOn(UPSButton* sender)
{
	if (playerPawn == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("No playerpawn"));
		return;
	}

	if (playerPawn->GetAudioCaptureState() == false)
	{
		playerPawn->ActivateVoiceCapture();
		txt_audiocapture_state->SetText(FText::FromString(FString("ON")));
	}
	else
	{
		playerPawn->DeActivateVoiceCapture();
		txt_audiocapture_state->SetText(FText::FromString(FString("OFF")));
		if (playerPawn->CheckSoundAppliedToAudioComponent())
		{
			btn_playcapturedaudio->SetColorAndOpacity(FLinearColor::Green);
		}
	}
}

void UUI_Audio_Record::_OnClickAudioCaptureOff(UPSButton* sender)
{
	if (playerPawn == nullptr)
	{
		return;
	}

	if (playerPawn->GetAudioCaptureState() == true)
	{
		playerPawn->DeActivateVoiceCapture();
		if (playerPawn->CheckSoundAppliedToAudioComponent())
		{
			btn_playcapturedaudio->SetColorAndOpacity(FLinearColor::Green);
		}
	}
}

void UUI_Audio_Record::_OnClickPlayCapturedAudio(UPSButton* sender)
{
	if (playerPawn == nullptr)
	{
		return;
	}
	
	if (playerPawn->GetAudioCaptureState() == false)
	{
		playerPawn->PlayAudio();
		
	}
}

bool UUI_Audio_Record::_CheckAudioCaptureState()
{
	if (playerPawn == nullptr)
	{
		return false;
	}
	return playerPawn->GetAudioCaptureState();
}
