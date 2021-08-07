// Fill out your copyright notice in the Description page of Project Settings.


#include "DGEntriesManager.h"
#include "DGInvestigationSubject.h"
#include "DGEvent.h"
#include "DGPerson.h"
#include "DGPlace.h"
#include "DGQuestion.h"
#include "DGGameInstance.h"

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

void UDGEntriesManager::DiscoverNewEntry(TSubclassOf<UObject> InvestigationEntry)
{
	if (InvestigationEntry == nullptr
		|| !InvestigationEntry->ImplementsInterface(UDGInvestigationSubject::StaticClass())
		|| IsEntryDiscoveredAlready(InvestigationEntry))
	{
		return;
	}

	if(InvestigationEntry->ImplementsInterface(UDGEvent::StaticClass()))
	{
		DiscoveredEvents.AddUnique(InvestigationEntry);
	}

	if (InvestigationEntry->ImplementsInterface(UDGPerson::StaticClass()))
	{
		DiscoveredPersons.AddUnique(InvestigationEntry);
	}

	if (InvestigationEntry->ImplementsInterface(UDGPlace::StaticClass()))
	{
		DiscoveredPlaces.AddUnique(InvestigationEntry);
	}
}

bool UDGEntriesManager::IsEntryDiscoveredAlready(TSubclassOf<UObject> InvestigationEntry) const
{
	if (InvestigationEntry != nullptr)
	{
		if (DiscoveredEvents.Contains(InvestigationEntry))
		{
			return true;
		}

		if (DiscoveredPersons.Contains(InvestigationEntry))
		{
			return true;
		}	
		
		if (DiscoveredPlaces.Contains(InvestigationEntry))
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
