// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DGEntriesDiscovererComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DETECTIVEGAME_API UDGEntriesDiscovererComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDGEntriesDiscovererComponent();

	UFUNCTION(BlueprintCallable)
	void DiscoverEntry(int32 InIndex);

	UFUNCTION(BlueprintCallable)
	void DiscoverAllEntries();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

protected:

	UPROPERTY(EditAnywhere, meta = (MustImplement = "DGInvestigationSubject"))
	TArray<TSubclassOf<UObject>> EntriesListToDiscover;
};
