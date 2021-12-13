// Fill out your copyright notice in the Description page of Project Settings.


#include "DGEntriesManager.h"
#include "DGInvestigationSubject.h"
#include "DGEvent.h"
#include "DGPerson.h"
#include "DGPlace.h"
#include "DGEvidence.h"
#include "DGGameInstance.h"
#include "DGCharacter.h"
#include "GameFramework/PlayerController.h"

void UDGEntriesManager::CreateNewInvestigationCase(const TScriptInterface<IDGInvestigationSubject>& InSuspect)
{
	if (InSuspect != nullptr)
	{
		const int32 ExistsUnderIndex = InvestigationCases.IndexOfByKey(InSuspect);
		if(ExistsUnderIndex == INDEX_NONE)
		{
			FDGInvestigationSuspectInfo& NewCase = InvestigationCases.Add_GetRef(FDGInvestigationSuspectInfo());

			NewCase.InvestigationSuspect = TSubclassOf<UObject>(InSuspect.GetObject()->GetClass());
		}
	}
}

void UDGEntriesManager::DeleteInvestigationCase(const TScriptInterface<IDGInvestigationSubject>& InSuspect)
{
	if (InSuspect != nullptr)
	{
		InvestigationCases.RemoveAll([InSuspect](const FDGInvestigationSuspectInfo& InCase){
			return  InCase == InSuspect;
		});
	}
}

void UDGEntriesManager::DiscoverNewEntry(TSubclassOf<UObject> InNewEntry)
{
	if (InNewEntry == nullptr
		|| !InNewEntry->ImplementsInterface(UDGInvestigationSubject::StaticClass())
		|| IsEntryDiscoveredAlready(InNewEntry))
	{
		return;
	}

	if(InNewEntry->ImplementsInterface(UDGEvent::StaticClass()))
	{
		DiscoveredEvents.AddUnique(InNewEntry);
	}

	if (InNewEntry->ImplementsInterface(UDGPerson::StaticClass()))
	{
		DiscoveredPersons.AddUnique(InNewEntry);
	}

	if (InNewEntry->ImplementsInterface(UDGPlace::StaticClass()))
	{
		DiscoveredPlaces.AddUnique(InNewEntry);
	}

	if (InNewEntry->ImplementsInterface(UDGEvidence::StaticClass()))
	{
		DiscoveredEvidence.AddUnique(InNewEntry);
	}

	OnNewEntriesDiscovered(InNewEntry);
}

bool UDGEntriesManager::IsEntryDiscoveredAlready(TSubclassOf<UObject> InEntry) const
{
	if (InEntry != nullptr)
	{
		if (DiscoveredEvents.Contains(InEntry))
		{
			return true;
		}

		if (DiscoveredPersons.Contains(InEntry))
		{
			return true;
		}	
		
		if (DiscoveredPlaces.Contains(InEntry))
		{
			return true;
		}

		if (DiscoveredEvidence.Contains(InEntry))
		{
			return true;
		}
	}

	return false;
}

const FDGInvestigationSuspectInfo* UDGEntriesManager::GetCaseForInvestigationSubject(const TScriptInterface<IDGInvestigationSubject>& InInvestigationSubject) const
{
	return InvestigationCases.FindByPredicate([InInvestigationSubject](const FDGInvestigationSuspectInfo& InCase){
		return InCase == InInvestigationSubject;
	});
}

void UDGEntriesManager::AddArgumentForCase(const TScriptInterface<IDGInvestigationSubject>& InSuspect, TSubclassOf<UObject> InArgumentFor, bool bShouldCreateCaseIfNotExists/* = true*/)
{
	if(InSuspect == nullptr)
	{
		return;
	}

	FDGInvestigationSuspectInfo* Case = const_cast<FDGInvestigationSuspectInfo*>(GetCaseForInvestigationSubject(InSuspect));
	if(Case != nullptr)
	{
		Case->ProsList.AddUnique(InArgumentFor);
	}
	else if(bShouldCreateCaseIfNotExists)
	{
		CreateNewInvestigationCase(InSuspect);

		AddArgumentForCase(InSuspect, InArgumentFor, false);
	}
}

