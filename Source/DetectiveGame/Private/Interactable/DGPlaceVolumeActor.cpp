// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactable/DGPlaceVolumeActor.h"
#include "Components/BoxComponent.h"
#include "DGCharacter.h"
#include "DGEntriesDiscovererComponent.h"
#include "Components/PrimitiveComponent.h"

// Sets default values
ADGPlaceVolumeActor::ADGPlaceVolumeActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	BoxVolumeComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxVolumeComponent"));
	if (BoxVolumeComponent != nullptr)
	{
		BoxVolumeComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

		BoxVolumeComponent->OnComponentBeginOverlap.AddUniqueDynamic(this, &ADGPlaceVolumeActor::OnBoxVolumeComponentOverlaps);

		RootComponent = BoxVolumeComponent;
	}

	EntriesDiscovererComponent = CreateDefaultSubobject<UDGEntriesDiscovererComponent>(TEXT("EntriesDiscovererComponent"));
}

void ADGPlaceVolumeActor::OnBoxVolumeComponentOverlaps(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ADGCharacter* Character = Cast<ADGCharacter>(OtherActor);
	if(Character != nullptr)
	{
		EntriesDiscovererComponent->DiscoverAllEntries();

		BoxVolumeComponent->OnComponentBeginOverlap.RemoveDynamic(this, &ADGPlaceVolumeActor::OnBoxVolumeComponentOverlaps);
	}
}