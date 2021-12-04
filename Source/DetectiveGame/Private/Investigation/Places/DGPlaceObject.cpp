// Fill out your copyright notice in the Description page of Project Settings.

#include "DGPlaceObject.h"

FText UDGPlaceObject::GetPlaceName() const
{
	return PlaceName;
}

const TArray<EDGQuestionType>& UDGPlaceObject::GetQuestionsCanBeSuspectFor() const
{
	return ListOfQuestionsCanBeSuspectFor;
}

const TArray<EDGQuestionType>& UDGPlaceObject::GetQuestionsCanBeSubjectFor() const
{
	return ListOfQuestionsCanBeSubjectFor;
}

FText UDGPlaceObject::GetInvestigationSubjectName() const
{
	return GetPlaceName();
}

FText UDGPlaceObject::GetInvestigationSubjectNameAsPartOfQuestion(EDGQuestionType InQuestionType) const
{
	return PlaceNameAsPartOfQuestion;
}