// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ServerInfoHandler.generated.h"
/**
 *
 */
USTRUCT(BlueprintType)
struct FServerInfoHandler
{
	GENERATED_USTRUCT_BODY()

	int pid;
	TArray<FString> playerName;
};
