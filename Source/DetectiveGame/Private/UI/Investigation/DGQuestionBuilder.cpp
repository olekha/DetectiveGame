// Fill out your copyright notice in the Description page of Project Settings.


#include "DGQuestionBuilder.h"
#include "Game/Tools/ComboBoxObject.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/ScrollBox.h"
#include "Components/Button.h"
#include "DGEntriesManager.h"
#include "DGGameInstance.h"
#include "DGLocalizations.h"
#include "DGInvestigationSubject.h"
#include "DGQuestion.h"
#include "DetectiveGame.h"

#include "Blueprint/WidgetTree.h"

void UDGQuestionBuilder::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	QuestionComboBox->OnSelectionChanged.AddDynamic(this, &UDGQuestionBuilder::OnQuestionChanget);

	InvestigationSubjectComboBox->OnSelectionChanged.AddDynamic(this, &UDGQuestionBuilder::OnSubjectChanget);

	AddSuspectButton->OnClicked.AddDynamic(this, &UDGQuestionBuilder::OnAddSuspectButtonPressed);
	AddProsButton->OnClicked.AddDynamic(this, &UDGQuestionBuilder::OnAddProsButtonPressed);
	AddConsButton->OnClicked.AddDynamic(this, &UDGQuestionBuilder::OnAddConsButtonPressed);
}

void UDGQuestionBuilder::NativeConstruct()
{
	Super::NativeConstruct();

	FillQuestions();
}

void UDGQuestionBuilder::FillQuestions()
{
	QuestionComboBox->ClearOptions();

	UDGGameInstance* GameInstance = Cast<UDGGameInstance>(GetPlayerContext().GetGameInstance());
	if (GameInstance != nullptr)
	{
		UDGEntriesManager* EntriesManager = GameInstance->GetEntriesManager();
		if (EntriesManager != nullptr)
		{
			for (uint8 i = 0; i < static_cast<uint8>(EDGQuestionType::Max); ++i)
			{
				TScriptInterface<IDGQuestion> Question = EntriesManager->GetQuestion(static_cast<EDGQuestionType>(i));

				if(Question != nullptr)
				{
					QuestionComboBox->AddOption(Question->GetQuestionFullText().ToString(), Question.GetObject());
				}
			}
		}
	}

	QuestionComboBox->SetSelectedIndex(0);
}

void UDGQuestionBuilder::FillSubjects()
{
	InvestigationSubjectComboBox->ClearOptions();

	UDGGameInstance* GameInstance = Cast<UDGGameInstance>(GetPlayerContext().GetGameInstance());
	if (GameInstance != nullptr)
	{
		UDGEntriesManager* EntriesManager = GameInstance->GetEntriesManager();
		if (EntriesManager != nullptr)
		{
			const TScriptInterface<IDGQuestion> SelectedQuestion = GetCurrentSelectedQuestion();
			if(SelectedQuestion != nullptr)
			{
				const EDGQuestionType SelectedQuestionType = SelectedQuestion->GetQuestionType();

				EntriesManager->GetSubjectsForQuestion(SelectedQuestionType, InvestigationSubjects);

				for (int32 i = 0; i < InvestigationSubjects.Num(); ++i)
				{
					const TScriptInterface<IDGInvestigationSubject> InvestigationSubject = InvestigationSubjects[i];
					if (InvestigationSubject != nullptr)
					{
						const FText InName = InvestigationSubject->GetInvestigationSubjectName();

						InvestigationSubjectComboBox->AddOption(InName.ToString(), InvestigationSubject.GetObject());
					}
				}
			}
		}
	}

	InvestigationSubjectComboBox->SetSelectedIndex(0);
}

void UDGQuestionBuilder::FillSuspects()
{
	InvestigationSuspectsComboBox->ClearOptions();

	UDGGameInstance* GameInstance = Cast<UDGGameInstance>(GetPlayerContext().GetGameInstance());
	if (GameInstance != nullptr)
	{
		UDGEntriesManager* EntriesManager = GameInstance->GetEntriesManager();
		if (EntriesManager != nullptr)
		{
			const TScriptInterface<IDGQuestion> SelectedQuestion = GetCurrentSelectedQuestion();
			if (SelectedQuestion != nullptr)
			{
				const EDGQuestionType SelectedQuestionType = SelectedQuestion->GetQuestionType();

				EntriesManager->GetSuspectsForQuestion(SelectedQuestionType, InvestigationSuspects);

				for (int32 i = 0; i < InvestigationSuspects.Num(); ++i)
				{
					const TScriptInterface<IDGInvestigationSubject> InvestigationSuspect = InvestigationSuspects[i];
					if (InvestigationSuspect != nullptr)
					{
						const FText InName = InvestigationSuspect->GetInvestigationSubjectName();

						InvestigationSuspectsComboBox->AddOption(InName.ToString(), InvestigationSuspect.GetObject());
					}
				}
			}
		}
	}

	InvestigationSuspectsComboBox->SetSelectedIndex(0);
}

void UDGQuestionBuilder::OnQuestionChanget(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	FillSubjects();
	FillSuspects();

	BuildQuestion();

	UpdateSuspectsList();
}

