// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DetectiveGame.h"
#include "DGEntriesManager.generated.h"

class IDGInvestigationSubject;
class IDGQuestion;

UCLASS(BlueprintType, Blueprintable, hidedropdown)
class DETECTIVEGAME_API UDGEntriesManager : public UObject
{
	GENERATED_BODY()

public:

	FORCEINLINE const TArray<TSubclassOf<UObject>>& GetDiscoveredEvents() const
	{
		return DiscoveredEvents;
	}

	FORCEINLINE const TArray<TSubclassOf<UObject>>& GetDiscoveredPersons() const
	{
		return DiscoveredPersons;
	}

	FORCEINLINE const TArray<TSubclassOf<UObject>>& GetDiscoveredPlaces() const
	{
		return DiscoveredPlaces;
	}

	FORCEINLINE const TArray<TSubclassOf<UObject>>& GetDiscoveredEvidence() const
	{
		return DiscoveredEvidence;
	}

	void CreateNewInvestigationCase(const TScriptInterface<IDGInvestigationSubject>& InSuspect);
	void DeleteInvestigationCase(const TScriptInterface<IDGInvestigationSubject>& InSuspect);

	void DiscoverNewEntry(TSubclassOf<UObject> InNewEntry);
	bool IsEntryDiscoveredAlready(TSubclassOf<UObject> InEntry) const;

	const FDGInvestigationSuspectInfo* GetCaseForInvestigationSubject(const TScriptInterface<IDGInvestigationSubject>& InInvestigationSubject) const;

	void AddArgumentForCase(const TScriptInterface<IDGInvestigationSubject>& InSuspect, TSubclassOf<UObject> InArgumentFor, bool bShouldCreateCaseIfNotExists = true);
	void AddArgumentAgainstCase(const TScriptInterface<IDGInvestigationSubject>& InSuspect, TSubclassOf<UObject> InArgumentAgainst, bool bShouldCreateCaseIfNotExists = true);

	void RemoveArgumentForCase(const TScriptInterface<IDGInvestigationSubject>& InSuspect, TSubclassOf<UObject> InArgumentFor);
	void RemoveArgumentAgainstCase(const TScriptInterface<IDGInvestigationSubject>& InSuspect, TSubclassOf<UObject> InArgumentAgainst);
	
	void RemoveArgumentForCase(const TScriptInterface<IDGInvestigationSubject>& InSuspect, const int32 InArgumentIndex);
	void RemoveArgumentAgainstCase(const TScriptInterface<IDGInvestigationSubject>& InSuspect, const int32 InArgumentIndex);

private:

	void OnNewEntriesDiscovered(TSubclassOf<UObject> InInvestigationEntry);

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

	//IDGEvidence
	UPROPERTY(BlueprintReadOnly, Transient, meta = (AllowPrivateAccess))
	TArray<TSubclassOf<UObject>> DiscoveredEvidence;

	UPROPERTY(BlueprintReadOnly, Transient, meta = (AllowPrivateAccess))
	TArray<FDGInvestigationSuspectInfo> InvestigationCases;
};
