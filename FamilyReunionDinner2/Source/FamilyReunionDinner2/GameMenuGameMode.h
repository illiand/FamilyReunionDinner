// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ServerInfoStruct.h"
#include "GameMenuGameMode.generated.h"

/**
 * 
 */
UCLASS()
class FAMILYREUNIONDINNER2_API AGameMenuGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	AGameMenuGameMode();

public:
	virtual void PostLogin(APlayerController* NewPlayer);

public:
	TArray<FServerInfoStruct> serverInfo;

	TArray<FProcHandle> serverHandles;
	TArray<int> serverPorts;
	void openServer(int index);
};
