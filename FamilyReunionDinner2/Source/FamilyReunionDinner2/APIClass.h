// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "RecipeCardStruct.h"
#include "IngredientCardStruct.h"
#include "CookingCardStruct.h"
#include "MyPlayerState.h"
#include "ServerInfoStruct.h"
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
	static TArray<FIngredientCardStruct> makeIngredientCards();
	static TArray<FCookingCardStruct> makeCookingCards();
	static TArray<TSharedPtr<FJsonObject>> FStringToJson(FString data);
	static TArray<APlayerState*> getPlayerControllersInGame(UWorld* world);
	static bool checkServerListEqual(TArray<FServerInfoStruct> serverInfo1, TArray<FServerInfoStruct> serverInfo2);

	UFUNCTION(BlueprintCallable)
	static FString getIPAddress();
};
