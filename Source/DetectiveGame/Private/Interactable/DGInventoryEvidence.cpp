// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactable/DGInventoryEvidence.h"
#include "DGEntriesDiscovererComponent.h"

ADGInventoryEvidence::ADGInventoryEvidence()
{
	EntriesDiscovererComponent = CreateDefaultSubobject<UDGEntriesDiscovererComponent>(TEXT("EntriesDiscovererComponent"));
}

FText ADGInventoryEvidence::GetEvidenceName() const
{
	return GetInteractableName();
}

void ADGInventoryEvidence::Interact_Implementation(class APawn* Initiator)
{
	Super::Interact_Implementation(Initiator);

	EntriesDiscovererComponent->DiscoverAllEntries();
}
