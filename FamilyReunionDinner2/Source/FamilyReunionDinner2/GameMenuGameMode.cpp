// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMenuGameMode.h"
#include "GameMenuGameStateBase.h"
#include "GameMenuPlayerState.h"
#include "GameMenuCharacter.h"

AGameMenuGameMode::AGameMenuGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/MyGameMenuCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;
	GameStateClass = AGameMenuGameStateBase::StaticClass();
	PlayerStateClass = AGameMenuPlayerState::StaticClass();
}

void AGameMenuGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
}

void AGameMenuGameMode::openServer(int index)
{
	FString execPath = FPaths::ProjectDir() + TEXT("Binaries/Win64/FamilyReunionDinner2Server.exe");
	FString args = TEXT("/Game/FirstPersonCPP/Maps/kitchen -log");

	UE_LOG(LogTemp, Warning, TEXT("Current server path = %s"), *FPaths::ConvertRelativePathToFull(execPath));

	FProcHandle serverHandle = FPlatformProcess::CreateProc(*FPaths::ConvertRelativePathToFull(execPath), *(args + FString::Printf(TEXT(" -ClientProcID=%u"), FPlatformProcess::GetCurrentProcessId())), true, true, false, NULL, 0, NULL, NULL);

	if (serverHandle.IsValid()) 
	{
		UE_LOG(LogTemp, Warning, TEXT("Server created"));
		serverHandles.Add(serverHandle);
	
		for (int i = 0; i < serverInfo[index].currentPlayers.Num(); i += 1) 
		{
			serverInfo[index].currentPlayers[i]->enterSession(7777 + serverHandles.Num());
		}
		
		for (int i = 0; i < GameState->PlayerArray.Num(); i++)
		{
			Cast<AGameMenuPlayerState>(GameState->PlayerArray[i])->handleServerDeleteEvent(serverInfo[index].serverInfo.ID);
		}

		serverInfo.RemoveAt(index);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Server created failed"));
	}
}