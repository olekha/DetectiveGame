// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "DGInvestigationSubjectWidget.generated.h"

class UTextBlock;
class IDGInvestigationSubject;

UCLASS()
class UDGInvestigationSubjectWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void InitWidget(const TScriptInterface<IDGInvestigationSubject> InSuspectObject);

	FORCEINLINE const TScriptInterface<IDGInvestigationSubject>& GetBoundInvestigationSubject() const
	{
		return BoundInvestigationSubject;
	}

	FORCEINLINE FOnButtonHoverEvent& GetOnButtonHoveredDelegate()
	{
		return OnHovered;
	}

	FORCEINLINE FOnButtonHoverEvent& GetOnButtonUnHoveredDelegate()
	{
		return OnUnhovered;
	}

	FORCEINLINE FOnButtonClickedEvent& GetOnButtonClickedDelegate()
	{
		return SuspectButton->OnClicked;
	}

	FORCEINLINE virtual bool IsHovered() const override
	{
		return bIsHovered;
	}

protected:

	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;

protected:

	UPROPERTY(meta = (BindWidget), BlueprintReadOnly)
	UTextBlock* SuspectNameTextBlock;

	UPROPERTY(meta = (BindWidget), BlueprintReadOnly)
	UButton* SuspectButton;

private:

	FOnButtonHoverEvent OnHovered;

	FOnButtonHoverEvent OnUnhovered;

	TScriptInterface<IDGInvestigationSubject> BoundInvestigationSubject;

	bool bIsHovered = false;
};
