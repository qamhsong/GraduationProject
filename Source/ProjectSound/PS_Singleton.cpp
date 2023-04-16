// Fill out your copyright notice in the Description page of Project Settings.


#include "PS_Singleton.h"

PROJECTSOUND_API UPS_Singleton* PSSingleton;

UPS_Singleton::UPS_Singleton(const FObjectInitializer& objectInitializer)
	: Super(objectInitializer)
	, _IsInit(false)
{
	if (HasAnyFlags(RF_ClassDefaultObject))
	{
		PSSingleton = this;
	}

	_Managers.Reset();
	_ManagersReserve.Reset();
}

void UPS_Singleton::_AddManager(UClass* staticClass, UManagerBase* newObject)
{
	check(staticClass);
	check(newObject);

	if (_ManagersReserve.Contains(staticClass))
	{
		_ManagersReserve.Emplace(staticClass, newObject);
		newObject->InitManager();
	}
	_ProcessReserve();
}

void UPS_Singleton::_ProcessReserve()
{
	if (_ManagersReserve.Num() >= 1)
	{
		for (const auto& pair : _ManagersReserve)
		{
			_Managers.Add(pair.Key, pair.Value);
		}
		_ManagersReserve.Reset();
	}
}

void UPS_Singleton::_RemoveAllManagers()
{
	for (const auto& pair : _Managers)
	{
		if (IsValid(pair.Value))
		{
			pair.Value->ReleaseManager();
		}
	}

	TArray<UManagerBase*> _ManagersForRemoval;
	_Managers.GenerateValueArray(_ManagersForRemoval);

	for (int32 reverseIndex = _ManagersForRemoval.Num() - 1; 0 <= reverseIndex; reverseIndex--)
	{
		if (_ManagersForRemoval[reverseIndex])
		{
			_ManagersForRemoval[reverseIndex]->ReleaseManager();
			_Managers.Remove(_ManagersForRemoval[reverseIndex]->GetClass());
		}
	}

	_ManagersForRemoval.Empty();
}

void UPS_Singleton::_Init(UPS_GameInstance* gameInstance)
{
	ensure(gameInstance);
	_GameInstance = gameInstance;

	_IsInit = true;

}

void UPS_Singleton::_Shutdown()
{
	_RemoveAllManagers();
	_ManagersReserve.Reset();
	_Managers.Reset();

}
