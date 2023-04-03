// Fill out your copyright notice in the Description page of Project Settings.


#include "PS_GameInstance.h"
#include "AudioAnalyzerManager.h"

TWeakObjectPtr<UPS_GameInstance> UPS_GameInstance::MyGameInstance = nullptr;

void UPS_GameInstance::Init()
{
	Super::Init();

	AudioManager = NewObject<UAudioAnalyzerManager>(UAudioAnalyzerManager::StaticClass());

#if UE_EDITOR
	UPS_GameInstance::MyGameInstance = this;
#else
	if (UPS_GameInstance::MyGameInstance.IsValid() == false)
	{
		UPS_GameInstance::MyGameInstance = this;
	}
#endif
}

void UPS_GameInstance::Shutdown()
{
	Super::Shutdown();
}

void UPS_GameInstance::BeginDestroy()
{
	Super::BeginDestroy();
}

UPS_GameInstance* UPS_GameInstance::GetMyInstance()
{
	if (UPS_GameInstance::MyGameInstance.IsValid())
	{
		return UPS_GameInstance::MyGameInstance.Get();
	}

	return nullptr;
}
