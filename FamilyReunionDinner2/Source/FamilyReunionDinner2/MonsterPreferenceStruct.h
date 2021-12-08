// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MonsterPreferenceStruct.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FMonsterPreferenceStruct
{
	GENERATED_USTRUCT_BODY();

	UPROPERTY(BlueprintReadOnly)
	FString name;

	UPROPERTY(BlueprintReadOnly)
	FString effect;

	UPROPERTY(BlueprintReadOnly)
	FString path;
};