void UDGQuestionBuilder::OnSubjectChanget(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	BuildQuestion();

	UpdateSuspectsList();
}

TScriptInterface<IDGQuestion> UDGQuestionBuilder::GetCurrentSelectedQuestion() const
{
	return TScriptInterface<IDGQuestion>(QuestionComboBox->GetSelectedOptionObject());
}

TScriptInterface<IDGInvestigationSubject> UDGQuestionBuilder::GetCurrentSelectedInvestigationSubject() const
{
	return TScriptInterface<IDGInvestigationSubject>(InvestigationSubjectComboBox->GetSelectedOptionObject());
}

TScriptInterface<IDGInvestigationSubject> UDGQuestionBuilder::GetCurrentSelectedInvestigationSuspect() const
{
	return TScriptInterface<IDGInvestigationSubject>(InvestigationSuspectsComboBox->GetSelectedOptionObject());
}

void UDGQuestionBuilder::BuildQuestion()
{
	const TScriptInterface<IDGQuestion> Question = GetCurrentSelectedQuestion();
	const TScriptInterface<IDGInvestigationSubject> Subject = GetCurrentSelectedInvestigationSubject();

	FText QuestionText = DGLocalizations::DefaultText;
	if(Question != nullptr)
	{
		QuestionText = Question->GetQuestionPartOfSentenceText();
	}

	FText SubjectText = DGLocalizations::DefaultText;
	if(Subject != nullptr)
	{
		SubjectText = Subject->GetInvestigationSubjectName();
	}

	QuestionTextBlock->SetText(FText::Format(NSLOCTEXT("QuestionBuilder", "BuiltQuestionTexts", "{0} {1}"), QuestionText, SubjectText));
}

void UDGQuestionBuilder::OnAddSuspectButtonPressed()
{
	FDGInvestigationQuestionInfo NewQuestion;

	const TScriptInterface<IDGQuestion> Question = GetCurrentSelectedQuestion();
	const TScriptInterface<IDGInvestigationSubject> Subject = GetCurrentSelectedInvestigationSubject();

	if(Question != nullptr)
	{
		NewQuestion.QuestionType = Question->GetQuestionType();
	}

	if(Subject != nullptr)
	{
		NewQuestion.QuestionSubject = TSubclassOf<UObject>(Subject.GetObject()->GetClass());
	}

	FDGInvestigationSuspectInfo NewSuspect;

	const TScriptInterface<IDGInvestigationSubject> Suspect = GetCurrentSelectedInvestigationSuspect();
	if(Suspect != nullptr)
	{
		NewSuspect.InvestigationSuspect = TSubclassOf<UObject>(Suspect.GetObject()->GetClass());
	}

	UDGGameInstance* GameInstance = Cast<UDGGameInstance>(GetPlayerContext().GetGameInstance());
	if (GameInstance != nullptr)
	{
		UDGEntriesManager* EntriesManager = GameInstance->GetEntriesManager();
		if (EntriesManager != nullptr)
		{
			EntriesManager->AddNewInvestigationCase(TPair<FDGInvestigationQuestionInfo, FDGInvestigationSuspectInfo>(NewQuestion, NewSuspect));
		}
	}

	UpdateSuspectsList();
}

void UDGQuestionBuilder::UpdateSuspectsList()
{
	SuspectsContainer->ClearChildren();

	const TScriptInterface<IDGQuestion> Question = GetCurrentSelectedQuestion();
	const TScriptInterface<IDGInvestigationSubject> Subject = GetCurrentSelectedInvestigationSubject();

	if(Question == nullptr || Subject == nullptr)
	{
		return;
	}

	UDGGameInstance* GameInstance = Cast<UDGGameInstance>(GetPlayerContext().GetGameInstance());
	if (GameInstance != nullptr)
	{
		UDGEntriesManager* EntriesManager = GameInstance->GetEntriesManager();
		if (EntriesManager != nullptr)
		{
			const TArray<FDGInvestigationSuspectInfo>* InvestigationList = EntriesManager->GetSuspectsList(FDGInvestigationQuestionInfo(Question, Subject));
			if(InvestigationList != nullptr)
			{
				for(int32 i = 0; i < InvestigationList->Num(); ++i)
				{
					const FDGInvestigationSuspectInfo& Suspect = (*InvestigationList)[i];

					if(Suspect.InvestigationSuspect == nullptr)
					{
						continue;
					}

					UTextBlock* TextBlock = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass());
					if(TextBlock != nullptr)
					{
						const TScriptInterface<IDGInvestigationSubject> SuspectObject = TScriptInterface<IDGInvestigationSubject>(Suspect.InvestigationSuspect->GetDefaultObject());
						if(SuspectObject != nullptr)
						{
							TextBlock->SetText(SuspectObject->GetInvestigationSubjectName());
						}

						SuspectsContainer->AddChild(TextBlock);
					}
				}
			}
		}
	}
}

void UDGQuestionBuilder::OnAddProsButtonPressed()
{

}

void UDGQuestionBuilder::OnAddConsButtonPressed()
{

}
