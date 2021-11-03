// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "RecipeCardStruct.h"
#include "APIClass.generated.h"

/**
 * 
 */
UCLASS()
class FAMILYREUNIONDINNER2_API UAPIClass : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	static TArray<FRecipeCardStruct> makeRecipeCards();
	static TArray<TSharedPtr<FJsonObject>> FStringToJson(FString data);
};
