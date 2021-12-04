// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "DGInvestigationSuspectWidget.generated.h"

class UTextBlock;
class IDGInvestigationSubject;

UCLASS()
class DETECTIVEGAME_API UDGInvestigationSuspectWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	void InitWidget(const TScriptInterface<IDGInvestigationSubject> InSuspectObjectc);

	FOnButtonHoverEvent& GetOnButtonHoveredDelegate();

	FOnButtonHoverEvent& GetOnButtonUnHoveredDelegate();

	FOnButtonClickedEvent& GetOnButtonClickedDelegate();

protected:

	UPROPERTY(meta = (BindWidget))
	UTextBlock* SuspectNameTextBlock;

	UPROPERTY(meta = (BindWidget))
	UButton* SuspectButton;	
};
