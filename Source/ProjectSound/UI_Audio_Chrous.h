// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI_Base.h"
#include "UI_Audio_Chrous.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class PROJECTSOUND_API UUI_Audio_Chrous : public UUI_Base
{
	GENERATED_BODY()

	static constexpr auto RESOURCE_PATH = TEXT("UI/UI_Audio_Chorus");
	
};
