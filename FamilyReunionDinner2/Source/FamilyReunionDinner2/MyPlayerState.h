// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
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
	TArray<ACookingCard*> cookingCards = TArray<ACookingCard*>();

	UPROPERTY(BlueprintReadOnly)
	bool inTurn = false;

public:
	UFUNCTION(Reliable, Client)
	void setCookingCards(const TArray<ACookingCard*>& data);
	void setCookingCards_Implementation(const TArray<ACookingCard*>& data);

	UFUNCTION(Reliable, Client)
	void setMonsterPreferenceUI(const FString& path);
	void setMonsterPreferenceUI_Implementation(const FString& path);

	UFUNCTION(Reliable, Client)
	void setTurn(bool ifTurn);
	void setTurn_Implementation(bool ifTurn);
};
