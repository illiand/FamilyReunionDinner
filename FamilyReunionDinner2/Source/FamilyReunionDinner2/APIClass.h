// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "APIClass.generated.h"

/**
 * 
 */
UCLASS()
class FAMILYREUNIONDINNER2_API UAPIClass : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	//read file to get a json array
	//UFUNCTION(BlueprintCallable, Category = "API")
	static TArray<TSharedPtr<FJsonObject>> readFile(FString path);
	static TArray<TSharedPtr<FJsonObject>> FStringToJson(FString data);
	
	UFUNCTION(BlueprintCallable, Category = "API")
	static TArray<ARecipeCard*> callXRC();
};
