// Fill out your copyright notice in the Description page of Project Settings.


#include "DGProsConsWidget.h"

FOnButtonHoverEvent& UDGProsConsWidget::GetOnRemoveButtonHoveredDelegate()
{
	return RemoveProsConsButton->OnHovered;
}

FOnButtonHoverEvent& UDGProsConsWidget::GetOnRemoveButtonUnHoveredDelegate()
{
	return RemoveProsConsButton->OnUnhovered;
}

FOnButtonClickedEvent& UDGProsConsWidget::GetOnRemoveButtonClickedDelegate()
{
	return RemoveProsConsButton->OnClicked;
}
