// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMenuCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "GameMenuPlayerState.h"
#include "GameMenuGameMode.h"
#include "APIClass.h"


// Sets default values
AGameMenuCharacter::AGameMenuCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGameMenuCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGameMenuCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AGameMenuCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AGameMenuCharacter::createGame_Implementation(const FString& serverName, int maxPlayerNum)
{
	FServerInfoStruct newServerListInfo;

	FServerListInfoStruct serverInfo;
	TArray<AGameMenuCharacter*> currentPlayers;
	serverInfo.ID = FMath::RandRange(10000, 99999);
	serverInfo.name = serverName;
	serverInfo.curPlayerNum = 1;
	serverInfo.maxPlayerNum = maxPlayerNum;
	currentPlayers.Add(Cast<AGameMenuCharacter>(this));
	
	newServerListInfo.serverInfo = serverInfo;
	newServerListInfo.currentPlayers = currentPlayers;

	Cast<AGameMenuPlayerState>(GetPlayerState())->playerRoomID = serverInfo.ID;

	Cast<AGameMenuGameMode>(GetWorld()->GetAuthGameMode())->serverInfo.Add(newServerListInfo);
	
	for (int i = 0; i < GetWorld()->GetGameState()->PlayerArray.Num(); i += 1)
	{
		Cast<AGameMenuPlayerState>(GetWorld()->GetGameState()->PlayerArray[i])->handleServerCreateEvent(serverInfo);
	}

	FRoomPlayerInfoStruct curRoomInfo;
	curRoomInfo.ID = Cast<AGameMenuPlayerState>(GetPlayerState())->FamilyReunionDinner2PlayerID;

	Cast<AGameMenuPlayerState>(GetPlayerState())->handlePlayerJoinRoomEvent(curRoomInfo);

	joinRoomEnd(serverInfo.ID, serverName, 1, maxPlayerNum);
}

void AGameMenuCharacter::startGame_Implementation(int roomID) 
{
	Cast<AGameMenuGameMode>(GetWorld()->GetAuthGameMode())->openServer(findRoomIndexByID(roomID));
}

void AGameMenuCharacter::enterSession_Implementation(int portNum) 
{
	FString target = UAPIClass::getIPAddress().Append(TEXT(":")).Append(FString::FromInt(portNum));
	UGameplayStatics::OpenLevel(GetWorld(), FName(*target));
}

int AGameMenuCharacter::findRoomIndexByID(int roomID) 
{
	TArray<FServerInfoStruct> serverInfo = Cast<AGameMenuGameMode>(GetWorld()->GetAuthGameMode())->serverInfo;

	for (int i = 0; i < serverInfo.Num(); i += 1)
	{
		if (serverInfo[i].serverInfo.ID == roomID) 
		{
			return i;
		}
	}

	return -1;
}

void AGameMenuCharacter::joinRoomRequest_Implementation(int roomID) 
{
	Cast<AGameMenuPlayerState>(GetPlayerState())->playerRoomID = roomID;
	int roomIndex = findRoomIndexByID(roomID);

	TArray<FRoomPlayerInfoStruct> curInfo;

	//add existing players for this new joining player to the room
	for (int i = 0; i < Cast<AGameMenuGameMode>(GetWorld()->GetAuthGameMode())->serverInfo[roomIndex].currentPlayers.Num(); i++)
	{
		FRoomPlayerInfoStruct curPlayer;
		curPlayer.ID = Cast<AGameMenuPlayerState>(Cast<AGameMenuGameMode>(GetWorld()->GetAuthGameMode())->serverInfo[roomIndex].currentPlayers[i]->GetPlayerState())->FamilyReunionDinner2PlayerID;

		curInfo.Add(curPlayer);
	}

	Cast<AGameMenuPlayerState>(GetPlayerState())->updateRoomInfoUpdate(curInfo);

	//add this new player to the room
	//add this player ui to all players in the room
	Cast<AGameMenuGameMode>(GetWorld()->GetAuthGameMode())->serverInfo[roomIndex].currentPlayers.Add(this);
	int curPlayerNum = Cast<AGameMenuGameMode>(GetWorld()->GetAuthGameMode())->serverInfo[roomIndex].currentPlayers.Num();
	int maxPlayerNum = Cast<AGameMenuGameMode>(GetWorld()->GetAuthGameMode())->serverInfo[roomIndex].serverInfo.maxPlayerNum;

	for (int i = 0; i < Cast<AGameMenuGameMode>(GetWorld()->GetAuthGameMode())->serverInfo[roomIndex].currentPlayers.Num(); i += 1)
	{
		FRoomPlayerInfoStruct newPlayer;
		newPlayer.ID = Cast<AGameMenuPlayerState>(GetPlayerState())->FamilyReunionDinner2PlayerID;
		Cast<AGameMenuPlayerState>(Cast<AGameMenuGameMode>(GetWorld()->GetAuthGameMode())->serverInfo[roomIndex].currentPlayers[i]->GetPlayerState())->handlePlayerJoinRoomEvent(newPlayer);
		Cast<AGameMenuPlayerState>(Cast<AGameMenuGameMode>(GetWorld()->GetAuthGameMode())->serverInfo[roomIndex].currentPlayers[i]->GetPlayerState())->handleRoomPlayerChangedEvent(curPlayerNum, maxPlayerNum);
	}

	for (int i = 0; i < GetWorld()->GetGameState()->PlayerArray.Num(); i += 1)
	{
		Cast<AGameMenuPlayerState>(GetWorld()->GetGameState()->PlayerArray[i])->handleServerListRoomPlayerChangedEvent(roomID, curPlayerNum, maxPlayerNum);
	}

	joinRoomEnd(roomID, Cast<AGameMenuGameMode>(GetWorld()->GetAuthGameMode())->serverInfo[roomIndex].serverInfo.name, curPlayerNum, maxPlayerNum);
}

