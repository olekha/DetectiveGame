// Copyright Epic Games, Inc. All Rights Reserved.

#include "DetectiveGame.h"
#include "Modules/ModuleManager.h"

#include "DGQuestion.h"

IMPLEMENT_PRIMARY_GAME_MODULE( FDefaultGameModuleImpl, DetectiveGame, "DetectiveGame" );

FDGInvestigationQuestionInfo::FDGInvestigationQuestionInfo(const TScriptInterface<IDGQuestion> InQuestion, const TScriptInterface<IDGInvestigationSubject> InQuestionSubject)
{
	QuestionType = InQuestion->GetQuestionType();
	QuestionSubject = TSubclassOf<UObject>(InQuestionSubject.GetObject()->GetClass());
}
