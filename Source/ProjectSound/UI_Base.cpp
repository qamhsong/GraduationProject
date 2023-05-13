// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_Base.h"

void UUI_Base::Create()
{
	_OnCreate();
}

void UUI_Base::_OnCreate()
{
	if(this->IsVisible())
		return;
	if (this->IsInViewport() == false)
	{
		if (this->GetParent() == nullptr)
		{
			this->AddToViewport();
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("UI Add to Viewport"));
}

void UUI_Base::_OnShow()
{
	if(this->IsVisible())
		return;
	if (this->IsInViewport() == false)
	{
		if (this->GetParent() == nullptr)
		{

		}
	}
	UE_LOG(LogTemp, Warning, TEXT("UI Add to ViewPort"));
}

void UUI_Base::_OnWidgetCalledFromParent()
{
}

void UUI_Base::Show()
{
	_OnShow();
}

void UUI_Base::OnWidgetCalledFromParent()
{
	_OnWidgetCalledFromParent();
}