void AGameMenuCharacter::joinRoomEnd_Implementation(int id, const FString& name, int curNum, int maxNum)
{
	GameMenuUI->createRoomCallback(id, name, curNum, maxNum);
}

void AGameMenuCharacter::leaveRoomRequest_Implementation()
{	
	int roomID = Cast<AGameMenuPlayerState>(GetPlayerState())->playerRoomID;
	int roomIndex = findRoomIndexByID(roomID);
	
	Cast<AGameMenuGameMode>(GetWorld()->GetAuthGameMode())->serverInfo[roomIndex].currentPlayers.Remove(this);
	int curPlayerNum = Cast<AGameMenuGameMode>(GetWorld()->GetAuthGameMode())->serverInfo[roomIndex].currentPlayers.Num();
	int maxPlayerNum = Cast<AGameMenuGameMode>(GetWorld()->GetAuthGameMode())->serverInfo[roomIndex].serverInfo.maxPlayerNum;

	//existing players
	for (int i = 0; i < Cast<AGameMenuGameMode>(GetWorld()->GetAuthGameMode())->serverInfo[roomIndex].currentPlayers.Num(); i += 1)
	{
		Cast<AGameMenuPlayerState>(Cast<AGameMenuGameMode>(GetWorld()->GetAuthGameMode())->serverInfo[roomIndex].currentPlayers[i]->GetPlayerState())->handlePlayerLeaveRoomEvent(Cast<AGameMenuPlayerState>(GetPlayerState())->FamilyReunionDinner2PlayerID);
		Cast<AGameMenuPlayerState>(Cast<AGameMenuGameMode>(GetWorld()->GetAuthGameMode())->serverInfo[roomIndex].currentPlayers[i]->GetPlayerState())->handleRoomPlayerChangedEvent(curPlayerNum, maxPlayerNum);
	}

	//delete the server if 0
	if (Cast<AGameMenuGameMode>(GetWorld()->GetAuthGameMode())->serverInfo[roomIndex].currentPlayers.Num() == 0) 
	{
		Cast<AGameMenuGameMode>(GetWorld()->GetAuthGameMode())->serverInfo.RemoveAt(roomIndex);

		for (int i = 0; i < GetWorld()->GetGameState()->PlayerArray.Num(); i += 1) 
		{
			Cast<AGameMenuPlayerState>(GetWorld()->GetGameState()->PlayerArray[i])->handleServerDeleteEvent(roomID);
		}
	}
	else 
	{
		//else update the UI of current players for all users
		for (int i = 0; i < GetWorld()->GetGameState()->PlayerArray.Num(); i += 1)
		{
			Cast<AGameMenuPlayerState>(GetWorld()->GetGameState()->PlayerArray[i])->handleServerListRoomPlayerChangedEvent(roomID, curPlayerNum, maxPlayerNum);
		}
	}

	//show the serverlist UI
	playerLeaveRoom();
}

void AGameMenuCharacter::playerLeaveRoom_Implementation() 
{
	GameMenuUI->LeaveRoomCallback();
}