// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "ServerListInfoStruct.h"
#include "RoomPlayerInfoStruct.h"
#include "GameMenuPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class FAMILYREUNIONDINNER2_API AGameMenuPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	virtual void BeginPlay();
	
public:
	UPROPERTY(BlueprintReadWrite)
	int playerRoomID = -1;

	UPROPERTY(BlueprintReadOnly)
	int FamilyReunionDinner2PlayerID;

	UFUNCTION()
	void call_createMainMenu();

	UFUNCTION(Server, Reliable)
	void requestServerInfo();
	void requestServerInfo_Implementation();

	UFUNCTION(Client, Reliable)
	void sendServerInfo(const TArray<FServerListInfoStruct>& data);
	void sendServerInfo_Implementation(const TArray<FServerListInfoStruct>& data);

	UFUNCTION(Server, Reliable)
	void generateFamilyReunionDinner2UserID();
	void generateFamilyReunionDinner2UserID_Implementation();

	UFUNCTION(Client, Reliable)
	void setFamilyReunionDinner2UserID(int id);
	void setFamilyReunionDinner2UserID_Implementation(int id);

	UFUNCTION(Client, Reliable)
	void updateServerInfoUpdate(const TArray<FServerListInfoStruct>& data);
	void updateServerInfoUpdate_Implementation(const TArray<FServerListInfoStruct>& data);

	UFUNCTION(Client, Reliable)
	void updateRoomInfoUpdate(const TArray<FRoomPlayerInfoStruct>& data);
	void updateRoomInfoUpdate_Implementation(const TArray<FRoomPlayerInfoStruct>& data);

	UFUNCTION(Client, Reliable)
	void handlePlayerJoinRoomEvent(FRoomPlayerInfoStruct data);
	void handlePlayerJoinRoomEvent_Implementation(FRoomPlayerInfoStruct data);

	//set the ui for players panal
	UFUNCTION(Client, Reliable)
	void handlePlayerLeaveRoomEvent(int id);
	void handlePlayerLeaveRoomEvent_Implementation(int id);

	UFUNCTION(Client, Reliable)
	void handleServerCreateEvent(FServerListInfoStruct data);
	void handleServerCreateEvent_Implementation(FServerListInfoStruct data);

	UFUNCTION(Client, Reliable)
	void handleServerDeleteEvent(int id);
	void handleServerDeleteEvent_Implementation(int id);

	//set the ui for (cur/max) number of players
	UFUNCTION(Client, Reliable)
	void handleRoomPlayerChangedEvent(int curNum, int maxNum);
	void handleRoomPlayerChangedEvent_Implementation(int curNum, int maxNum);

	UFUNCTION(Client, Reliable)
	void handleServerListRoomPlayerChangedEvent(int id, int curNum, int maxNum);
	void handleServerListRoomPlayerChangedEvent_Implementation(int id, int curNum, int maxNum);
};
