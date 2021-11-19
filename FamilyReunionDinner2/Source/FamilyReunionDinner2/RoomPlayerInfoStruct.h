// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameMenuCharacter.h"
#include "RoomPlayerInfoStruct.generated.h"
/**
 *
 */
USTRUCT(BlueprintType)
struct FRoomPlayerInfoStruct
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadOnly)
	FString name;

	UPROPERTY(BlueprintReadOnly)
	int ID;
};
