// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ServerListInfoStruct.generated.h"
/**
 *
 */
USTRUCT(BlueprintType)
struct FServerListInfoStruct
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadOnly)
	FString name;

	UPROPERTY(BlueprintReadOnly)
	int ID;

	UPROPERTY(BlueprintReadOnly)
	int curPlayerNum;

	UPROPERTY(BlueprintReadOnly)
	int maxPlayerNum;
};