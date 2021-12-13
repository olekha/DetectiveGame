// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/DGGameInstance.h"
#include "DGEntriesManager.h"

UDGEntriesManager* UDGGameInstance::GetEntriesManager(UObject* InObjectContext)
{
	if(InObjectContext == nullptr)
	{
		return nullptr;
	}

	UWorld* WorldObject = InObjectContext->GetWorld();
	if(WorldObject == nullptr)
	{
		return nullptr;
	}

	UDGGameInstance* GameInstanceObject = WorldObject->GetGameInstance<UDGGameInstance>();
	if(GameInstanceObject == nullptr)
	{
		return nullptr;
	}

	if (GameInstanceObject->EntriesManager == nullptr && GameInstanceObject->EntriesManagerClass != nullptr)
	{
		GameInstanceObject->EntriesManager = NewObject<UDGEntriesManager>(WorldObject, GameInstanceObject->EntriesManagerClass);
	}

	return GameInstanceObject->EntriesManager;
}
