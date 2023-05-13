// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_AudioOption.h"
#include "Components/VerticalBox.h"
#include "Components/TextBlock.h"
#include "UI_Audio.h"
#include "UIManager.h"


void UUI_AudioOption::_OnCreate()
{
	Super::_OnCreate();

}

void UUI_AudioOption::_OnShow()
{
	Super::_OnShow();
	this->AddToViewport();
	GetUIManager()->ApplyInputMode(EUIInputMode::GameAndUI);
	UE_LOG(LogTemp, Warning ,TEXT("_OnShow UI_Audio_Option"));
	for (auto widget : vbox_option_list->GetAllChildren())
	{
		if (UUI_Base* castWidget = Cast<UUI_Base>(widget))
		{
			castWidget->OnWidgetCalledFromParent();
			UE_LOG(LogTemp, Warning, TEXT("_OnShow UI_Audio_Base"));
		 }
	}
 
}
