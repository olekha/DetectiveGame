// Fill out your copyright notice in the Description page of Project Settings.


#include "DGQuestionBuilderWidget.h"
#include "Game/Tools/ComboBoxObject.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/ScrollBox.h"
#include "Components/Button.h"
#include "Components/CheckBox.h"
#include "Blueprint/WidgetTree.h"
#include "Components/WidgetSwitcher.h"

#include "DetectiveGame.h"
#include "DGInvestigationSubjectWidget.h"
#include "DGGameInstance.h"
#include "DGEntriesManager.h"
#include "DGPerson.h"
#include "DGProsConsWidget.h"
#include "DGInvestigationSubject.h"

void UDGQuestionBuilderWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	PeopleCheckbox->OnCheckStateChanged.AddUniqueDynamic(this, &UDGQuestionBuilderWidget::OnOneFilterChanged);
	PlacesCheckbox->OnCheckStateChanged.AddUniqueDynamic(this, &UDGQuestionBuilderWidget::OnOneFilterChanged);
	ItemsCheckbox->OnCheckStateChanged.AddUniqueDynamic(this, &UDGQuestionBuilderWidget::OnOneFilterChanged);
	EventsCheckbox->OnCheckStateChanged.AddUniqueDynamic(this, &UDGQuestionBuilderWidget::OnOneFilterChanged);

	AddNewCaseButton->OnClicked.AddUniqueDynamic(this, &UDGQuestionBuilderWidget::OnAddNewCaseButtonClicked);

	AddProsButton->OnClicked.AddDynamic(this, &UDGQuestionBuilderWidget::OnAddProsButtonPressed);
	AddConsButton->OnClicked.AddDynamic(this, &UDGQuestionBuilderWidget::OnAddConsButtonPressed);

	KillerSuspectsComboBox->OnSelectionChanged.AddUniqueDynamic(this, &UDGQuestionBuilderWidget::OnKillerSelectionChanged);
	WeaponSuspectsComboBox->OnSelectionChanged.AddUniqueDynamic(this, &UDGQuestionBuilderWidget::OnWeaponSelectionChanged);
	PlaceSuspectsComboBox->OnSelectionChanged.AddUniqueDynamic(this, &UDGQuestionBuilderWidget::OnPlaceSelectionChanged);
	DateSuspectsComboBox->OnSelectionChanged.AddUniqueDynamic(this, &UDGQuestionBuilderWidget::OnDateSelectionChanged);
}

void UDGQuestionBuilderWidget::NativeConstruct()
{
	Super::NativeConstruct();

	OnOneFilterChanged(true);

	UpdateCaseInfo(nullptr);

	UpdateCurrentSelectedSuspects();
}

void UDGQuestionBuilderWidget::OnOneFilterChanged(bool bIsChecked/* = true*/)
{
	UpdateFilterMask();

	UpdateListOfInverstigationSubjects(FilterBitmask);
}

void UDGQuestionBuilderWidget::UpdateFilterMask()
{	
	FilterBitmask = 0;

	FilterBitmask ^= (-static_cast<uint8>(PeopleCheckbox->IsChecked()) ^ FilterBitmask) & (1UL << static_cast<uint8>(EDGFilterOption::People));
	FilterBitmask ^= (-static_cast<uint8>(PlacesCheckbox->IsChecked()) ^ FilterBitmask) & (1UL << static_cast<uint8>(EDGFilterOption::Places));
	FilterBitmask ^= (-static_cast<uint8>(EventsCheckbox->IsChecked()) ^ FilterBitmask) & (1UL << static_cast<uint8>(EDGFilterOption::Events));
	FilterBitmask ^= (-static_cast<uint8>(ItemsCheckbox->IsChecked()) ^ FilterBitmask) & (1UL << static_cast<uint8>(EDGFilterOption::Items));
}

