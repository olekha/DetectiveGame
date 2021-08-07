// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DGInteractable.h"
#include "DGInteractableActor.generated.h"

UCLASS()
class DETECTIVEGAME_API ADGInteractableActor : public AActor, public IDGInteractable

{
	GENERATED_BODY()
	
public:
	ADGInteractableActor();

	//**IDInteractable implementation - START*/
	virtual	void Interact_Implementation(APawn* Initiator) override;
	virtual FText GetInteractableName() const override;
	//**IDInteractable implementation - END*/

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText InteractableName;

};
