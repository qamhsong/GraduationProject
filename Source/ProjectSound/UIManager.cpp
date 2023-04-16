// Fill out your copyright notice in the Description page of Project Settings.


#include "UIManager.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "PS_GameInstance.h"
#include "PS_PlayerController.h"
#include "Components/Widget.h"


void UUIManager::InitManager()
{
	Super::InitManager();

	// currently there is no need of ui pool
	_UIPool.Empty();

	_CurrentInputMode = EUIInputMode::GameOnly;

}

void UUIManager::ResetManager()
{
	Super::ResetManager();
}

void UUIManager::ReleaseManager()
{
	Super::ReleaseManager();
	ResetManager();
}

EUIInputMode UUIManager::ApplyInputMode(EUIInputMode NewInputMode)
{
	EUIInputMode tempInputMode = EUIInputMode::GameOnly;

	UPS_GameInstance* ginst = GInst();

	if (ginst == nullptr)
	{
		UE_LOG(LogTemp, Warning , TEXT("NO GAME INSTANCE"));
		return tempInputMode;
	}
	UWorld* world = ginst->GetWorld();
	if (world == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("No World..?"))
		return tempInputMode;
	}

	auto* pc = world->GetFirstPlayerController<APS_PlayerController>();


	if (pc == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("NO PLAYER CONTROLLER"));
		return tempInputMode;
	}

	switch (NewInputMode)
	{
	case EUIInputMode::GameOnly:
	{
		UWidgetBlueprintLibrary::SetInputMode_GameOnly(pc);
		pc->SetShowMouseCursor(false);
	}
	break;
	case EUIInputMode::UIOnly:
	{
		UWidgetBlueprintLibrary::SetInputMode_UIOnlyEx(pc);
		pc->SetShowMouseCursor(true);
	}
	break;
	case EUIInputMode::GameAndUI:
	default:
	{
		FInputModeGameAndUI	InputMode;
		InputMode.SetHideCursorDuringCapture(false);
		pc->SetInputMode(InputMode);
		pc->SetShowMouseCursor(true);
	}
	break;
	}

	return tempInputMode;
}
