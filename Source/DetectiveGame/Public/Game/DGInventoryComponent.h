// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DGInventoryComponent.generated.h"

class ADGInventoryItem;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DETECTIVEGAME_API UDGInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDGInventoryComponent();

	ADGInventoryItem* GetItem(uint16 index) const;

	bool HasAnItemInIventory(ADGInventoryItem* Item) const;

	void AddToInventory(ADGInventoryItem* Item);

	void RemoveFromInventory(ADGInventoryItem* Item);

protected:
	virtual void BeginPlay() override;

private:

	UPROPERTY(Transient)
	TArray<ADGInventoryItem*> Inventory;
};
