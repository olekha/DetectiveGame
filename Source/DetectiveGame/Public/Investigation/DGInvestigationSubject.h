// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DGInvestigationSubject.generated.h"

enum class EDGQuestionType : uint8;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UDGInvestigationSubject : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class DETECTIVEGAME_API IDGInvestigationSubject
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual const TArray<EDGQuestionType>& GetQuestionsCanBeSuspectFor() const = 0;

	virtual const TArray<EDGQuestionType>& GetQuestionsCanBeSubjectFor() const = 0;

	virtual FText GetInvestigationSubjectName() const = 0;

	virtual FText GetInvestigationSubjectNameAsPartOfQuestion(EDGQuestionType InQuestionType) const = 0;

};
