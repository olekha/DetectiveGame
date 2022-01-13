// Fill out your copyright notice in the Description page of Project Settings.
#include "DGBrowseableActor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Pawn.h"
#include "DGBrowser.h"

// Sets default values
ADGBrowseableActor::ADGBrowseableActor()
{
	PrimaryActorTick.bCanEverTick = false;

	ItemMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BrowsableItemMeshComponent"));
	if (ItemMeshComponent != nullptr)
	{
		ItemMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		RootComponent = ItemMeshComponent;
	}

	ItemCollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("ItemCollisionComponent"));
	if (ItemCollisionComponent != nullptr)
	{
		ItemCollisionComponent->SetupAttachment(ItemMeshComponent);
		ItemCollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		ItemCollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		ItemCollisionComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	}

	BrowsableName = FText::FromString(GetName());
}

void ADGBrowseableActor::Interact_Implementation(APawn* Initiator)
{
	SetItemForBrowsing(Cast<IDGBrowser>(Initiator));
}

FText ADGBrowseableActor::GetInteractableName() const
{
	return BrowsableName;
}

void ADGBrowseableActor::HideItem()
{
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
	SetActorLocation(FVector(0.0f, 0.0f, -2000.0f));
}

void ADGBrowseableActor::ShowItem(FVector ShowLocation)
{
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
	SetActorLocation(ShowLocation);
}

UStaticMesh* const ADGBrowseableActor::GetItemMesh() const
{
	return (ItemMeshComponent != nullptr ? ItemMeshComponent->GetStaticMesh() : nullptr);
}

void ADGBrowseableActor::SetItemForBrowsing(IDGBrowser* ForBrowser)
{
	if (ForBrowser != nullptr)
	{
		if (!ForBrowser->IsBrowsingAnyItem())
		{
			ForBrowser->SetMeshForBrowsing(this);
		}
	}
}

void ADGBrowseableActor::RemoveItemFromBrowsing(IDGBrowser* FromBrowser)
{
	if(FromBrowser != nullptr)
	{
		if(FromBrowser->GetCurrentBrowsable() == this)
		{
			FromBrowser->ResetBrowsing();
		}
	}
}

UMaterialInterface* ADGBrowseableActor::GetMaterialForBrowse(const uint32 InMaterialSlot) const 
{	
	if(MaterialsForBrowse.Contains(InMaterialSlot))
	{
		return MaterialsForBrowse[InMaterialSlot];
	}

	return nullptr;
}

FVector2D ADGBrowseableActor::GetInBrowserZoomLimits() const 
{
	return FVector2D(MinZoomInBrowseValue, MaxZoomInBrowseValue);
}