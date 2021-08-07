// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "DGBrowser.h"
#include "DGSuspect.h"
#include "DGCharacter.generated.h"

class ADGInventoryItem;
class UCameraComponent;
class UStaticMeshComponent;
class UBoxComponent;
class ADGBrowseableActor;
class UDGInventoryComponent;

UCLASS()
class DETECTIVEGAME_API ADGCharacter : public ACharacter, public IDGBrowser, public IDGSuspect
{
	GENERATED_BODY()

public:

	ADGCharacter();

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/**IDGBrowser - implementation START*/
	virtual void SetMeshForBrowsing(ADGBrowseableActor* Item) override;
	virtual void SetMeshForBrowsing(uint16 IndexInInventory) override;

	virtual void ResetBrowsing() override;

	virtual bool IsBrowsingAnyItem() const override;

	virtual ADGBrowseableActor* GetCurrentBrowsable() const override;
	/**IDGBrowser - implementation START*/

	/**IDGSuspect - implementation START*/
	virtual FText GetSuspectName() const override;
	/**IDGSuspect - implementation START*/

	void MakeLineTrace(TArray<FHitResult>& OutHit, struct FCollisionResponseParams CollisionResponseParams, ECollisionChannel ColisionChanel, UClass* ClassToFiltr = UClass::StaticClass()) const;

protected:

	virtual void BeginPlay() override;

	UFUNCTION()
	void MoveForward_Input(float value);

	UFUNCTION()
	void MoveRight_Input(float value);

	UFUNCTION()
	void LookUp_Input(float value);

	UFUNCTION()
	void Turn_Input(float value);

	UFUNCTION()
	void ZoomItem_Input(float value);

	UFUNCTION()
	void OnInteract_Input();

	UFUNCTION()
	void OnBrowseItem_Input();

public:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UDGInventoryComponent* InventoryComponent;

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* BrowseItemComponent;

	//Trigger that used to avoid render skip for browse item in case it appears behind a wall
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UBoxComponent* BrowseModeBoundComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UCameraComponent* CameraComponent;

	UPROPERTY(Transient, BlueprintReadOnly)
	ADGBrowseableActor* BrowseItem;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float DepthOfFieldValue = 1.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText CharacterName;
};
