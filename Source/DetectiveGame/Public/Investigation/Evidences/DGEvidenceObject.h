// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DGEvidence.h"
#include "DGInvestigationSubject.h"
#include "DGEvidenceObject.generated.h"

UCLASS(BlueprintType, Blueprintable, hidedropdown)
class DETECTIVEGAME_API UDGEvidenceObject : public UObject, public IDGEvidence, public IDGInvestigationSubject
{
	GENERATED_BODY()

public:
	
	/**IDGInvestigationSubject - implementation START*/
	virtual FText GetInvestigationSubjectName() const override;
	/**IDGInvestigationSubject - implementation END*/

	/**IDGInvestigationSubject - implementation START*/
	virtual FText GetEvidenceName() const override;
	/**IDGInvestigationSubject - implementation END*/

protected:

	UPROPERTY(EditAnywhere)
	FText EvidenceName;
};
