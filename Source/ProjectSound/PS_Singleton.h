// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "PS_GameInstance.h"
#include "ManagerBase.h"
#include "PS_Singleton.generated.h"


/**
 * 
 */
UCLASS()
class PROJECTSOUND_API UPS_Singleton : public UObject
{
	GENERATED_BODY()

public:
	friend class UPS_GameInstance;

	UPS_Singleton(const FObjectInitializer& objectInitializer);

	TWeakObjectPtr<UPS_GameInstance> _GameInstance;

	bool _IsInit;

	UPROPERTY()
	TMap<UClass*, UManagerBase*> _Managers;

	UPROPERTY()
	TMap<UClass*, UManagerBase*> _ManagersReserve;

	template<class T>
	void AddManager();

	void _AddManager(UClass* staticClass, UManagerBase* newObject);

	void _ProcessReserve();

	void _RemoveAllManagers();

	template<class T>
	T* GetManager();

	template<class T>
	T* _GetManager();

protected:
	void  _Init(UPS_GameInstance* gameInstance);

	void _Shutdown();
};


extern PROJECTSOUND_API class UPS_Singleton* PSSingleton;

template<class T>
T* UPS_Singleton::_GetManager()
{
	auto* staticClass = T::StaticClass();

	if (_ManagersReserve.Contains(staticClass) == true)
	{
		return Cast<T>(*_ManagersReserve.Find(staticClass));
	}
	if (_Managers.Contains(staticClass) == true)
	{
		return Cast<T>(*_Managers.Find(staticClass));
	}
	return nullptr;
}

template<class T>
inline void UPS_Singleton::AddManager()
{
	_AddManager(T::StaticClass(), NewObject<T>(this));
}

template<class T>
T* UPS_Singleton::GetManager()
{
	if (_GetManager<T>() == nullptr)
	{
		auto* staticClass = T::StaticClass();
		_AddManager(staticClass, NewObject<T>(this));
	}
	return Cast<T>(_GetManager<T>());
}

FORCEINLINE static UPS_GameInstance* GInst()
{
	if (PSSingleton == nullptr)
		return nullptr;
	if (PSSingleton->_GameInstance.IsValid() == false)
		return nullptr;
	return PSSingleton->_GameInstance.Get();
}