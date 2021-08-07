// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/DGGameMode.h"
#include "DGCharacter.h"

ADGGameMode::ADGGameMode()
{
	DefaultPawnClass = ADGCharacter::StaticClass();
}