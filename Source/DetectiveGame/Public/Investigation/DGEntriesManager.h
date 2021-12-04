// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DetectiveGame.h"
#include "DGEntriesManager.generated.h"

class IDGInvestigationSubject;
class IDGQuestion;

USTRUCT(BlueprintType)
struct FDGSuspectContainer
{
	GENERATED_BODY();
	
	UPROPERTY(BlueprintReadOnly)
	TArray<FDGInvestigationSuspectInfo> Container;
};

UCLASS(BlueprintType, Blueprintable, hidedropdown)
class DETECTIVEGAME_API UDGEntriesManager : public UObject
{
	GENERATED_BODY()

public:

	static bool IsQuestionInfoValid(const FDGInvestigationQuestionInfo& InQuestionInfo);
	static bool IsSuspectInfoValid(const FDGInvestigationSuspectInfo& InQuestionInfo);

	void DiscoverNewEntry(TSubclassOf<UObject> InNewEntry);

	bool IsEntryDiscoveredAlready(TSubclassOf<UObject> InEntry) const;

	//Subject that can be considered as answer to question of required Type
	void GetSuspectsForQuestion(EDGQuestionType InQuestion, TArray<TScriptInterface<IDGInvestigationSubject>>& OutEntries) const;

	//Subject that can be used in sentences to form a question with required Type 
	void GetSubjectsForQuestion(EDGQuestionType InQuestion, TArray<TScriptInterface<IDGInvestigationSubject>>& OutEntries) const;

	TScriptInterface<IDGQuestion> GetQuestion(EDGQuestionType InQuestionType) const;

	void AddNewInvestigationCase(TPair<FDGInvestigationQuestionInfo, FDGInvestigationSuspectInfo> InNewInvestigationCase);

	const TArray<FDGInvestigationSuspectInfo>* GetSuspectsList(const FDGInvestigationQuestionInfo& InQuestion);

private:

	void OnNewEntriesDiscovered(TSubclassOf<UObject> InInvestigationEntry);

protected:

	//IDGQuestion
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, meta = (MustImplement = "DGQuestion"))
	TArray<TSubclassOf<UObject>> Questions;

private:

	//IDGEvent
	UPROPERTY(BlueprintReadOnly, Transient, meta = (AllowPrivateAccess))
	TArray<TSubclassOf<UObject>> DiscoveredEvents;

	//IDGPerson
	UPROPERTY(BlueprintReadOnly, Transient, meta = (AllowPrivateAccess))
	TArray<TSubclassOf<UObject>> DiscoveredPersons;

	//IDGPlac
	UPROPERTY(BlueprintReadOnly, Transient, meta = (AllowPrivateAccess))
	TArray<TSubclassOf<UObject>> DiscoveredPlaces;

	UPROPERTY(BlueprintReadOnly, Transient, meta = (AllowPrivateAccess))
	TMap<FDGInvestigationQuestionInfo, FDGSuspectContainer> InvestigationCases;
};
