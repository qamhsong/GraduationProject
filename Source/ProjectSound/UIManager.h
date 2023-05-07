// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ManagerBase.h"
#include "PS_Singleton.h"
#include "Blueprint/UserWidget.h"
#include "UI_Base.h"
#include "UIManager.generated.h"


UENUM()
enum class EUIInputMode : uint8
{
	GameOnly = 0,
	GameAndUI,
	UIOnly,
	EUIInputMode_Max,
};

UCLASS()
class PROJECTSOUND_API UUIManager : public UManagerBase
{
	GENERATED_BODY()
	
public:
	virtual void InitManager() override;
	virtual void ResetManager() override;
	virtual void ReleaseManager() override;

	
	UPROPERTY()
	TMap<UUI_Base*, float> _UIPool;

	UPROPERTY()
	EUIInputMode _CurrentInputMode = EUIInputMode::UIOnly;

	EUIInputMode ApplyInputMode(EUIInputMode NewInputMode);

	template<typename T>
	T* Create();

	template<typename T>
	T* Show();

	template<typename T>
	T* Hide();
};

template<typename T>
T* UUIManager::Create()
{
	T* UI = nullptr;

	FString path = TEXT("");
	path.Append("/Game/");
	path.Append(T::RESOURCE_PATH);
	UClass* uiClass = ConstructorHelpersInternal::FindOrLoadClass(path, UUI_Base::StaticClass());

	if (!uiClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("uiClass Load Fail"));
		return nullptr;
	}
	UI = CreateWidget<T>(UPS_GameInstance::GetMyInstance()->GetWorld(), uiClass);
	
	if(UI == nullptr)
		return nullptr;

	UI->Create();

	return UI;
}

template<typename T>
T* UUIManager::Show()
{
	T* UI = Create<T>();
	if (UI)
	{
		UI->Show();
	}
	return UI;
}

template<typename T>
inline T* UUIManager::Hide()
{
	return nullptr;
}

FORCEINLINE static class UUIManager* GetUIManager()
{
	return PSSingleton->GetManager<UUIManager>();
}