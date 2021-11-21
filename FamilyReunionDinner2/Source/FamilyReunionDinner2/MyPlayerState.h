// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "IngredientCard.h"
#include "RecipeCard.h"
#include "CookingCard.h"
#include "MyPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class FAMILYREUNIONDINNER2_API AMyPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	//not actually exist on server
	TArray<AIngredientCard*> ingredientCards = TArray<AIngredientCard*>();
	TArray<ARecipeCard*> recipeCards = TArray<ARecipeCard*>();

	TArray<ACookingCard*> cookingCards = TArray<ACookingCard*>();

	UPROPERTY(BlueprintReadOnly)
	bool inTurn = false;

public:
	void setCardRotationBasedOnPlayerLocation(AActor* card);

public:
	UFUNCTION(Reliable, Client)
	void createIngredientCard(FIngredientCardStruct data, int positionIndex);
	void createIngredientCard_Implementation(FIngredientCardStruct data, int positionIndex);

	UFUNCTION(Reliable, Client)
	void createRecipeCard(FRecipeCardStruct data, int positionIndex);
	void createRecipeCard_Implementation(FRecipeCardStruct data, int positionIndex);

	UFUNCTION(Reliable, Client)
	void setMonsterPreference();
	void setMonsterPreference_Implementation();

	void setMonsterPreferenceUI(const FString& path);

	UFUNCTION(Reliable, Client)
	void setTurn(bool ifTurn);
	void setTurn_Implementation(bool ifTurn);
};
