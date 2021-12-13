// Fill out your copyright notice in the Description page of Project Settings.
#include "DGPersonObject.h"

FText UDGPersonObject::GetPersonName() const
{
	return PersonFullName;
}

 FText UDGPersonObject::GetPersonDescription() const
 {
	 return PersonFullDescription;
 }

 FText UDGPersonObject::GetDateOfBirth() const
 {
	return PersonDateOfBirth;
 }

 FText UDGPersonObject::GetInvestigationSubjectName() const
 {
	return GetPersonName();
 }