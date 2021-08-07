// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ComboBoxString.h"
#include "ComboBoxObject.generated.h"

/**
 * 
 */
UCLASS()
class DETECTIVEGAME_API UComboBoxObject : public UComboBoxString
{
	GENERATED_BODY()
	
public:

	void AddOption(const FString& OptionString) = delete;

	void AddOption(const FString& OptionString, UObject* OptionObject);
	
	bool RemoveOption(const FString& Option);

	UFUNCTION(BlueprintCallable, Category = "ComboBox")
	int32 FindOptionIndexByObject(UObject* OptionObject) const;

	UFUNCTION(BlueprintCallable, Category = "ComboBox")
	UObject* GetOptionObjectAtIndex(int32 Index) const;

	void ClearOptions();

	UObject* GetSelectedOptionObject() const;

private:

	UPROPERTY(Transient)
	TArray<UObject*> OptionsObjects;
};
