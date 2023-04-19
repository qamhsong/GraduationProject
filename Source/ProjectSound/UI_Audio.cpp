// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_Audio.h"
#include "UIManager.h"


void UUI_Audio::_OnCreate()
{
	Super::_OnCreate();
}

void UUI_Audio::_OnShow()
{
	Super::_OnShow();
	this->AddToViewport();
	GetUIManager()->ApplyInputMode(EUIInputMode::UIOnly);
}