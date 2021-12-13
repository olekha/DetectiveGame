// Fill out your copyright notice in the Description page of Project Settings.

#include "DGPlaceObject.h"

FText UDGPlaceObject::GetPlaceName() const
{
	return PlaceName;
}

FText UDGPlaceObject::GetInvestigationSubjectName() const
{
	return GetPlaceName();
}