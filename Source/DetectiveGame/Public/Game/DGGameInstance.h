// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "DGGameInstance.generated.h"

class UDGEntriesManager;
class UDGEventsManager;

UCLASS(hidedropdown)
class DETECTIVEGAME_API UDGGameInstance : public UGameInstance
{
	GENERATED_BODY()

	friend class UDGGameInstance;

public:

	static UDGEntriesManager* GetEntriesManager(UObject* InObjectContext);

protected:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UDGEntriesManager> EntriesManagerClass;	

	UPROPERTY(Transient)
	UDGEntriesManager* EntriesManager;

};
