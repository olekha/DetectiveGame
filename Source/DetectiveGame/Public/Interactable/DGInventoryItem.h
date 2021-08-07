// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DGBrowseableActor.h"
#include "DGInventoryItem.generated.h"

UCLASS()
class DETECTIVEGAME_API ADGInventoryItem : public ADGBrowseableActor
{
	GENERATED_BODY()
	
protected:

	/**IDInteractable implementation - START*/
	virtual	void Interact_Implementation(class APawn* Initiator) override;
	/**IDInteractable - implementation END*/
};
