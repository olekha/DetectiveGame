// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DGEvidence.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UDGEvidence : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class DETECTIVEGAME_API IDGEvidence
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual FText GetEvidenceName() const = 0;

};