void UDGEntriesManager::AddArgumentAgainstCase(const TScriptInterface<IDGInvestigationSubject>& InSuspect, TSubclassOf<UObject> InArgumentAgainst, bool bShouldCreateCaseIfNotExists/* = true*/)
{
	if (InSuspect == nullptr)
	{
		return;
	}

	FDGInvestigationSuspectInfo* Case = const_cast<FDGInvestigationSuspectInfo*>(GetCaseForInvestigationSubject(InSuspect));
	if (Case != nullptr)
	{
		Case->ConsList.AddUnique(InArgumentAgainst);
	}
	else if (bShouldCreateCaseIfNotExists)
	{
		CreateNewInvestigationCase(InSuspect);

		AddArgumentForCase(InSuspect, InArgumentAgainst, false);
	}
}

void UDGEntriesManager::RemoveArgumentForCase(const TScriptInterface<IDGInvestigationSubject>& InSuspect, TSubclassOf<UObject> InArgumentFor)
{
	if (InSuspect == nullptr)
	{
		return;
	}

	FDGInvestigationSuspectInfo* Case = const_cast<FDGInvestigationSuspectInfo*>(GetCaseForInvestigationSubject(InSuspect));
	if (Case == nullptr)
	{
		return;
	}

	Case->ProsList.Remove(InArgumentFor);
}

void UDGEntriesManager::RemoveArgumentAgainstCase(const TScriptInterface<IDGInvestigationSubject>& InSuspect, TSubclassOf<UObject> InArgumentAgainst)
{
	if (InSuspect == nullptr)
	{
		return;
	}

	FDGInvestigationSuspectInfo* Case = const_cast<FDGInvestigationSuspectInfo*>(GetCaseForInvestigationSubject(InSuspect));
	if (Case == nullptr)
	{
		return;
	}

	Case->ConsList.Remove(InArgumentAgainst);
}

void UDGEntriesManager::RemoveArgumentForCase(const TScriptInterface<IDGInvestigationSubject>& InSuspect, const int32 InArgumentIndex)
{
	if (InSuspect == nullptr)
	{
		return;
	}

	FDGInvestigationSuspectInfo* Case = const_cast<FDGInvestigationSuspectInfo*>(GetCaseForInvestigationSubject(InSuspect));
	if (Case == nullptr)
	{
		return;
	}

	Case->ProsList.RemoveAt(InArgumentIndex);
}

void UDGEntriesManager::RemoveArgumentAgainstCase(const TScriptInterface<IDGInvestigationSubject>& InSuspect, const int32 InArgumentIndex)
{
	if (InSuspect == nullptr)
	{
		return;
	}

	FDGInvestigationSuspectInfo* Case = const_cast<FDGInvestigationSuspectInfo*>(GetCaseForInvestigationSubject(InSuspect));
	if (Case == nullptr)
	{
		return;
	}

	Case->ConsList.RemoveAt(InArgumentIndex);
}

void UDGEntriesManager::OnNewEntriesDiscovered(TSubclassOf<UObject> InInvestigationEntry)
{
	UWorld* World = GetWorld();
	if(World == nullptr
		|| InInvestigationEntry == nullptr)
	{
		return;
	}

	APlayerController* PC = World->GetFirstPlayerController();
	if (PC == nullptr)
	{
		return;
	}

	ADGCharacter* Character = Cast<ADGCharacter>(PC->GetPawn());
	if(Character != nullptr)
	{
		const TScriptInterface<IDGInvestigationSubject> InvestigationSubject = InInvestigationEntry->GetDefaultObject();

		Character->OnNewEntriesDiscovered(this, InvestigationSubject);
	}
}
