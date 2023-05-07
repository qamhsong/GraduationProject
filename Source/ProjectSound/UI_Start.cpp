// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_Start.h"
#include "UIManager.h"
#include "UI_Audio.h"
#include "PSButton.h"

void UUI_Start::_OnCreate()
{
	Super::_OnCreate();

	SAFE_BIND_DELEGATE_LEFT_CLICKED(this, btn_Start, &UUI_Start::_OnClickStart);

}

void UUI_Start::_OnShow()
{
	Super::_OnShow();
	this->AddToViewport();
	GetUIManager()->ApplyInputMode(EUIInputMode::GameAndUI);
}

void UUI_Start::_OnClickStart(UPSButton* sender)
{
	this->SetVisibility(ESlateVisibility::Collapsed);
	GetUIManager()->Show<UUI_Audio>();
	UE_LOG(LogTemp, Warning, TEXT("UI_Audio activate"));
}

