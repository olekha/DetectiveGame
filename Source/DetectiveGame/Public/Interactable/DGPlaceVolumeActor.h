// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DGPlaceVolumeActor.generated.h"

class UDGPlaceObject;
class UDGEntriesDiscovererComponent;
class UBoxComponent;
class UPrimitiveComponent;

UCLASS()
class DETECTIVEGAME_API ADGPlaceVolumeActor : public AActor
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	ADGPlaceVolumeActor();

private:

	UFUNCTION()
	void OnBoxVolumeComponentOverlaps(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess))
	UBoxComponent* BoxVolumeComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess))
	UDGEntriesDiscovererComponent* EntriesDiscovererComponent;

};
