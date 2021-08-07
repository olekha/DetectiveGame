// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DGBrowser.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UDGBrowser : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class DETECTIVEGAME_API IDGBrowser
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual void SetMeshForBrowsing(class ADGBrowseableActor* Item) = 0;
	virtual void SetMeshForBrowsing(uint16 IndexInInventory) = 0;

	virtual ADGBrowseableActor* GetCurrentBrowsable() const = 0;

	virtual void ResetBrowsing() = 0;

	virtual bool IsBrowsingAnyItem() const = 0;

};
