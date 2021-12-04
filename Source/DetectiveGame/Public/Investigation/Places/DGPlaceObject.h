// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DGPlace.h"
#include "DGInvestigationSubject.h"
#include "DGPlaceObject.generated.h"

UCLASS(BlueprintType, Blueprintable, hidedropdown)
class DETECTIVEGAME_API UDGPlaceObject : public UObject, public IDGPlace, public IDGInvestigationSubject
{
	GENERATED_BODY()
	
public: 

	/**IDGPlace - implementation START*/
	virtual FText GetPlaceName() const override;
	/**IDGPlace - implementation END*/

	/**IDGInvestigationSubject - implementation START*/
	virtual const TArray<EDGQuestionType>& GetQuestionsCanBeSuspectFor() const override;
	virtual const TArray<EDGQuestionType>& GetQuestionsCanBeSubjectFor() const override;
	virtual FText GetInvestigationSubjectName() const override;
	virtual FText GetInvestigationSubjectNameAsPartOfQuestion(EDGQuestionType InQuestionType) const override;
	/**IDGInvestigationSubject - implementation END*/

protected:
	
	UPROPERTY(EditAnywhere)
	FText PlaceName;

	UPROPERTY(EditAnywhere)
	FText PlaceNameAsPartOfQuestion;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<EDGQuestionType> ListOfQuestionsCanBeSuspectFor;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<EDGQuestionType> ListOfQuestionsCanBeSubjectFor;

};
