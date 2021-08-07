#pragma once
#include "CoreMinimal.h"

enum class EDGQuestionType : uint8;

namespace DGLocalizations
{
	extern const FText& DefaultText;

	extern const TMap<EDGQuestionType, const FText> QuestionsText;
	const FText& GetQuestionText(EDGQuestionType InQuestionType);
}