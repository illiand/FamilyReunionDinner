// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CompletedRecipeInfo.generated.h"
/**
 * 
 */
USTRUCT(BlueprintType)
struct FCompletedRecipeInfo
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadOnly)
	FString path;

	UPROPERTY(BlueprintReadOnly)
	int flavor;

	UPROPERTY(BlueprintReadOnly)
	int heat;

	UPROPERTY(BlueprintReadOnly)
	int point;

	UPROPERTY(BlueprintReadOnly)
	bool failed;

	UPROPERTY(BlueprintReadOnly)
	FString failedReason;
};