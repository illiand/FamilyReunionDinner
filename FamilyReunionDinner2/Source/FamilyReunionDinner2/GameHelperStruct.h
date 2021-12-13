// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameHelperStruct.generated.h"
/**
 * 
 */
USTRUCT(BlueprintType)
struct FGameHelperStruct
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadOnly)
	FString title;

	UPROPERTY(BlueprintReadOnly)
	FString path;
	
	UPROPERTY(BlueprintReadOnly)
	FString text;
};
