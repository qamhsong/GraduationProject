// Fill out your copyright notice in the Description page of Project Settings.


#include "ManagerBase.h"

void UManagerBase::InitManager()
{
	if (IsRooted() == false)
	{
		// Prevent Garbage Collection
		AddToRoot();
	}
}


void UManagerBase::ResetManager()
{
}

void UManagerBase::ReleaseManager()
{
	ResetManager();
	RemoveFromRoot();
}
