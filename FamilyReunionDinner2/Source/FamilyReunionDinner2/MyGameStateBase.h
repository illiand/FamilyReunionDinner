// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "RecipeCardStruct.h"
#include "RecipeCard.h"
#include "IngredientCardStruct.h"
#include "IngredientCard.h"
#include "CookingCardStruct.h"
#include "CookingCard.h"
#include "MyGameStateBase.generated.h"

/**
 * 
 */
UCLASS()
class FAMILYREUNIONDINNER2_API AMyGameStateBase : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	TArray<FRecipeCardStruct> recipeCardFileData = TArray<FRecipeCardStruct>();
	TArray<FIngredientCardStruct> ingredientCardFileData = TArray<FIngredientCardStruct>();
	TArray<FCookingCardStruct> cookingCardFileData = TArray<FCookingCardStruct>();

	TArray<FRecipeCardStruct> recipeCardOnTableFileData = TArray<FRecipeCardStruct>();
	TArray<FIngredientCardStruct> ingredientCardOnTableFileData = TArray<FIngredientCardStruct>();
	
public:
	void initGame();
	void nextTurn();

private:
	int currentTurnIndex = 0;
};
