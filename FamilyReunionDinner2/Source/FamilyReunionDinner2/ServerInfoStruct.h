// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameMenuCharacter.h"
#include "ServerInfoStruct.generated.h"
/**
 *
 */
USTRUCT(BlueprintType)
struct FServerInfoStruct
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadOnly)
	FString name;
	UPROPERTY(BlueprintReadOnly)
	TArray<AGameMenuCharacter*> currentPlayers;
	UPROPERTY(BlueprintReadOnly)
	int maxPlayerNum;
};