// Fill out your copyright notice in the Description page of Project Settings.


#include "DGInteractableActor.h"

ADGInteractableActor::ADGInteractableActor()
{
	PrimaryActorTick.bCanEverTick = false;

	InteractableName = FText::FromString(GetName());
}

void ADGInteractableActor::Interact_Implementation(APawn* Initiator)
{

}

FText ADGInteractableActor::GetInteractableName() const
{
	return InteractableName;
}