void UDGQuestionBuilderWidget::UpdateListOfInverstigationSubjects(const uint8 InFilter/* = 0*/)
{
	EntriesScrollBox->ClearChildren();

	UDGEntriesManager* EntriesManager = UDGGameInstance::GetEntriesManager(this);
	if (EntriesManager == nullptr)
	{
		return;
	}

	if ((InFilter >> static_cast<uint8>(EDGFilterOption::People)) & 1UL)
	{
		AddToInvestigationSubjectsContainer(EntriesManager->GetDiscoveredPersons());
	}

	if ((InFilter >> static_cast<uint8>(EDGFilterOption::Places)) & 1UL)
	{
		AddToInvestigationSubjectsContainer(EntriesManager->GetDiscoveredPlaces());
	}

	if ((InFilter >> static_cast<uint8>(EDGFilterOption::Events)) & 1UL)
	{
		AddToInvestigationSubjectsContainer(EntriesManager->GetDiscoveredEvents());
	}

	if ((InFilter >> static_cast<uint8>(EDGFilterOption::Items)) & 1UL)
	{
		AddToInvestigationSubjectsContainer(EntriesManager->GetDiscoveredEvidence());
	}

	EntriesScrollBox->ScrollToStart();
}

void UDGQuestionBuilderWidget::AddToInvestigationSubjectsContainer(const TArray<TSubclassOf<UObject>>& InArrayOfSubjects)
{
	if(InvestigationSuspectButtonClass == nullptr)
	{
		return;
	}

	for(const TSubclassOf<UObject>& SubjectClass : InArrayOfSubjects)
	{
		if(SubjectClass == nullptr)
		{
			continue;
		}

		const TScriptInterface<IDGInvestigationSubject> Subject = TScriptInterface<IDGInvestigationSubject>(SubjectClass->GetDefaultObject());
		if (Subject != nullptr)
		{
			UDGInvestigationSubjectWidget* SubjectWidget = CreateWidget<UDGInvestigationSubjectWidget>(this, InvestigationSuspectButtonClass);

			SubjectWidget->InitWidget(Subject);
			EntriesScrollBox->AddChild(SubjectWidget);

			SubjectWidget->GetOnButtonHoveredDelegate().AddUniqueDynamic(this, &UDGQuestionBuilderWidget::OnInvestigationSubjectWidgetHovered);
		}
	}
}

void UDGQuestionBuilderWidget::OnInvestigationSubjectWidgetHovered()
{
	int32 CurrentHoveredSuspect = GetCurrentHoveredSubject();

	const TScriptInterface<IDGInvestigationSubject>& HoveredInvestigationSubject = GetInvestigationSubjectByIndex(CurrentHoveredSuspect);

	UpdateCaseInfo(HoveredInvestigationSubject);
}

