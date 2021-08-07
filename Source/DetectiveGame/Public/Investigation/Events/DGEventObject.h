// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DGEvent.h"
#include "DGInvestigationSubject.h"
#include "DetectiveGame.h"
#include "DGEventObject.generated.h"

UCLASS(BlueprintType, Blueprintable, hidedropdown)
class DETECTIVEGAME_API UDGEventObject : public UObject, public IDGEvent, public IDGInvestigationSubject
{
	GENERATED_BODY()
	
public:

	/**IDGEvent - implementation START*/
	virtual FText GetEventName() const override;

	virtual FText GetEventDescription() const override;
	/**IDGEvent - implementation END*/

	/**IDGInvestigationSubject - implementation START*/
	virtual const TArray<EDGQuestionType>& GetQuestionsCanBeSuspectFor() const override;
	virtual const TArray<EDGQuestionType>& GetQuestionsCanBeSubjectFor() const override;
	virtual FText GetInvestigationSubjectName() const override;
	/**IDGInvestigationSubject - implementation END*/

protected:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText EventName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText EventDescription;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<EDGQuestionType> ListOfQuestionsCanBeSuspectFor;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<EDGQuestionType> ListOfQuestionsCanBeSubjectFor;
};
