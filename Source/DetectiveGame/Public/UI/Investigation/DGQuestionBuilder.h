// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DGQuestionBuilder.generated.h"

class UComboBoxObject;
class UTextBlock;
class UVerticalBox;
class IDGInvestigationSubject;
class UButton;
class IDGQuestion;
class UScrollBox;
class UComboBoxString;

UCLASS()
class DETECTIVEGAME_API UDGQuestionBuilder : public UUserWidget
{
	GENERATED_BODY()
	
protected:

	virtual void NativeOnInitialized() override;

	virtual void NativeConstruct() override;

private:

	void FillQuestions();
	UFUNCTION()
	void OnQuestionChanget(FString SelectedItem, ESelectInfo::Type SelectionType);
	UFUNCTION()
	void OnSubjectChanget(FString SelectedItem, ESelectInfo::Type SelectionType);

	void FillSubjects();
	void FillSuspects();

	TScriptInterface<IDGQuestion> GetCurrentSelectedQuestion() const;
	TScriptInterface<IDGInvestigationSubject> GetCurrentSelectedInvestigationSubject() const;
	TScriptInterface<IDGInvestigationSubject> GetCurrentSelectedInvestigationSuspect() const;

	void BuildQuestion();

	UFUNCTION()
	void OnAddSuspectButtonPressed();
	UFUNCTION()
	void OnAddProsButtonPressed();
	UFUNCTION()
	void OnAddConsButtonPressed();

	void UpdateSuspectsList();
protected:

	UPROPERTY(meta = (BindWidget))
	UComboBoxObject* QuestionComboBox;

	UPROPERTY(meta = (BindWidget))
	UComboBoxObject* InvestigationSubjectComboBox;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* QuestionTextBlock;

	UPROPERTY(meta = (BindWidget))
	UScrollBox* SuspectsContainer;

	UPROPERTY(meta = (BindWidget))
	UComboBoxObject* InvestigationSuspectsComboBox;

	UPROPERTY(meta = (BindWidget))
	UVerticalBox* ProsContainer;

	UPROPERTY(meta = (BindWidget))
	UComboBoxString* ProsComboBox;

	UPROPERTY(meta = (BindWidget))
	UVerticalBox* ConsContainer;

	UPROPERTY(meta = (BindWidget))
	UComboBoxString* ConsComboBox;

	UPROPERTY(meta = (BindWidget))
	UButton* AddSuspectButton;

	UPROPERTY(meta = (BindWidget))
	UButton* AddProsButton;

	UPROPERTY(meta = (BindWidget))
	UButton* AddConsButton;

private:

	UPROPERTY(Transient)
	TArray<TScriptInterface<IDGInvestigationSubject>> InvestigationSubjects;

	UPROPERTY(Transient)
	TArray<TScriptInterface<IDGInvestigationSubject>> InvestigationSuspects;
};