void UDGQuestionBuilderWidget::UpdateCaseInfo(const TScriptInterface<IDGInvestigationSubject>& InInvestigationSubject)
{
	InvestigationSubjectInfoShownFor = InInvestigationSubject;

	if (InvestigationSubjectInfoShownFor == nullptr)
	{
		SetCasePanelVisibility(false);
		return;
	}

	if (!InvestigationSubjectInfoShownFor.GetObject()->Implements<UDGPerson>())
	{
		SetCasePanelVisibility(false);
		return;
	}

	UDGEntriesManager* EntriesManager = UDGGameInstance::GetEntriesManager(this);
	if (EntriesManager == nullptr)
	{
		SetCasePanelVisibility(false);
		return;
	}

	SetCasePanelVisibility(true);

	ClearCaseInfo();

	const FDGInvestigationSuspectInfo* InvestigationSubjectCase = EntriesManager->GetCaseForInvestigationSubject(InvestigationSubjectInfoShownFor);
	if (InvestigationSubjectCase == nullptr)
	{
		CaseInfoPanelWidgetSwitcher->SetActiveWidgetIndex(0);
		return;
	}

	CaseInfoPanelWidgetSwitcher->SetActiveWidgetIndex(1);

	if (ProsConsButtonClass != nullptr)
	{
		for (const TSubclassOf<UObject>& ProClass : InvestigationSubjectCase->ProsList)
		{
			if (ProClass == nullptr)
			{
				continue;
			}

			const TScriptInterface<IDGInvestigationSubject> Pro = TScriptInterface<IDGInvestigationSubject>(ProClass->GetDefaultObject());
			if (Pro != nullptr)
			{
				UDGProsConsWidget* ProWidget = CreateWidget<UDGProsConsWidget>(this, ProsConsButtonClass);

				ProWidget->InitWidget(Pro);
				ProsContainer->AddChild(ProWidget);

				ProWidget->GetOnRemoveButtonClickedDelegate().AddUniqueDynamic(this, &UDGQuestionBuilderWidget::OnRemoveProButtonClicked);
			}
		}

		for (const TSubclassOf<UObject>& ConClass : InvestigationSubjectCase->ConsList)
		{
			if (ConClass == nullptr)
			{
				continue;
			}

			const TScriptInterface<IDGInvestigationSubject> Con = TScriptInterface<IDGInvestigationSubject>(ConClass->GetDefaultObject());
			if (Con != nullptr)
			{
				UDGProsConsWidget* ConWidget = CreateWidget<UDGProsConsWidget>(this, ProsConsButtonClass);

				ConWidget->InitWidget(Con);
				ConsContainer->AddChild(ConWidget);

				ConWidget->GetOnRemoveButtonClickedDelegate().AddUniqueDynamic(this, &UDGQuestionBuilderWidget::OnRemoveConButtonClicked);
			}
		}
	}

	ProsAndConsComboBox->ClearOptions();

	for (const TSubclassOf<UObject>& SubjectClass : EntriesManager->GetDiscoveredEvents())
	{
		if (SubjectClass == nullptr)
		{
			continue;
		}

		if(EntriesManager->IsSubjectAddedToInvestigation(InvestigationSubjectCase->InvestigationSuspect, SubjectClass))
		{
			continue;
		}

		const TScriptInterface<IDGInvestigationSubject> Subject = TScriptInterface<IDGInvestigationSubject>(SubjectClass->GetDefaultObject());
		if (Subject != nullptr)
		{
			ProsAndConsComboBox->AddOption(Subject->GetInvestigationSubjectName().ToString(), Subject.GetObject());
		}
	}
}

int32 UDGQuestionBuilderWidget::GetCurrentHoveredSubject() const
{
	for (int32 i = 0; i < EntriesScrollBox->GetChildrenCount(); ++i)
	{
		UWidget* SuspectWidget = EntriesScrollBox->GetChildAt(i);
		if (SuspectWidget != nullptr && SuspectWidget->IsHovered())
		{
			return i;
		}
	}

	return INDEX_NONE;
}

TScriptInterface<IDGInvestigationSubject> UDGQuestionBuilderWidget::GetInvestigationSubjectByIndex(const int32 InIndex) const
{
	if(UDGInvestigationSubjectWidget* SubjectWidget =  Cast<UDGInvestigationSubjectWidget>(EntriesScrollBox->GetChildAt(InIndex)))
	{
		return SubjectWidget->GetBoundInvestigationSubject();
	}
	
	return nullptr;
}

void UDGQuestionBuilderWidget::ClearCaseInfo()
{
	ProsAndConsComboBox->ClearOptions();

	ProsContainer->ClearChildren();
	ConsContainer->ClearChildren();
}

void UDGQuestionBuilderWidget::OnAddNewCaseButtonClicked()
{
	if(InvestigationSubjectInfoShownFor == nullptr)
	{
		return;
	}

	UDGEntriesManager* EntriesManager = UDGGameInstance::GetEntriesManager(this);
	if (EntriesManager == nullptr)
	{
		return;
	}

	EntriesManager->CreateNewInvestigationCase(InvestigationSubjectInfoShownFor);

	UpdateCaseInfo(InvestigationSubjectInfoShownFor);
}

