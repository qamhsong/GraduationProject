// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Button.h"
#include "PSButton.generated.h"

UENUM()
enum class EPSButtonFunctionMethod : uint8 
{
	Pressed = 0,
	Released,
	LeftClicked,
	Hovered,
	Unhovered,
};

#ifdef BUTTON_DELEGATE_PRESSED
#undef BUTTON_DELEGATE_PRESSED
#endif // ButtonDelegate_Pressed
#define BUTTON_DELEGATE_PRESSED(Target, Object, FuncName)			\
		{															\
			Object->OnPressed.RemoveDynamic(Target, FuncName);		\
			Object->OnPressed.AddDynamic(Target, FuncName);			\
		}

#ifdef BUTTON_DELEGATE_RELEASED
#undef BUTTON_DELEGATE_RELEASED
#endif // ButtonDelegate_Released
#define BUTTON_DELEGATE_RELEASED(Target, Object, FuncName)			\
		{															\
			Object->OnReleased.RemoveDynamic(Target, FuncName);		\
			Object->OnReleased.AddDynamic(Target, FuncName);		\
		}

#ifdef BUTTON_DELEGATE_LEFT_CLICKED
#undef BUTTON_DELEGATE_LEFT_CLICKED
#endif // ButtonDelegate_Clicked
#define BUTTON_DELEGATE_LEFT_CLICKED(Target, Object, FuncName)		\
		{															\
			Object->OnClicked.RemoveDynamic(Target, FuncName);		\
			Object->OnClicked.AddDynamic(Target, FuncName);			\
		}

#ifdef BUTTON_DELEGATE_HOVERED
#undef BUTTON_DELEGATE_HOVERED
#endif // ButtonDelegate_Hovered
#define BUTTON_DELEGATE_HOVERED(Target, Object, FuncName)			\
		{															\
			Object->OnHovered.RemoveDynamic(Target, FuncName);		\
			Object->OnHovered.AddDynamic(Target, FuncName);			\
		}

#ifdef BUTTON_DELEGATE_UNHOVERED
#undef BUTTON_DELEGATE_UNHOVERED
#endif // ButtonDelegate_Unhovered
#define BUTTON_DELEGATE_UNHOVERED(Target, Object, FuncName)			\
		{															\
			Object->OnUnhovered.RemoveDynamic(Target, FuncName);	\
			Object->OnUnhovered.AddDynamic(Target, FuncName);		\
		}

class UPSButton;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPSButtonPressedEvent, UPSButton*, Sender);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPSButtonReleasedEvent, UPSButton*, Sender);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPSButtonClickedEvent, UPSButton*, Sender);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPSButtonHoveredEvent, UPSButton*, Sender);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPSButtonUnhoveredEvent, UPSButton*, Sender);


#define SAFE_BIND_DELEGATE_PRESSED(Target, Object, FuncName)	\
{																\
	Object->SetDelegate( EPSButtonFunctionMethod::Pressed );	\
	Object->OnPSPressed.RemoveDynamic(Target, FuncName);		\
	Object->OnPSPressed.AddDynamic(Target, FuncName);			\
}
#define SAFE_BIND_DELEGATE_RELEASED(Target, Object, FuncName)	\
{																	\
	Object->SetDelegate( EPSButtonFunctionMethod::Released );		\
	Object->OnPSReleased.RemoveDynamic(Target, FuncName);			\
	Object->OnPSReleased.AddDynamic(Target, FuncName);				\
}
#define SAFE_BIND_DELEGATE_LEFT_CLICKED(Target, Object, FuncName)	\
{																	\
	Object->SetDelegate( EPSButtonFunctionMethod::LeftClicked );	\
	Object->OnPSLeftClicked.RemoveDynamic(Target, FuncName);		\
	Object->OnPSLeftClicked.AddDynamic(Target, FuncName);			\
}

#define SAFE_BIND_DELEGATE_HOVERED(Target, Object, FuncName)	\
{																\
	Object->SetDelegate( EPSButtonFunctionMethod::Hovered );	\
	Object->OnPSHovered.RemoveDynamic(Target, FuncName);		\
	Object->OnPSHovered.AddDynamic(Target, FuncName);			\
}
#define SAFE_BIND_DELEGATE_UNHOVERED(Target, Object, FuncName)	\
{																\
	Object->SetDelegate( EPSButtonFunctionMethod::Unhovered );	\
	Object->OnPSUnhovered.RemoveDynamic(Target, FuncName);		\
	Object->OnPSUnhovered.AddDynamic(Target, FuncName);			\
}

#define SAFE_UNBIND_DELEGATE_PRESSED(Target, Object, FuncName)	\
{																	\
	Object->RemoveDelegate( EPSButtonFunctionMethod::Pressed );		\
	Object->OnPSPressed.RemoveDynamic(Target, FuncName);			\
}
#define SAFE_UNBIND_DELEGATE_RELEASED(Target, Object, FuncName)	\
{																	\
	Object->RemoveDelegate( EPSButtonFunctionMethod::Released );		\
	Object->OnPSReleased.RemoveDynamic(Target, FuncName);			\
}
#define SAFE_UNBIND_DELEGATE_LEFT_CLICKED(Target, Object, FuncName)	\
{																		\
	Object->RemoveDelegate( EPSButtonFunctionMethod::LeftClicked );		\
	Object->OnPSLeftClicked.RemoveDynamic(Target, FuncName);			\
}
#define SAFE_UNBIND_DELEGATE_HOVERED(Target, Object, FuncName)	\
{																	\
	Object->RemoveDelegate( EPSButtonFunctionMethod::Hovered );		\
	Object->OnPSHovered.RemoveDynamic(Target, FuncName);			\
}
#define SAFE_UNBIND_DELEGATE_UNHOVERED(Target, Object, FuncName)	\
{																	\
	Object->RemoveDelegate( EPSButtonFunctionMethod::Unhovered );	\
	Object->OnPSUnhovered.RemoveDynamic(Target, FuncName);			\
}

UCLASS(meta = (DisplayName = "PS_Button", ToolTip= "Test ToolTip"))
class PROJECTSOUND_API UPSButton : public UButton
{
	GENERATED_BODY()

public:
	FOnPSButtonPressedEvent		OnPSPressed;
	FOnPSButtonReleasedEvent	OnPSReleased;
	FOnPSButtonClickedEvent		OnPSLeftClicked;
	FOnPSButtonHoveredEvent		OnPSHovered;
	FOnPSButtonUnhoveredEvent	OnPSUnhovered;

	DECLARE_MULTICAST_DELEGATE_OneParam(FPSOnChangeProprty, const UPSButton*)
	FPSOnChangeProprty OnChangeProprty;

public:
	UPSButton(const FObjectInitializer& ObjectInitializer);

	virtual void SynchronizeProperties() override;
	void SetDelegate(EPSButtonFunctionMethod buttonFunction);
	void RemoveDelegate (EPSButtonFunctionMethod buttonFunction);

private:
	UFUNCTION()
	void _OnPressed();

	UFUNCTION()
	void _OnReleased();

	UFUNCTION()
	void _OnLeftClicked();

	UFUNCTION()
	void _OnHovered();

	UFUNCTION()
	void _OnUnHovered();
};
