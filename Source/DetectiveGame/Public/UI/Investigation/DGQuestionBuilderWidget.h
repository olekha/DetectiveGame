// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DGQuestionBuilderWidget.generated.h"

class UComboBoxObject;
class UCheckBox;
class UScrollBox;
class UButton;
class UVerticalBox;
class IDGInvestigationSubject;
class UDGProsConsWidget;
class UDGInvestigationSubjectWidget;
class UWidgetSwitcher;
class UTextBlock;

UCLASS()
class UDGQuestionBuilderWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:

	virtual void NativeOnInitialized() override;

	virtual void NativeConstruct() override;

private:

	/*bIsChecked parameter is here only because it is used in OnCheckStateChanged in UCheckBox*/
	UFUNCTION()
	void OnOneFilterChanged(bool bIsChecked = true);

	void UpdateFilterMask();
	void UpdateListOfInverstigationSubjects(const uint8 InFilter = 0);
	void AddToInvestigationSubjectsContainer(const TArray<TSubclassOf<UObject>>& InArrayOfSubjects);

	UFUNCTION()
	void OnInvestigationSubjectWidgetHovered();

	int32 GetCurrentHoveredSubject() const;

	TScriptInterface<IDGInvestigationSubject> GetInvestigationSubjectByIndex(const int32 InIndex) const;

	UFUNCTION()
	void OnAddNewCaseButtonClicked();

	void ClearCaseInfo();
	void UpdateCaseInfo(const TScriptInterface<IDGInvestigationSubject>& InInvestigationSubject);
	void SetCasePanelVisibility(bool bInShouldBeVisible);

	UFUNCTION()
	void OnRemoveProButtonClicked();
	UFUNCTION()
	void OnRemoveConButtonClicked();

	int32 GetCurrentHoveredPro() const;
	int32 GetCurrentHoveredCon() const;

	UFUNCTION()
	void OnAddProsButtonPressed();
	UFUNCTION()
	void OnAddConsButtonPressed();

	TScriptInterface<IDGInvestigationSubject> GetInvestigationSubjectForProsAndCons() const;

	void UpdateCurrentSelectedSuspects();

	UFUNCTION()
	void OnKillerSelectionChanged(FString InSelectedItem, ESelectInfo::Type InSelectionType);
	UFUNCTION()
	void OnWeaponSelectionChanged(FString InSelectedItem, ESelectInfo::Type InSelectionType);
	UFUNCTION()
	void OnPlaceSelectionChanged(FString InSelectedItem, ESelectInfo::Type InSelectionType);
	UFUNCTION()
	void OnDateSelectionChanged(FString InSelectedItem, ESelectInfo::Type InSelectionType);

	TScriptInterface<IDGInvestigationSubject> GetSelectedKiller() const;
	TScriptInterface<IDGInvestigationSubject> GetSelectedWeapon() const;
	TScriptInterface<IDGInvestigationSubject> GetSelectedPlace() const;

protected:

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UDGInvestigationSubjectWidget> InvestigationSuspectButtonClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UDGProsConsWidget> ProsConsButtonClass;

	UPROPERTY(meta = (BindWidget), BlueprintReadOnly)
	UCheckBox* PeopleCheckbox;

	UPROPERTY(meta = (BindWidget), BlueprintReadOnly)
	UCheckBox* PlacesCheckbox;

	UPROPERTY(meta = (BindWidget), BlueprintReadOnly)
	UCheckBox* ItemsCheckbox;

	UPROPERTY(meta = (BindWidget), BlueprintReadOnly)
	UCheckBox* EventsCheckbox;

	UPROPERTY(meta = (BindWidget), BlueprintReadOnly)
	UScrollBox* EntriesScrollBox;

	UPROPERTY(meta = (BindWidget), BlueprintReadOnly)
	UWidgetSwitcher* CaseInfoPanelWidgetSwitcher;

	UPROPERTY(meta = (BindWidget), BlueprintReadOnly)
	UButton* AddNewCaseButton;

	UPROPERTY(meta = (BindWidget), BlueprintReadOnly)
	UComboBoxObject* ProsAndConsComboBox;

	UPROPERTY(meta = (BindWidget), BlueprintReadOnly)
	UButton* AddProsButton;

	UPROPERTY(meta = (BindWidget), BlueprintReadOnly)
	UButton* AddConsButton;

	UPROPERTY(meta = (BindWidget), BlueprintReadOnly)
	UVerticalBox* ProsContainer;

	UPROPERTY(meta = (BindWidget), BlueprintReadOnly)	
	UVerticalBox* ConsContainer;

	UPROPERTY(meta = (BindWidget), BlueprintReadOnly)
	UTextBlock* KillerNameTextBlock;

	UPROPERTY(meta = (BindWidget), BlueprintReadOnly)
	UTextBlock* WeaponNameTextBlock;
	
	UPROPERTY(meta = (BindWidget), BlueprintReadOnly)
	UTextBlock* PlaceNameTextBlock;

	UPROPERTY(meta = (BindWidget), BlueprintReadOnly)
	UTextBlock* DateAndTimeTextBlock;

	UPROPERTY(meta = (BindWidget), BlueprintReadOnly)
	UComboBoxObject* KillerSuspectsComboBox;

	UPROPERTY(meta = (BindWidget), BlueprintReadOnly)
	UComboBoxObject* WeaponSuspectsComboBox;

	UPROPERTY(meta = (BindWidget), BlueprintReadOnly)
	UComboBoxObject* PlaceSuspectsComboBox;

	UPROPERTY(meta = (BindWidget), BlueprintReadOnly)
	UComboBoxObject* DateSuspectsComboBox;

private:

	UPROPERTY(Transient)
	TScriptInterface<IDGInvestigationSubject> InvestigationSubjectInfoShownFor;

	UPROPERTY(Transient)
	TArray<TScriptInterface<IDGInvestigationSubject>> InvestigationSubjects;

	UPROPERTY(Transient)
	TArray<TScriptInterface<IDGInvestigationSubject>> InvestigationSuspects;

	uint8 FilterBitmask = 0;
};
