// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DGBrowsable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UDGBrowsable : public UInterface
{
	GENERATED_BODY()
};

class IDGBrowser;

class DETECTIVEGAME_API IDGBrowsable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual void SetItemForBrowsing(IDGBrowser* ForBrowser) = 0;

	virtual void RemoveItemFromBrowsing(IDGBrowser* FromBrowser) = 0;

	virtual UMaterialInterface* GetMaterialForBrowse() const = 0;

	virtual FVector2D GetInBrowserZoomLimits() const = 0;
};
