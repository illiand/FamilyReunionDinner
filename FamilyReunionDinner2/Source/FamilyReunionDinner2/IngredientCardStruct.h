// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IngredientCardStruct.generated.h"
/**
 *
 */
USTRUCT(BlueprintType)
struct FIngredientCardStruct
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadOnly)
		FString name;
	UPROPERTY(BlueprintReadOnly)
		FString type;
	UPROPERTY(BlueprintReadOnly)
		FString size;
	UPROPERTY(BlueprintReadOnly)
		FString point;
};
