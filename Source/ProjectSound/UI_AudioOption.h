// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI_Base.h"
#include "UI_AudioOption.generated.h"


class UVerticalBox;
class UTextBlock;

/**
 * 
 */
UCLASS()
class PROJECTSOUND_API UUI_AudioOption : public UUI_Base
{
	GENERATED_BODY()
	
public:

	static constexpr auto RESOURCE_PATH = TEXT("UI/UI_AudioOption");

protected:
	virtual void _OnCreate();
	virtual void _OnShow();

public:
	UPROPERTY(meta = (BindWidget))
	UVerticalBox* vbox_option_list;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* txt_AudioSettings;

};
