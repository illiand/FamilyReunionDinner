// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CookingCardStruct.h"
#include "IngredientCardStruct.h"
#include "RecipeCardStruct.generated.h"
/**
 * 
 */
USTRUCT(BlueprintType)
struct FRecipeCardStruct
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadOnly)
	FString name;
	UPROPERTY(BlueprintReadOnly)
	FString type;
	UPROPERTY(BlueprintReadOnly)
	FString flavorRange;
	UPROPERTY(BlueprintReadOnly)
	FString heatRange;
	UPROPERTY(BlueprintReadOnly)
	FString size;
	UPROPERTY(BlueprintReadOnly)
	FString point;

	UPROPERTY(BlueprintReadOnly)
	TArray<FCookingCardStruct> addedCookingCards;

	UPROPERTY(BlueprintReadOnly)
	TArray<FIngredientCardStruct> addedIngredientCards;
};
