// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameStateBase.h"

void AMyGameStateBase::initGame()
{
	//load recipe cards' data and create them into the scene
	TArray<FRecipeCardStruct> recipeFileData = UAPIClass::makeRecipeCards();
	TSubclassOf<ARecipeCard> recipeCard = LoadClass<ARecipeCard>(nullptr, TEXT("Blueprint'/Game/FirstPersonCPP/Blueprints/MyRecipeCard.MyRecipeCard_C'"));

	for (int i = 0; i < recipeFileData.Num(); i += 1)
	{
		float x = 40 + i % 5 * 16.5;
		float y = -28 + i / 5 * 27;

		ARecipeCard* card = GetWorld()->SpawnActor<ARecipeCard>(recipeCard, FVector(x, y, 53.5), FRotator(0, 0, 0), FActorSpawnParameters());
		card->data = recipeFileData[i];
		recipes.Add(card);
	}
}

void AMyGameStateBase::nextTurn() 
{
	Cast<AMyPlayerState>(PlayerArray[currentTurnIndex])->inTurn = false;
	currentTurnIndex += 1;

	if (currentTurnIndex == PlayerArray.Num()) 
	{
		currentTurnIndex = 0;
	}

	Cast<AMyPlayerState>(PlayerArray[currentTurnIndex])->inTurn = true;

}