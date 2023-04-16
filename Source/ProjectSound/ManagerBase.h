// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ManagerBase.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class PROJECTSOUND_API UManagerBase : public UObject
{
	GENERATED_BODY()
	
public:

	virtual void InitManager();
	virtual void ResetManager();
	virtual void ReleaseManager();

	virtual void Startup(){}
	virtual void Shutdown(){}
};
