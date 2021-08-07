// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DGInteractable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UDGInteractable : public UInterface
{
	GENERATED_BODY()
};

class APawn;

class DETECTIVEGAME_API IDGInteractable
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interaction")
	void Interact(APawn* Initiator);
	virtual void Interact_Implementation(APawn* Initiator) = 0;

	virtual FText GetInteractableName() const = 0;
};
