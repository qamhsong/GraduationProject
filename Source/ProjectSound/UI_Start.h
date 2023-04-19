// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI_Base.h"
#include "UI_Start.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTSOUND_API UUI_Start : public UUI_Base
{
	GENERATED_BODY()
	
public:
	static constexpr auto RESOURCE_PATH = TEXT("UI/UI_Start");

protected:
	virtual void _OnCreate();
	virtual void _OnShow();
};