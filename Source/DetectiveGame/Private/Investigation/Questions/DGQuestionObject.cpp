// Fill out your copyright notice in the Description page of Project Settings.


#include "DGQuestionObject.h"
#include "DetectiveGame.h"
#include "DGEvent.h"
#include "DGLocalizations.h"

EDGQuestionType UDGQuestionObject::GetQuestionType() const
{
    return QuestionType;
}

FText UDGQuestionObject::GetQuestionFullText() const
{
    return FullText;
}

FText UDGQuestionObject::GetQuestionPartOfSentenceText() const
{
    return PartOfSentenceText;
}