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

#include "MonsterPreferenceStruct.h"

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
	TArray<FMonsterPreferenceStruct> monsterPreferenceFileData = TArray<FMonsterPreferenceStruct>();

	TArray<FRecipeCardStruct> recipeCardOnTableFileData = TArray<FRecipeCardStruct>();
	TArray<FIngredientCardStruct> ingredientCardOnTableFileData = TArray<FIngredientCardStruct>();
	
	TArray<FRecipeCardStruct> completedDishFileData = TArray<FRecipeCardStruct>();

	TArray<FMonsterPreferenceStruct> monsterPreferenceInGame = TArray<FMonsterPreferenceStruct>();

	FString monsterNameInGame;
	FString monsterPathInGame;

public:
	FTimerHandle turnTimer;
	FTimerHandle actionTimer;

	int inActionPotIndex = 0;
	int inActionPotItemIndex = -1;
	bool succussAction = false;

	int curRound = 1;
	int maxRound = 8;

public:
	void initGame();
	void nextTurn();

	void addRecipeCardInGame(int index);
	void addIngredientCardInGame(int index);
	void addCookingCardInGame(APlayerState* playerState, FVector position, FRotator rotation, int index);

	void castIngredientCardEffect(int cardIndex, int potIndex);
	void castCookingCardEffect(ACookingCard* card, int potIndex);
	void castRecipeCardEffect(int potIndex);

	void removeRecipeCardInGame(int index);
	void removeIngredientCardInGame(int index);
	void removeCookingCardInGame(APlayerState* playerState, int index);

	void turnTimeUp();

	void activeCompleteDishTimer(int index);
	void reactionTimeUpWithCompleteDish(int index);
	void getDishActionResult(int index);

	void activeRemoveItemInPotTimer(int index, int potIndex);
	void reactionTimeUpWithRemoveItemInPot(int index, int potIndex);
	void getRemoveItemActionResult(int index, int potIndex);

	bool checkRecipeSuccuss(FRecipeCardStruct toCheck, FString& failedReason);
	bool checkPreferenceSuccuss(FMonsterPreferenceStruct toCheck, FString& failedReason);
	bool checkMonsterSuccuss(FString& failedReason);

	bool hasPot(FString type);

	void checkGameFinalStatus();
	bool checkCanGameOver(FString& errorMessage);

private:
	int currentTurnIndex = 0;
};
