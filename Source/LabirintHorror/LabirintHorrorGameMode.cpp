// Copyright Epic Games, Inc. All Rights Reserved.

#include "LabirintHorrorGameMode.h"
#include "LabirintHorrorHUD.h"
#include "LabirintHorrorCharacter.h"
#include "UObject/ConstructorHelpers.h"

ALabirintHorrorGameMode::ALabirintHorrorGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = ALabirintHorrorHUD::StaticClass();
}
