// Fill out your copyright notice in the Description page of Project Settings.


#include "DGEntriesDiscovererComponent.h"
#include "DGGameInstance.h"
#include "DGEntriesManager.h"
#include "DGInvestigationSubject.h"

// Sets default values for this component's properties
UDGEntriesDiscovererComponent::UDGEntriesDiscovererComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

// Called when the game starts
void UDGEntriesDiscovererComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

}

void UDGEntriesDiscovererComponent::DiscoverEntry(int32 InIndex)
{
	if(EntriesListToDiscover.IsValidIndex(InIndex) && IsValid(EntriesListToDiscover[InIndex]))
	{
		UDGEntriesManager* EntriesManager = UDGGameInstance::GetEntriesManager(this);
		if (EntriesManager != nullptr)
		{
			EntriesManager->DiscoverNewEntry(EntriesListToDiscover[InIndex]);
		}
	}
}

void UDGEntriesDiscovererComponent::DiscoverAllEntries()
{
	for(int32 i = 0; i < EntriesListToDiscover.Num(); ++i)
	{
		DiscoverEntry(i);
	}
}
