// Fill out your copyright notice in the Description page of Project Settings.


#include "DGInvestigationSuspectWidget.h"
#include "Components/TextBlock.h"
#include "DGInvestigationSubject.h"

void UDGInvestigationSuspectWidget::InitWidget(const TScriptInterface<IDGInvestigationSubject> InSuspectObjectc)
{
	if(InSuspectObjectc == nullptr)
	{
		return;
	}

	SuspectNameTextBlock->SetText(InSuspectObjectc->GetInvestigationSubjectName());
}

FOnButtonHoverEvent& UDGInvestigationSuspectWidget::GetOnButtonHoveredDelegate()
{
	return SuspectButton->OnHovered;
}

FOnButtonHoverEvent& UDGInvestigationSuspectWidget::GetOnButtonUnHoveredDelegate()
{
	return SuspectButton->OnUnhovered;
}

FOnButtonClickedEvent& UDGInvestigationSuspectWidget::GetOnButtonClickedDelegate()
{
	return SuspectButton->OnClicked;
}
