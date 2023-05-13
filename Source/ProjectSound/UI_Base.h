// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI_Base.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTSOUND_API UUI_Base : public UUserWidget
{
	GENERATED_BODY()
	
public:
	// Assign Resource Path.
	// UI Manager class will create the user interface blueprint which matches the resource path.
	static constexpr auto RESOURCE_PATH = TEXT("");

	void Create();
	void Show();
	void OnWidgetCalledFromParent();

protected:
	virtual void _OnCreate();
	virtual void _OnShow();
	virtual void _OnWidgetCalledFromParent();
};
