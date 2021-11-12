// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "Net/UnrealNetwork.h"
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
	float notificationCount;

	UPROPERTY(Replicated)
	TArray<ACookingCard*> cookingCards = TArray<ACookingCard*>();

	UPROPERTY(Replicated)
	FString monsterPreference;

	UPROPERTY(Replicated, BlueprintReadOnly)
	bool inTurn = false;
};
