// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "IngredientCard.h"
#include "RecipeCard.h"
#include "Pot.h"
#include "CookingCard.h"
#include "CompletedRecipeInfo.h"
#include "CompletedPreferenceInfo.h"
#include "MyPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class FAMILYREUNIONDINNER2_API AMyPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	virtual void BeginPlay();

public:
	//not actually exist on server
	TArray<AIngredientCard*> ingredientCards = TArray<AIngredientCard*>();
	TArray<ARecipeCard*> recipeCards = TArray<ARecipeCard*>();
	TArray<APot*> recipePots = TArray<APot*>();

	UPROPERTY(Replicated)
	TArray<ACookingCard*> cookingCards = TArray<ACookingCard*>();

	UPROPERTY(BlueprintReadOnly)
	bool inTurn = false;

	UPROPERTY(BlueprintReadOnly)
	bool inDragging = false;

	UPROPERTY(BlueprintReadOnly)
	bool hintShowed = false;

	UPROPERTY(BlueprintReadOnly)
	bool inReaction = false;

	UPROPERTY(BlueprintReadOnly)
	bool preReaction = false;

	bool reactionComplete = true;

	UPROPERTY(BlueprintReadOnly)
	FString inTurnPlayerName;

	UPROPERTY(BlueprintReadOnly)
	int actionPoint = 1;

	UPROPERTY(BlueprintReadOnly)
	FString FamilyReunionDinner2PlayerID;

	UPROPERTY(BlueprintReadOnly)
	FString gameResult;

	UPROPERTY(BlueprintReadOnly)
	TArray<FString> playersID;

	UPROPERTY(BlueprintReadOnly)
	TArray<FCompletedPreferenceInfo> preferenceResult;

	UPROPERTY(BlueprintReadOnly)
	TArray<FCompletedRecipeInfo> recipeResult;

	FTimerHandle turnTimer;
	FTimerHandle reactionTimer;
private:
	float turnTimeRemaining = 0;
	float actionTimerRemaining = 0;

public:
	void setCardRotationBasedOnPlayerLocation(AActor* card);
	void turnTimerRun();
	void reactionTimerRun();

	/*
	@return data
	0: curFlavor
	1: curHeat
	2: curPoint
	3: curSize
	*/
	TArray<int> calculateParameter(FRecipeCardStruct data);
	int calculateBonusPoint(FRecipeCardStruct data);
	bool potHasType(FString type, FRecipeCardStruct data);
	int calculateHeat(FRecipeCardStruct data);
public:
	UFUNCTION(Reliable, Client)
	void createIngredientCard(FIngredientCardStruct data, int positionIndex);
	void createIngredientCard_Implementation(FIngredientCardStruct data, int positionIndex);

	UFUNCTION(Reliable, Client)
	void createRecipeCard(FRecipeCardStruct data, int positionIndex);
	void createRecipeCard_Implementation(FRecipeCardStruct data, int positionIndex);

	UFUNCTION(Reliable, Client)
	void addIngredientCardToPot(FIngredientCardStruct data, int index);
	void addIngredientCardToPot_Implementation(FIngredientCardStruct data, int index);

	UFUNCTION(Reliable, Client)
	void addCookingCardToPot(FCookingCardStruct data, int index);
	void addCookingCardToPot_Implementation(FCookingCardStruct data, int index);

	UFUNCTION(Reliable, Client)
	void destroyIngredientCard(int index);
	void destroyIngredientCard_Implementation(int index);

	UFUNCTION(Reliable, Client)
	void destroyRecipeCard(int index);
	void destroyRecipeCard_Implementation(int index);

	UFUNCTION(Reliable, Client)
	void removePotItem(int index, int potIndex);
	void removePotItem_Implementation(int index, int potIndex);

	UFUNCTION(Reliable, Client)
	void setMonsterPreferenceUI(const FString& path);
	void setMonsterPreferenceUI_Implementation(const FString& path);

	UFUNCTION(Reliable, Client)
	void setTurn(bool ifTurn);
	void setTurn_Implementation(bool ifTurn);

	UFUNCTION(Reliable, NetMulticast)
	void setInTurnPlayerName(const FString& name);
	void setInTurnPlayerName_Implementation(const FString& name);

	UFUNCTION(Reliable, NetMulticast)
	void setReaction(bool ifReaction);
	void setReaction_Implementation(bool ifReaction);

	UFUNCTION(Reliable, NetMulticast)
	void setReactionComplete(bool ifReactionComplete);
	void setReactionComplete_Implementation(bool ifReactionComplete);

	UFUNCTION(Reliable, NetMulticast)
	void setPreReaction(bool ifPreReaction);
	void setPreReaction_Implementation(bool ifPreReaction);

	UFUNCTION(Reliable, NetMulticast)
	void setActionPoint(int curPoint);
	void setActionPoint_Implementation(int curPoint);

	UFUNCTION(Reliable, Client)
	void activeTurnTimer();
	void activeTurnTimer_Implementation();

	UFUNCTION(Reliable, Client)
	void activeReactionTimer();
	void activeReactionTimer_Implementation();

	UFUNCTION(Reliable, Client)
	void destroyReactionTimer();
	void destroyReactionTimer_Implementation();

	UFUNCTION(Reliable, Client)
	void showWorldMessage(const FString& text, const FVector& color);
	void showWorldMessage_Implementation(const FString& text, const FVector& color);

	UFUNCTION(Reliable, Server)
	void requestUserID();
	void requestUserID_Implementation();

	UFUNCTION(Reliable, Client)
	void sendUserID(const FString& id);
	void sendUserID_Implementation(const FString& id);

	UFUNCTION(Reliable, Client)
	void addToCompletedRecipeUI(const FString& path, int flavor, int heat, int point, bool failed, const FString& failedReason);
	void addToCompletedRecipeUI_Implementation(const FString& path, int flavor, int heat, int point, bool failed, const FString& failedReason);

	UFUNCTION(Reliable, Client)
	void sendGameOverData(const FString& result, const TArray<FCompletedRecipeInfo>& recipeData, const TArray<FCompletedPreferenceInfo>& preferenceData, const TArray<FString>& playersIDData);
	void sendGameOverData_Implementation(const FString& result, const TArray<FCompletedRecipeInfo>& recipeData, const TArray<FCompletedPreferenceInfo>& preferenceData, const TArray<FString>& playersIDData);
};
