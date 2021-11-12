// Copyright Epic Games, Inc. All Rights Reserved.

#include "FamilyReunionDinner2GameMode.h"
#include "MyGameStateBase.h"
#include "MyPlayerState.h"
#include "FamilyReunionDinner2HUD.h"
#include "FamilyReunionDinner2Character.h"
#include "UObject/ConstructorHelpers.h"

AFamilyReunionDinner2GameMode::AFamilyReunionDinner2GameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;
	GameStateClass = AMyGameStateBase::StaticClass();
	PlayerStateClass = AMyPlayerState::StaticClass();
	// use our custom HUD class
	HUDClass = AFamilyReunionDinner2HUD::StaticClass();
}

void AFamilyReunionDinner2GameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	int currentPlayerCount = GameState->PlayerArray.Num();
	Cast<AFamilyReunionDinner2Character>(NewPlayer->GetPawn())->setLocationByIndex(currentPlayerCount);

	if (currentPlayerCount == 1) 
	{
		Cast<AMyPlayerState>(GameState->PlayerArray[0])->inTurn = true;
	}
}

void AFamilyReunionDinner2GameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
}