// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_Start.h"
#include "UIManager.h"

void UUI_Start::_OnCreate()
{
	Super::_OnCreate();
}

void UUI_Start::_OnShow()
{
	Super::_OnShow();
	this->AddToViewport();
	GetUIManager()->ApplyInputMode(EUIInputMode::GameAndUI);
}
