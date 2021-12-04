// Fill out your copyright notice in the Description page of Project Settings.


#include "DGEntriesManager.h"
#include "DGInvestigationSubject.h"
#include "DGEvent.h"
#include "DGPerson.h"
#include "DGPlace.h"
#include "DGQuestion.h"
#include "DGGameInstance.h"
#include "DGCharacter.h"
#include "GameFramework/PlayerController.h"

bool UDGEntriesManager::IsQuestionInfoValid(const FDGInvestigationQuestionInfo& InQuestionInfo)
{
	return InQuestionInfo.QuestionType != EDGQuestionType::None
			&& InQuestionInfo.QuestionType != EDGQuestionType::Max
			&& InQuestionInfo.QuestionSubject != nullptr;
}

bool UDGEntriesManager::IsSuspectInfoValid(const FDGInvestigationSuspectInfo& InQuestionInfo)
{
	return InQuestionInfo.InvestigationSuspect != nullptr;
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
	}

	return false;
}

void UDGEntriesManager::GetSuspectsForQuestion(EDGQuestionType InQuestion, TArray<TScriptInterface<IDGInvestigationSubject>>& OutEntries) const
{
	OutEntries.Reset();

	for(int32 i = 0; i < DiscoveredEvents.Num(); ++i)
	{
		TSubclassOf<UObject> DiscoveredEvent = DiscoveredEvents[i];

		const TScriptInterface<IDGInvestigationSubject> Event = TScriptInterface<IDGInvestigationSubject>(DiscoveredEvent->GetDefaultObject());
		if (Event != nullptr)
		{
			const TArray<EDGQuestionType>& QuestionsArray = Event->GetQuestionsCanBeSuspectFor();

			const bool bContainsQuestion = QuestionsArray.Contains(InQuestion);

			if(bContainsQuestion)
			{
				OutEntries.AddUnique(Event);
			}			
		}
	}

	for (int32 i = 0; i < DiscoveredPersons.Num(); ++i)
	{
		TSubclassOf<UObject> DiscoveredPerson = DiscoveredPersons[i];

		const TScriptInterface<IDGInvestigationSubject> Persone = DiscoveredPerson->GetDefaultObject();
		if (Persone != nullptr)
		{
			const TArray<EDGQuestionType>& QuestionsArray = Persone->GetQuestionsCanBeSuspectFor();

			const bool bContainsQuestion = QuestionsArray.Contains(InQuestion);

			if(bContainsQuestion)
			{
				OutEntries.AddUnique(Persone);
			}
		}
	}

	for (int32 i = 0; i < DiscoveredPlaces.Num(); ++i)
	{
		TSubclassOf<UObject> DiscoveredPlace = DiscoveredPlaces[i];

		const TScriptInterface<IDGInvestigationSubject> Place = DiscoveredPlace->GetDefaultObject();
		if (Place != nullptr)
		{
			const TArray<EDGQuestionType>& QuestionsArray = Place->GetQuestionsCanBeSuspectFor();

			const bool bContainsQuestion = QuestionsArray.Contains(InQuestion);

			if(bContainsQuestion)
			{
				OutEntries.AddUnique(Place);
			}		
		}
	}
}

void UDGEntriesManager::GetSubjectsForQuestion(EDGQuestionType InQuestion, TArray<TScriptInterface<IDGInvestigationSubject>>& OutEntries) const
{
	OutEntries.Reset();

	for (int32 i = 0; i < DiscoveredEvents.Num(); ++i)
	{
		TSubclassOf<UObject> DiscoveredEvent = DiscoveredEvents[i];

		const TScriptInterface<IDGInvestigationSubject> Event = TScriptInterface<IDGInvestigationSubject>(DiscoveredEvent->GetDefaultObject());
		if (Event != nullptr)
		{
			const TArray<EDGQuestionType>& QuestionsArray = Event->GetQuestionsCanBeSubjectFor();

			const bool bContainsQuestion = QuestionsArray.Contains(InQuestion);

			if (bContainsQuestion)
			{
				OutEntries.AddUnique(Event);
			}
		}
	}

	for (int32 i = 0; i < DiscoveredPersons.Num(); ++i)
	{
		TSubclassOf<UObject> DiscoveredPerson = DiscoveredPersons[i];

		const TScriptInterface<IDGInvestigationSubject> Persone = DiscoveredPerson->GetDefaultObject();
		if (Persone != nullptr)
		{
			const TArray<EDGQuestionType>& QuestionsArray = Persone->GetQuestionsCanBeSubjectFor();

			const bool bContainsQuestion = QuestionsArray.Contains(InQuestion);

			if (bContainsQuestion)
			{
				OutEntries.AddUnique(Persone);
			}
		}
	}

	for (int32 i = 0; i < DiscoveredPlaces.Num(); ++i)
	{
		TSubclassOf<UObject> DiscoveredPlace = DiscoveredPlaces[i];

		const TScriptInterface<IDGInvestigationSubject> Place = DiscoveredPlace->GetDefaultObject();
		if (Place != nullptr)
		{
			const TArray<EDGQuestionType>& QuestionsArray = Place->GetQuestionsCanBeSubjectFor();

			const bool bContainsQuestion = QuestionsArray.Contains(InQuestion);

			if (bContainsQuestion)
			{
				OutEntries.AddUnique(Place);
			}
		}
	}
}

TScriptInterface<IDGQuestion> UDGEntriesManager::GetQuestion(EDGQuestionType InQuestionType) const
{
	const TSubclassOf<UObject>* Question = Questions.FindByPredicate([InQuestionType](TSubclassOf<UObject> InQuestionClass) {

		if (InQuestionClass == nullptr)
		{
			return false;
		}

		const TScriptInterface<IDGQuestion> Place = InQuestionClass->GetDefaultObject();
		if (Place == nullptr)
		{
			return false;
		}

		return Place->GetQuestionType() == InQuestionType;
		});

	if (Question == nullptr)
	{
		return nullptr;
	}

	return TScriptInterface<IDGQuestion>(Question->GetDefaultObject());
}

void UDGEntriesManager::AddNewInvestigationCase(TPair<FDGInvestigationQuestionInfo, FDGInvestigationSuspectInfo> InNewInvestigationCase)
{
	if(UDGEntriesManager::IsQuestionInfoValid(InNewInvestigationCase.Key)
		&& UDGEntriesManager::IsSuspectInfoValid(InNewInvestigationCase.Value))
	{
		FDGSuspectContainer& Container = InvestigationCases.FindOrAdd(InNewInvestigationCase.Key);

		Container.Container.AddUnique(InNewInvestigationCase.Value);
	}
}

const TArray<FDGInvestigationSuspectInfo>* UDGEntriesManager::GetSuspectsList(const FDGInvestigationQuestionInfo& InQuestion)
{
	FDGSuspectContainer* Container = InvestigationCases.Find(InQuestion);

	if(Container != nullptr)
	{
		return &(Container->Container);
	}

	return nullptr;
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
