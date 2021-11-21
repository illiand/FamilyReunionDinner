// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerState.h"

void AMyPlayerState::setCookingCards_Implementation(const TArray<ACookingCard*>& data)
{
	cookingCards = data;
}

void AMyPlayerState::setMonsterPreferenceUI_Implementation(const FString& path)
{

}

void AMyPlayerState::setTurn_Implementation(bool ifTurn)
{
	inTurn = ifTurn;
}