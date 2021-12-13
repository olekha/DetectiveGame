// Fill out your copyright notice in the Description page of Project Settings.

#include "DGEventObject.h"

FText UDGEventObject::GetEventName() const
{
	return EventName;
}

FText UDGEventObject::GetInvestigationSubjectName() const
{
	return GetEventName();
}

FText UDGEventObject::GetEventDescription() const
{
	return EventDescription;
}