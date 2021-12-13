// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DGInventoryItem.h"
#include "DGEvidence.h"
#include "DGInventoryEvidence.generated.h"

class UDGEntriesDiscovererComponent;

UCLASS()
class DETECTIVEGAME_API ADGInventoryEvidence : public ADGInventoryItem, public IDGEvidence
{
	GENERATED_BODY()

public:

	ADGInventoryEvidence();

/**IDGEvidence implementation - START*/
	virtual FText GetEvidenceName() const override;
/**IDGEvidence - implementation END*/
	
/**IDInteractable implementation - START*/
	virtual	void Interact_Implementation(class APawn* Initiator) override;
/**IDInteractable - implementation END*/

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess))
	UDGEntriesDiscovererComponent* EntriesDiscovererComponent;
};
