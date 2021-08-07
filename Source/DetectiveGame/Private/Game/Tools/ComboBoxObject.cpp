// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Tools/ComboBoxObject.h"

void UComboBoxObject::AddOption(const FString& OptionString, UObject* OptionObject)
{
	Super::AddOption(OptionString);

	OptionsObjects.Add(OptionObject);
}

bool UComboBoxObject::RemoveOption(const FString& Option)
{
	int32 OptionIndex = FindOptionIndex(Option);

	if(OptionsObjects.IsValidIndex(OptionIndex))
	{
		OptionsObjects.RemoveAt(OptionIndex);
	}

	return Super::RemoveOption(Option);
}

int32 UComboBoxObject::FindOptionIndexByObject(UObject* OptionObject) const
{
	for (int32 OptionIndex = 0; OptionIndex < OptionsObjects.Num(); OptionIndex++)
	{
		const UObject* const OptionAtIndex = OptionsObjects[OptionIndex];

		if (OptionAtIndex == OptionObject)
		{
			return OptionIndex;
		}
	}

	return -1;
}

UObject* UComboBoxObject::GetOptionObjectAtIndex(int32 Index) const
{
	if(OptionsObjects.IsValidIndex(Index))
	{
		return OptionsObjects[Index];
	}

	return nullptr;
}

void UComboBoxObject::ClearOptions()
{
	OptionsObjects.Empty();

	Super::ClearOptions();
}

UObject* UComboBoxObject::GetSelectedOptionObject() const
{
	if (CurrentOptionPtr.IsValid())
	{
		for (int32 OptionIndex = 0; OptionIndex < Options.Num(); ++OptionIndex)
		{
			if (Options[OptionIndex] == CurrentOptionPtr)
			{
				if(OptionsObjects.IsValidIndex(OptionIndex))
				{
					return OptionsObjects[OptionIndex];
				}
			}
		}
	}

	return nullptr;
}
