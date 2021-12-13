// Copyright Epic Games, Inc. All Rights Reserved.

#include "FamilyReunionDinner2GameMode.h"
#include "MyGameStateBase.h"
#include "MyPlayerState.h"
#include "FamilyReunionDinner2HUD.h"
#include "FamilyReunionDinner2Character.h"
#include "Kismet/GameplayStatics.h"
#include "MySaveGame.h"
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
		Cast<AMyPlayerState>(GameState->PlayerArray[0])->hintShowed = false;
		Cast<AMyPlayerState>(GameState->PlayerArray[0])->setTurn(true);
	}
	
	UMySaveGame* saveData = Cast<UMySaveGame>(UGameplayStatics::LoadGameFromSlot("SaveData0", 0));

	for (int i = 0; i < saveData->pids.Num(); i += 1)
	{
		if (saveData->pids[i] == FPlatformProcess::GetCurrentProcessId())
		{
			if (saveData->playerCount[i] == currentPlayerCount)
			{
				FTimerHandle timerHandle;
				GetWorldTimerManager().SetTimer(timerHandle, this, &AFamilyReunionDinner2GameMode::initGameCountDown, 1, false);
			}

			break;
		}
	}
}

void AFamilyReunionDinner2GameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
}

void AFamilyReunionDinner2GameMode::initGameCountDown() 
{
	Cast<AMyGameStateBase>(GameState)->initGame();
}