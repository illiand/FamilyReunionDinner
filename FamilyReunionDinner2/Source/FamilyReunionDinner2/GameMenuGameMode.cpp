// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMenuGameMode.h"
#include "GameMenuGameStateBase.h"
#include "GameMenuPlayerState.h"
#include "GameMenuCharacter.h"
#include "MySaveGame.h"
#include "Kismet/GameplayStatics.h"

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
	args += " ";
	args += "?MaxPlayer=";
	args += FString::FromInt(serverInfo[index].currentPlayers.Num());

	uint32* pid = new uint32(0);

	FProcHandle serverHandle = FPlatformProcess::CreateProc(*FPaths::ConvertRelativePathToFull(execPath), *(args + FString::Printf(TEXT(" -ClientProcID=%u"), FPlatformProcess::GetCurrentProcessId())), true, true, false, pid, 0, NULL, NULL);
	
	if (serverHandle.IsValid()) 
	{
		int targetPort = 0;

		//check avi room
		for (int i = 0; i < serverHandles.Num(); i += 1)
		{
			if (!FPlatformProcess::IsProcRunning(serverHandles[i]))
			{
				targetPort = serverPorts[i];

				serverHandles.RemoveAt(i);
				serverHandles.Insert(serverHandle, i);

				break;
			}
		}

		if (targetPort == 0)
		{
			serverHandles.Add(serverHandle);

			targetPort = 7777 + serverHandles.Num();
			serverPorts.Add(targetPort);
		}

		UMySaveGame* saveData = Cast<UMySaveGame>(UGameplayStatics::CreateSaveGameObject(UMySaveGame::StaticClass()));
		saveData->pids.Add(*pid);
		saveData->playerCount.Add(serverInfo[index].currentPlayers.Num());
		UGameplayStatics::SaveGameToSlot(saveData, "SaveData0", 0);
	
		UMySaveGame* saveData2 = Cast<UMySaveGame>(UGameplayStatics::LoadGameFromSlot("SaveData0", 0));

		for (int i = 0; i < serverInfo[index].currentPlayers.Num(); i += 1) 
		{
			Cast<AGameMenuPlayerState>(serverInfo[index].currentPlayers[i]->GetPlayerState())->setTransformingStatus(true);
			serverInfo[index].currentPlayers[i]->enterSession(targetPort);
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
