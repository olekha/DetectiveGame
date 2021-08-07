// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DetectiveGame.generated.h"

UENUM(BlueprintType)
enum class EDGQuestionType : uint8
{
	None UMETA(Hidden),
	What,
	When,
	Where,
	How,
	Why,
	Who,
	Max UMETA(Hidden)
};

USTRUCT(BlueprintType)
struct FDGInvestigationQuestionInfo
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadOnly)
	EDGQuestionType QuestionType;

	UPROPERTY(Transient, BlueprintReadOnly)
	TSubclassOf<UObject> QuestionSubject;

	FDGInvestigationQuestionInfo() : QuestionType(EDGQuestionType::None), QuestionSubject(nullptr) {}	
	FDGInvestigationQuestionInfo(EDGQuestionType InQuestionType, TSubclassOf<UObject> InQuestionSubject) : QuestionType(InQuestionType), QuestionSubject(InQuestionSubject) {}
	FDGInvestigationQuestionInfo(EDGQuestionType InQuestionType, const TScriptInterface<class IDGInvestigationSubject> InQuestionSubject)
	{
		QuestionType = InQuestionType;
		QuestionSubject = TSubclassOf<UObject>(InQuestionSubject.GetObject()->GetClass());
	}
	
	FDGInvestigationQuestionInfo(const TScriptInterface<class IDGQuestion> InQuestion, const TScriptInterface<class IDGInvestigationSubject> InQuestionSubject);

	bool operator==(const FDGInvestigationQuestionInfo& Other) const
	{
		return this->QuestionType == Other.QuestionType && this->QuestionSubject == Other.QuestionSubject;
	}
};


FORCEINLINE uint32 GetTypeHash(const FDGInvestigationQuestionInfo& Thing)
{
	uint32 Hash = FCrc::MemCrc32(&Thing, sizeof(FDGInvestigationQuestionInfo));

	return Hash;
}

USTRUCT(Blueprintable, BlueprintType)
struct FDGInvestigationSuspectInfo
{
	GENERATED_BODY()

	UPROPERTY(Transient, BlueprintReadOnly)
	TSubclassOf<UObject> InvestigationSuspect;

	UPROPERTY(Transient, BlueprintReadOnly)
	TArray<TSubclassOf<UObject>> ProsList;

	UPROPERTY(Transient, BlueprintReadOnly)
	TArray<TSubclassOf<UObject>> ConsList;

	bool operator==(const FDGInvestigationSuspectInfo& Other) const
	{
		return this->InvestigationSuspect == Other.InvestigationSuspect;
	}
};