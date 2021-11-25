// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CookingCardStruct.generated.h"
/**
 *
 */
USTRUCT(BlueprintType)
struct FCookingCardStruct
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadOnly)
	FString name;
	UPROPERTY(BlueprintReadOnly)
	FString type;
	UPROPERTY(BlueprintReadOnly)
	FString degree;

	UPROPERTY(BlueprintReadOnly)
	bool typeHinted;
	UPROPERTY(BlueprintReadOnly)
	bool degreeHinted;
};
