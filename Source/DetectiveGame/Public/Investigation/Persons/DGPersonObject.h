// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DGPerson.h"
#include "DGInvestigationSubject.h"
#include "DGPersonObject.generated.h"

UCLASS(BlueprintType, Blueprintable, hidedropdown)
class DETECTIVEGAME_API UDGPersonObject : public UObject, public IDGPerson, public IDGInvestigationSubject
{
	GENERATED_BODY()
	
public:

	/**IDGPerson - implementation START*/
	virtual FText GetFullName() const override;

	virtual FText GetFullDescription() const override;

	virtual FText GetDateOfBirth() const override;
	/**IDGPerson - implementation END*/

	/**IDGInvestigationSubject - implementation START*/
	virtual const TArray<EDGQuestionType>& GetQuestionsCanBeSuspectFor() const override;
	virtual const TArray<EDGQuestionType>& GetQuestionsCanBeSubjectFor() const override;
	virtual FText GetInvestigationSubjectName() const override;
	virtual FText GetInvestigationSubjectNameAsPartOfQuestion(EDGQuestionType InQuestionType) const override;
	/**IDGInvestigationSubject - implementation END*/

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText PersonFullName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText PersonNameAsPartOfQuestion;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText PersonFullDescription;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText PersonDateOfBirth;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<EDGQuestionType> ListOfQuestionsCanBeSuspectFor;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<EDGQuestionType> ListOfQuestionsCanBeSubjectFor;
};
