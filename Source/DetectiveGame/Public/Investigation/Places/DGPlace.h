// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DGPlace.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UDGPlace : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class DETECTIVEGAME_API IDGPlace
{
	GENERATED_BODY()

public:

	virtual FText GetPlaceName() const = 0;
};
