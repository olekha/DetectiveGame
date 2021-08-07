// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DGInteractable.h"
#include "DGBrowsable.h"
#include "DGBrowseableActor.generated.h"

class IDGBrowser;

UCLASS()
class DETECTIVEGAME_API ADGBrowseableActor : public AActor, public IDGInteractable, public IDGBrowsable
{
	GENERATED_BODY()

public:

	ADGBrowseableActor();

	//**IDInteractable implementation - START*/
	virtual void Interact_Implementation(class APawn* Initiator) override;
	virtual FText GetInteractableName() const override;
	//**IDInteractable implementation - END*/

	/**IDGInvestigationSubject implementation - START*/
	virtual void SetItemForBrowsing(IDGBrowser* ForBrowser) override;
	virtual void RemoveItemFromBrowsing(IDGBrowser* FromBrowser) override;
	virtual UMaterialInterface* GetMaterialForBrowse() const override;
	virtual FVector2D GetInBrowserZoomLimits() const override;
	/**IDGInvestigationSubject - implementation END*/

	void HideItem();
	void ShowItem(FVector ShowLocation);

	UStaticMesh* const GetItemMesh() const;

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "View")
	float MinZoomInBrowseValue = -50.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "View")
	float MaxZoomInBrowseValue = 0.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "View")
	UMaterialInterface* MaterialForBrowse;

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UStaticMeshComponent* ItemMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UBoxComponent* ItemCollisionComponent;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText BrowsableName;
};
