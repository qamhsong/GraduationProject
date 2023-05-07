// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine.h"

#pragma region ----------Delegate----------
#define PS_MULTICAST_DELEGATE_ADD(Delegate, Func) \
	if( false == Delegate.IsBoundToObject( this ) ) \
		Delegate.AddUObject( this, &Func )

#define PS_MULTICAST_DELEGATE_REMOVE(Delegate) \
	if( true == Delegate.IsBoundToObject( this ) ) \
		Delegate.RemoveAll( this )

#define PS_DELEGATE_BIND(Delegate, Func) \
	if( false == Delegate.IsBoundToObject( this ) ) \
		Delegate.BindUObject( this, &Func )

#define PS_DELEGATE_UNBIND(Delegate) \
	if( true == Delegate.IsBoundToObject( this ) ) \
		Delegate.Unbind()	
#pragma endregion ----------Delegate----------
