// Copyright Epic Games, Inc. All Rights Reserved.

#include "FamilyReunionDinner2GameMode.h"
#include "FamilyReunionDinner2HUD.h"
#include "FamilyReunionDinner2Character.h"
#include "UObject/ConstructorHelpers.h"

AFamilyReunionDinner2GameMode::AFamilyReunionDinner2GameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AFamilyReunionDinner2HUD::StaticClass();
}
