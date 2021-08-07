// Fill out your copyright notice in the Description page of Project Settings.
#include "DGPersonObject.h"

FText UDGPersonObject::GetFullName() const
{
	return PersonFullName;
}

 FText UDGPersonObject::GetFullDescription() const
 {
	 return PersonFullDescription;
 }

 FText UDGPersonObject::GetDateOfBirth() const
 {
	return PersonDateOfBirth;
 }

 const TArray<EDGQuestionType>& UDGPersonObject::GetQuestionsCanBeSuspectFor() const
 {
	return ListOfQuestionsCanBeSuspectFor;
 }

 const TArray<EDGQuestionType>& UDGPersonObject::GetQuestionsCanBeSubjectFor() const
 {
	 return ListOfQuestionsCanBeSubjectFor;
 }

 FText UDGPersonObject::GetInvestigationSubjectName() const
 {
	return GetFullName();
 }
