// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMenuPlayerState.h"
#include "Net/UnrealNetwork.h"
#include "Misc/OutputDeviceNull.h"
#include "GameMenuGameMode.h"
#include "GameMenuLevelScriptActor.h"

void AGameMenuPlayerState::BeginPlay() 
{
	Super::BeginPlay();

	if (GetLocalRole() != 3 && GetOwner() != NULL)
	{
		this->generateFamilyReunionDinner2UserID();
		this->requestServerInfo();
	}
}

void AGameMenuPlayerState::generateFamilyReunionDinner2UserID_Implementation()
{
	FamilyReunionDinner2PlayerID = FMath::RandRange(10000000, 99999999);
	setFamilyReunionDinner2UserID(FamilyReunionDinner2PlayerID);
}

void AGameMenuPlayerState::setFamilyReunionDinner2UserID_Implementation(int id)
{
	FamilyReunionDinner2PlayerID = id;
	call_createMainMenu();
}

void AGameMenuPlayerState::call_createMainMenu() 
{
	FOutputDeviceNull ar;

	if (GetLevel() != NULL)
	{
		AGameMenuLevelScriptActor* levelBluePrint = Cast<AGameMenuLevelScriptActor>(GetLevel()->GetLevelScriptActor());
		levelBluePrint->CallFunctionByNameWithArguments(TEXT("initMainMenu"), ar, NULL, true);
	}	
}

void AGameMenuPlayerState::updateServerInfoUpdate_Implementation(const TArray<FServerListInfoStruct>& data)
{
	for (int i = 0; i < data.Num(); i += 1) 
	{
		handleServerCreateEvent_Implementation(data[i]);
	}
}

void AGameMenuPlayerState::requestServerInfo_Implementation() 
{
	TArray<FServerListInfoStruct> data;

	for (int i = 0; i < Cast<AGameMenuGameMode>(GetWorld()->GetAuthGameMode())->serverInfo.Num(); i++)
	{
		data.Add(Cast<AGameMenuGameMode>(GetWorld()->GetAuthGameMode())->serverInfo[i].serverInfo);
	}

	sendServerInfo(data);
}

void AGameMenuPlayerState::sendServerInfo_Implementation(const TArray<FServerListInfoStruct>& data)
{
	for (int i = 0; i < data.Num(); i += 1)
	{
		handleServerCreateEvent_Implementation(data[i]);
	}
}

void AGameMenuPlayerState::updateRoomInfoUpdate_Implementation(const TArray<FRoomPlayerInfoStruct>& data)
{
	for (int i = 0; i < data.Num(); i += 1)
	{
		handlePlayerJoinRoomEvent_Implementation(data[i]);
	}
}

void AGameMenuPlayerState::handlePlayerJoinRoomEvent_Implementation(FRoomPlayerInfoStruct data)
{
	Cast<AGameMenuCharacter>(GetPawn())->GameMenuUI->addRoomPlayerUI(data.ID, data.name);
}

void AGameMenuPlayerState::handlePlayerLeaveRoomEvent_Implementation(int id)
{
	Cast<AGameMenuCharacter>(GetPawn())->GameMenuUI->removeRoomPlayerUI(id);
}

void AGameMenuPlayerState::handleRoomPlayerChangedEvent_Implementation(int curNum, int maxNum)
{
	Cast<AGameMenuCharacter>(GetPawn())->GameMenuUI->roomPlayerChangedUI(curNum, maxNum);
}

void AGameMenuPlayerState::handleServerListRoomPlayerChangedEvent_Implementation(int id, int curNum, int maxNum)
{
	Cast<AGameMenuCharacter>(GetPawn())->GameMenuUI->serverListRoomPlayerChangedUI(id, curNum, maxNum);
}

void AGameMenuPlayerState::handleServerCreateEvent_Implementation(FServerListInfoStruct data)
{
	Cast<AGameMenuCharacter>(GetPawn())->GameMenuUI->addServerListUI(data.ID, data.name, data.curPlayerNum, data.maxPlayerNum);
}

void AGameMenuPlayerState::handleServerDeleteEvent_Implementation(int id)
{
	Cast<AGameMenuCharacter>(GetPawn())->GameMenuUI->removeServerListUI(id);
}
