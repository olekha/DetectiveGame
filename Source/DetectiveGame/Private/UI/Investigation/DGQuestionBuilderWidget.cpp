// Fill out your copyright notice in the Description page of Project Settings.


#include "DGQuestionBuilderWidget.h"
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
#include "DGInvestigationSuspectWidget.h"

#include "Blueprint/WidgetTree.h"

void UDGQuestionBuilderWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	QuestionComboBox->OnSelectionChanged.AddDynamic(this, &UDGQuestionBuilderWidget::OnQuestionChanget);

	InvestigationSubjectComboBox->OnSelectionChanged.AddDynamic(this, &UDGQuestionBuilderWidget::OnSubjectChanget);

	AddSuspectButton->OnClicked.AddDynamic(this, &UDGQuestionBuilderWidget::OnAddSuspectButtonPressed);
	AddProsButton->OnClicked.AddDynamic(this, &UDGQuestionBuilderWidget::OnAddProsButtonPressed);
	AddConsButton->OnClicked.AddDynamic(this, &UDGQuestionBuilderWidget::OnAddConsButtonPressed);
}

void UDGQuestionBuilderWidget::NativeConstruct()
{
	Super::NativeConstruct();

	FillQuestions();
}

void UDGQuestionBuilderWidget::FillQuestions()
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

void UDGQuestionBuilderWidget::FillSubjects()
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

void UDGQuestionBuilderWidget::FillSuspects()
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

void UDGQuestionBuilderWidget::OnQuestionChanget(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	FillSubjects();
	FillSuspects();

	BuildQuestion();

	UpdateSuspectsList();
}

void UDGQuestionBuilderWidget::OnSubjectChanget(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	BuildQuestion();

	UpdateSuspectsList();
}

TScriptInterface<IDGQuestion> UDGQuestionBuilderWidget::GetCurrentSelectedQuestion() const
{
	return TScriptInterface<IDGQuestion>(QuestionComboBox->GetSelectedOptionObject());
}

TScriptInterface<IDGInvestigationSubject> UDGQuestionBuilderWidget::GetCurrentSelectedInvestigationSubject() const
{
	return TScriptInterface<IDGInvestigationSubject>(InvestigationSubjectComboBox->GetSelectedOptionObject());
}

TScriptInterface<IDGInvestigationSubject> UDGQuestionBuilderWidget::GetCurrentSelectedInvestigationSuspect() const
{
	return TScriptInterface<IDGInvestigationSubject>(InvestigationSuspectsComboBox->GetSelectedOptionObject());
}

void UDGQuestionBuilderWidget::BuildQuestion()
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

void UDGQuestionBuilderWidget::OnAddSuspectButtonPressed()
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

void UDGQuestionBuilderWidget::UpdateSuspectsList()
{
	if(InvestigationSuspectButtonClass == nullptr)
	{
		return;
	}

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

					UDGInvestigationSuspectWidget* SuspectButton = CreateWidget<UDGInvestigationSuspectWidget>(this, InvestigationSuspectButtonClass);
					
					const TScriptInterface<IDGInvestigationSubject> SuspectObject = TScriptInterface<IDGInvestigationSubject>(Suspect.InvestigationSuspect->GetDefaultObject());

					SuspectButton->InitWidget(SuspectObject);

					SuspectButton->GetOnButtonClickedDelegate().AddUniqueDynamic(this, &UDGQuestionBuilderWidget::OnSuspectButtonClicked);

					SuspectsContainer->AddChild(SuspectButton);
				}
			}
		}
	}
}

void UDGQuestionBuilderWidget::OnSuspectButtonClicked()
{
	//@okharchenko rework

	int32 CurrentHoveredSuspect = GetCurrentHoveredSuspect();

	if(CurrentHoveredSuspect == INDEX_NONE)
	{
		return;
	}

	const TScriptInterface<IDGQuestion> Question = GetCurrentSelectedQuestion();
	const TScriptInterface<IDGInvestigationSubject> Subject = GetCurrentSelectedInvestigationSubject();

	if (Question == nullptr || Subject == nullptr)
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
			if (InvestigationList != nullptr)
			{
				const FDGInvestigationSuspectInfo& Suspect = (*InvestigationList)[CurrentHoveredSuspect];

				for (const TSubclassOf<UObject>& Pro : Suspect.ProsList)
				{
					const TScriptInterface<IDGInvestigationSubject> ProObject = TScriptInterface<IDGInvestigationSubject>(Pro->GetDefaultObject());
					if (ProObject != nullptr)
					{


					}
				}

				for (const TSubclassOf<UObject>& Con : Suspect.ConsList)
				{
					const TScriptInterface<IDGInvestigationSubject> ConObject = TScriptInterface<IDGInvestigationSubject>(Con->GetDefaultObject());
					if (ConObject != nullptr)
					{


					}
				}
			}
		}
	}
}

int32 UDGQuestionBuilderWidget::GetCurrentHoveredSuspect() const
{
	for (int32 i = 0; i < SuspectsContainer->GetChildrenCount(); ++i)
	{
		UDGInvestigationSuspectWidget* SuspectWidget = Cast<UDGInvestigationSuspectWidget>(SuspectsContainer->GetChildAt(i));
		if(SuspectWidget->IsHovered())
		{
			return i;
		}
	}

	return INDEX_NONE;
}

void UDGQuestionBuilderWidget::OnAddProsButtonPressed()
{

}

void UDGQuestionBuilderWidget::OnAddConsButtonPressed()
{

}
