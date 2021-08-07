// Fill out your copyright notice in the Description page of Project Settings.


#include "DGInventoryItem.h"
#include "DGCharacter.h"
#include "DGInventoryComponent.h"

void ADGInventoryItem::Interact_Implementation(APawn* Initiator)
{
	ADGCharacter* CharacterInitiator = Cast<ADGCharacter>(Initiator);
	if (CharacterInitiator != nullptr)
	{
		CharacterInitiator->InventoryComponent->AddToInventory(this);
		HideItem();
	}
};