void UDGQuestionBuilderWidget::SetCasePanelVisibility(bool bInShouldBeVisible)
{
	CaseInfoPanelWidgetSwitcher->SetVisibility(bInShouldBeVisible ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}

void UDGQuestionBuilderWidget::OnRemoveProButtonClicked()
{
	if(InvestigationSubjectInfoShownFor == nullptr)
	{
		return;
	}
	
	UDGEntriesManager* EntriesManager = UDGGameInstance::GetEntriesManager(this);
	if (EntriesManager == nullptr)
	{
		return;
	}

	int32 CurrentHoveredPro = GetCurrentHoveredPro();
	if (CurrentHoveredPro == INDEX_NONE)
	{
		return;
	}

	EntriesManager->RemoveArgumentForCase(InvestigationSubjectInfoShownFor, CurrentHoveredPro);

	UpdateCaseInfo(InvestigationSubjectInfoShownFor);
}

void UDGQuestionBuilderWidget::OnRemoveConButtonClicked()
{
	if (InvestigationSubjectInfoShownFor == nullptr)
	{
		return;
	}

	UDGEntriesManager* EntriesManager = UDGGameInstance::GetEntriesManager(this);
	if (EntriesManager == nullptr)
	{
		return;
	}

	int32 CurrentHoveredCon = GetCurrentHoveredCon();
	if (CurrentHoveredCon == INDEX_NONE)
	{
		return;
	}

	EntriesManager->RemoveArgumentAgainstCase(InvestigationSubjectInfoShownFor, CurrentHoveredCon);

	UpdateCaseInfo(InvestigationSubjectInfoShownFor);
}

int32 UDGQuestionBuilderWidget::GetCurrentHoveredPro() const
{
	for (int32 i = 0; i < ProsContainer->GetChildrenCount(); ++i)
	{
		UWidget* ProWidget = ProsContainer->GetChildAt(i);
		if (ProWidget != nullptr && ProWidget->IsHovered())
		{
			return i;
		}
	}

	return INDEX_NONE;
}

int32 UDGQuestionBuilderWidget::GetCurrentHoveredCon() const
{
	for (int32 i = 0; i < ConsContainer->GetChildrenCount(); ++i)
	{
		UWidget* ConWidget = ConsContainer->GetChildAt(i);
		if (ConWidget != nullptr && ConWidget->IsHovered())
		{
			return i;
		}
	}

	return INDEX_NONE;
}

void UDGQuestionBuilderWidget::OnAddProsButtonPressed()
{
	if (InvestigationSubjectInfoShownFor == nullptr)
	{
		return;
	}

	UDGEntriesManager* EntriesManager = UDGGameInstance::GetEntriesManager(this);
	if (EntriesManager == nullptr)
	{
		return;
	}

	TScriptInterface<IDGInvestigationSubject> ArgumentForCase = GetInvestigationSubjectForProsAndCons();

	if(ArgumentForCase == nullptr)
	{
		return;
	}

	EntriesManager->AddArgumentForCase(InvestigationSubjectInfoShownFor, TSubclassOf<UObject>(ArgumentForCase.GetObject()->GetClass()));

	UpdateCaseInfo(InvestigationSubjectInfoShownFor);
}

void UDGQuestionBuilderWidget::OnAddConsButtonPressed()
{
	if (InvestigationSubjectInfoShownFor == nullptr)
	{
		return;
	}

	UDGEntriesManager* EntriesManager = UDGGameInstance::GetEntriesManager(this);
	if (EntriesManager == nullptr)
	{
		return;
	}

	TScriptInterface<IDGInvestigationSubject> ArgumentAgainstCase = GetInvestigationSubjectForProsAndCons();

	if (ArgumentAgainstCase == nullptr)
	{
		return;
	}

	EntriesManager->AddArgumentAgainstCase(InvestigationSubjectInfoShownFor, TSubclassOf<UObject>(ArgumentAgainstCase.GetObject()->GetClass()));

	UpdateCaseInfo(InvestigationSubjectInfoShownFor);
}

TScriptInterface<IDGInvestigationSubject> UDGQuestionBuilderWidget::GetInvestigationSubjectForProsAndCons() const
{
	return TScriptInterface<IDGInvestigationSubject>(ProsAndConsComboBox->GetSelectedOptionObject());
}

void UDGQuestionBuilderWidget::UpdateCurrentSelectedSuspects()
{
	UDGEntriesManager* EntriesManager = UDGGameInstance::GetEntriesManager(this);
	if(EntriesManager == nullptr)
	{
		return;
	}

	KillerSuspectsComboBox->ClearOptions();
	for (const TSubclassOf<UObject>& SubjectClass : EntriesManager->GetDiscoveredPersons())
	{
		if (SubjectClass == nullptr)
		{
			continue;
		}

		const TScriptInterface<IDGInvestigationSubject> Subject = TScriptInterface<IDGInvestigationSubject>(SubjectClass->GetDefaultObject());
		if (Subject != nullptr)
		{
			KillerSuspectsComboBox->AddOption(Subject->GetInvestigationSubjectName().ToString(), Subject.GetObject());
		}
	}

	PlaceSuspectsComboBox->ClearOptions();
	for (const TSubclassOf<UObject>& SubjectClass : EntriesManager->GetDiscoveredPlaces())
	{
		if (SubjectClass == nullptr)
		{
			continue;
		}

		const TScriptInterface<IDGInvestigationSubject> Subject = TScriptInterface<IDGInvestigationSubject>(SubjectClass->GetDefaultObject());
		if (Subject != nullptr)
		{
			PlaceSuspectsComboBox->AddOption(Subject->GetInvestigationSubjectName().ToString(), Subject.GetObject());
		}
	}

	WeaponSuspectsComboBox->ClearOptions();
	for (const TSubclassOf<UObject>& SubjectClass : EntriesManager->GetDiscoveredEvidence())
	{
		if (SubjectClass == nullptr)
		{
			continue;
		}

		const TScriptInterface<IDGInvestigationSubject> Subject = TScriptInterface<IDGInvestigationSubject>(SubjectClass->GetDefaultObject());
		if (Subject != nullptr)
		{
			WeaponSuspectsComboBox->AddOption(Subject->GetInvestigationSubjectName().ToString(), Subject.GetObject());
		}
	}

	DateSuspectsComboBox->ClearOptions();

	OnKillerSelectionChanged("", ESelectInfo::Direct);
	OnWeaponSelectionChanged("", ESelectInfo::Direct);
	OnPlaceSelectionChanged("", ESelectInfo::Direct);
	OnDateSelectionChanged("", ESelectInfo::Direct);
}

void UDGQuestionBuilderWidget::OnKillerSelectionChanged(FString InSelectedItem, ESelectInfo::Type InSelectionType)
{
	FText SelectedKillerName = FText::FromString("-");
	if(TScriptInterface<IDGInvestigationSubject> SelectedKiller = GetSelectedKiller())
	{
		SelectedKillerName = SelectedKiller->GetInvestigationSubjectName();
	}

	KillerNameTextBlock->SetText(SelectedKillerName);
}

void UDGQuestionBuilderWidget::OnWeaponSelectionChanged(FString InSelectedItem, ESelectInfo::Type InSelectionType)
{
	FText SelectedWeaponName = FText::FromString("-");
	if (TScriptInterface<IDGInvestigationSubject> SelectedWeapon = GetSelectedWeapon())
	{
		SelectedWeaponName = SelectedWeapon->GetInvestigationSubjectName();
	}

	WeaponNameTextBlock->SetText(SelectedWeaponName);
}

void UDGQuestionBuilderWidget::OnPlaceSelectionChanged(FString InSelectedItem, ESelectInfo::Type InSelectionType)
{
	FText SelectedPlaceName = FText::FromString("-");
	if (TScriptInterface<IDGInvestigationSubject> SelectedPlace = GetSelectedPlace())
	{
		SelectedPlaceName = SelectedPlace->GetInvestigationSubjectName();
	}

	PlaceNameTextBlock->SetText(SelectedPlaceName);
}

void UDGQuestionBuilderWidget::OnDateSelectionChanged(FString InSelectedItem, ESelectInfo::Type InSelectionType)
{
	DateAndTimeTextBlock->SetText(FText::FromString("-"));
}

TScriptInterface<IDGInvestigationSubject> UDGQuestionBuilderWidget::GetSelectedKiller() const
{
	return TScriptInterface<IDGInvestigationSubject>(KillerSuspectsComboBox->GetSelectedOptionObject());
}

TScriptInterface<IDGInvestigationSubject> UDGQuestionBuilderWidget::GetSelectedWeapon() const
{
	return TScriptInterface<IDGInvestigationSubject>(WeaponSuspectsComboBox->GetSelectedOptionObject());
}

TScriptInterface<IDGInvestigationSubject> UDGQuestionBuilderWidget::GetSelectedPlace() const
{
	return TScriptInterface<IDGInvestigationSubject>(PlaceSuspectsComboBox->GetSelectedOptionObject());
}
