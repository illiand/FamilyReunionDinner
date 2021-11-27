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

public:
	FTimerHandle turnTimer;
	FTimerHandle actionTimer;

	int inActionPotIndex = 0;
	int inActionPotItemIndex = -1;
	bool succussAction = false;

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
	void activeMoveItemInPotTimer(int index, int potIndex);
	void reactionTimeUpWithMoveItemInPot(int index, int potIndex);
private:
	int currentTurnIndex = 0;
};
