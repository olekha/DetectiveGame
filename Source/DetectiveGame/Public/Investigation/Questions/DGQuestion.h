// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DGQuestion.generated.h"

enum class EDGQuestionType : uint8;
class IDGEvent;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UDGQuestion : public UInterface
{
	GENERATED_BODY()
};

class DETECTIVEGAME_API IDGQuestion
{
	GENERATED_BODY()

public:

	virtual EDGQuestionType GetQuestionType() const = 0;
	virtual FText GetQuestionFullText() const = 0;
	virtual FText GetQuestionPartOfSentenceText() const = 0;
};
