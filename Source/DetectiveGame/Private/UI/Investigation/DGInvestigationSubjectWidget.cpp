// Fill out your copyright notice in the Description page of Project Settings.


#include "DGInvestigationSubjectWidget.h"
#include "Components/TextBlock.h"
#include "DGInvestigationSubject.h"

void UDGInvestigationSubjectWidget::InitWidget(const TScriptInterface<IDGInvestigationSubject> InSuspectObject)
{
	if(InSuspectObject == nullptr)
	{
		return;
	}

	SuspectNameTextBlock->SetText(InSuspectObject->GetInvestigationSubjectName());

	BoundInvestigationSubject = InSuspectObject;
}

void UDGInvestigationSubjectWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	bIsHovered = true;

	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);

	OnHovered.Broadcast();
}

void UDGInvestigationSubjectWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	bIsHovered = false;

	Super::NativeOnMouseLeave(InMouseEvent);

	OnUnhovered.Broadcast();
}
