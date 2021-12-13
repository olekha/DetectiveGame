// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DetectiveGame.generated.h"

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

	bool operator==(const TScriptInterface<class IDGInvestigationSubject>& Other) const
	{
		if(Other == nullptr)
		{
			return InvestigationSuspect == nullptr;
		}

		return this->InvestigationSuspect == TSubclassOf<UObject>(Other.GetObject()->GetClass());
	}

	FDGInvestigationSuspectInfo& operator+(const FDGInvestigationSuspectInfo& Other)
	{
		this->ProsList.Append(Other.ProsList);
		this->ConsList.Append(Other.ConsList);

		return *this;
	}

	FDGInvestigationSuspectInfo& operator+=(const FDGInvestigationSuspectInfo& Other)
	{
		*this = *this + Other;

		return *this;
	}
};

enum class EDGFilterOption : uint8
{
	People,
	Places,
	Items,
	Events,
};
