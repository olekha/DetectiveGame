// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DGInvestigationSubjectWidget.h"
#include "DGProsConsWidget.generated.h"

class UTextBlock;
class IDGInvestigationSubject;

UCLASS()
class UDGProsConsWidget : public UDGInvestigationSubjectWidget
{
	GENERATED_BODY()

public:

	FOnButtonHoverEvent& GetOnRemoveButtonHoveredDelegate();

	FOnButtonHoverEvent& GetOnRemoveButtonUnHoveredDelegate();

	FOnButtonClickedEvent& GetOnRemoveButtonClickedDelegate();

protected:

	UPROPERTY(meta = (BindWidget), BlueprintReadOnly)
	UButton* RemoveProsConsButton;	
};
