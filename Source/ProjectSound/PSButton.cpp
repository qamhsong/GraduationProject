// Fill out your copyright notice in the Description page of Project Settings.


#include "PSButton.h"
#include "Components/ButtonSlot.h"
#include "PS_GameInstance.h"
#include "PS_Singleton.h"

UPSButton::UPSButton(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{

}


void UPSButton::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	if (!IsValid(this))
	{
		return;
	}

}

void UPSButton::SetDelegate(EPSButtonFunctionMethod buttonFunction)
{
	switch (buttonFunction)
	{
	case EPSButtonFunctionMethod::Pressed:
		BUTTON_DELEGATE_PRESSED(this, this, &UPSButton::_OnPressed);
		break;
	case EPSButtonFunctionMethod::Released:
		BUTTON_DELEGATE_RELEASED(this, this, &UPSButton::_OnReleased);
		break;
	case EPSButtonFunctionMethod::LeftClicked:
		BUTTON_DELEGATE_LEFT_CLICKED(this, this, &UPSButton::_OnLeftClicked);
		break;
	case EPSButtonFunctionMethod::Hovered:
		BUTTON_DELEGATE_HOVERED(this, this, &UPSButton::_OnHovered);
		break;
	case EPSButtonFunctionMethod::Unhovered:
		BUTTON_DELEGATE_UNHOVERED(this, this, &UPSButton::_OnUnHovered);
		break;
	}
}

void UPSButton::RemoveDelegate(EPSButtonFunctionMethod buttonFunction)
{
	switch (buttonFunction)
	{
	case EPSButtonFunctionMethod::Pressed:
		OnPressed.RemoveDynamic(this, &UPSButton::_OnPressed);
		break;
	case EPSButtonFunctionMethod::Released:
		OnReleased.RemoveDynamic(this, &UPSButton::_OnReleased);
		break;
	case EPSButtonFunctionMethod::LeftClicked:
		OnClicked.RemoveDynamic(this, &UPSButton::_OnLeftClicked);
		break;
	case EPSButtonFunctionMethod::Hovered:
		OnHovered.RemoveDynamic(this, &UPSButton::_OnHovered);
		break;
	case EPSButtonFunctionMethod::Unhovered:
		OnUnhovered.RemoveDynamic(this, &UPSButton::_OnUnHovered);
		break;
	}
}

void UPSButton::_OnPressed()
{
	if (OnPSPressed.IsBound())
	{
		OnPSPressed.Broadcast(this);
	}
}

void UPSButton::_OnReleased()
{
	if (OnPSReleased.IsBound())
	{
		OnPSReleased.Broadcast(this);
	}
}

void UPSButton::_OnLeftClicked()
{
	if (OnPSLeftClicked.IsBound())
	{
		OnPSLeftClicked.Broadcast(this);

		UPS_GameInstance* ginst = UPS_GameInstance::GetMyInstance();
		if (ginst == nullptr)
		{
			return;
		}
		UWorld* world = ginst->GetWorld();
		if (world == nullptr)
		{
			return;
		}
		// for later possible UI Sound bind
	}
}

void UPSButton::_OnHovered()
{
	if (OnPSHovered.IsBound())
	{
		OnPSHovered.Broadcast(this);
	}
}

void UPSButton::_OnUnHovered()
{
	if (OnPSUnhovered.IsBound())
	{
		OnPSUnhovered.Broadcast(this);
	}
}

