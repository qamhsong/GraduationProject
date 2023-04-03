// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "PS_GameInstance.generated.h"

class UAudioAnalyzerManager;


UCLASS()
class PROJECTSOUND_API UPS_GameInstance : public UGameInstance
{
	GENERATED_BODY()
	
	virtual void Init() override;
	virtual void Shutdown() override;
	virtual void BeginDestroy() override;

	TWeakObjectPtr<UAudioAnalyzerManager> AudioManager;
	
private:
	static TWeakObjectPtr<UPS_GameInstance> MyGameInstance;

public:
	UFUNCTION(BlueprintCallable, BlueprintPure, BlueprintCosmetic)
	static UPS_GameInstance* GetMyInstance();

};
