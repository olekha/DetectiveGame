// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/DGGameInstance.h"
#include "DGEntriesManager.h"

UDGEntriesManager* UDGGameInstance::GetEntriesManager() const
{
	if (EntriesManager == nullptr && EntriesManagerClass != nullptr)
	{
		EntriesManager = NewObject<UDGEntriesManager>(GetWorld(), EntriesManagerClass);
	}
	return EntriesManager;
}