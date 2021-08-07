#include "DGLocalizations.h"
#include "DetectiveGame.h"

#define LOCTEXT_NAMESPACE "DetectiveGameLocalizations"

const FText& DGLocalizations::DefaultText = LOCTEXT("DefaultText", "Default");

const TMap<EDGQuestionType, const FText> DGLocalizations::QuestionsText =
{
	{ EDGQuestionType::What,		LOCTEXT("WhatText",			"What happendd?"		)},
	{ EDGQuestionType::When,		LOCTEXT("WhenText",			"When happened?"		)},
	{ EDGQuestionType::Where,		LOCTEXT("WhereText",		"Where happened?"		)},
	{ EDGQuestionType::How,			LOCTEXT("HowText",			"How happened?"			)},
	{ EDGQuestionType::Why,			LOCTEXT("WhyText",			"Why happened?"			)},
	{ EDGQuestionType::Who,			LOCTEXT("WhoText",			"Who did?"				)}
};

const FText& DGLocalizations::GetQuestionText(EDGQuestionType InQuestionType)
{
	if(QuestionsText.Contains(InQuestionType))
	{
		return QuestionsText[InQuestionType];
	}

	return DefaultText;
};

#undef LOCTEXT_NAMESPACE 
