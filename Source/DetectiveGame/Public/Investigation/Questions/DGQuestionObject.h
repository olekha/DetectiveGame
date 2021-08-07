// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DGQuestion.h"
#include "DGQuestionObject.generated.h"

UCLASS(BlueprintType, Blueprintable, hidedropdown)
class DETECTIVEGAME_API UDGQuestionObject : public UObject, public IDGQuestion
{
	GENERATED_BODY()

public:

	virtual EDGQuestionType GetQuestionType() const override;
	virtual FText GetQuestionFullText() const override;
	virtual FText GetQuestionPartOfSentenceText() const override;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EDGQuestionType QuestionType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText FullText;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText PartOfSentenceText;
};
