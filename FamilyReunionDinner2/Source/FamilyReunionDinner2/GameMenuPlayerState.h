// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "ServerInfoStruct.h"
#include "GameMenuPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class FAMILYREUNIONDINNER2_API AGameMenuPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	UPROPERTY(Replicated, BlueprintReadOnly)
	bool serverListNeedUpdate = true;

	UPROPERTY(Replicated, BlueprintReadOnly)
	int FamilyReunionDinner2PlayerID;

	UPROPERTY(Replicated, BlueprintReadOnly)
	TArray<FServerInfoStruct> serverInfo;

	UFUNCTION(Reliable, Server, BlueprintCallable)
	void updateComplete();
	void updateComplete_Implementation();

};
