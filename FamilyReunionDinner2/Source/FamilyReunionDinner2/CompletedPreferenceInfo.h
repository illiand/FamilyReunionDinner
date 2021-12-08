// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CompletedPreferenceInfo.generated.h"
/**
 * 
 */
USTRUCT(BlueprintType)
struct FCompletedPreferenceInfo
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadOnly)
	FString path;

	UPROPERTY(BlueprintReadOnly)
	bool failed;

	UPROPERTY(BlueprintReadOnly)
	FString failedReason;
};
