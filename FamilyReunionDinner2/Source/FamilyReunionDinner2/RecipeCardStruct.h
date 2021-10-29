// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RecipeCardStruct.generated.h"
/**
 * 
 */
USTRUCT(Blueprintable)
struct FRecipeCardStruct
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	FString name;
	UPROPERTY()
	FString type;
	UPROPERTY()
	FString flavorRange;
	UPROPERTY()
	FString heatRange;
	UPROPERTY()
	FString size;
	UPROPERTY()
	FString point;
};
