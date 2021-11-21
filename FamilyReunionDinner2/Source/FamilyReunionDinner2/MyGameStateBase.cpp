// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameStateBase.h"

void AMyGameStateBase::initGame()
{
	TArray<FRecipeCardStruct> recipeFileData = UAPIClass::makeRecipeCards();
	TSubclassOf<ARecipeCard> recipeCard = LoadClass<ARecipeCard>(nullptr, TEXT("Blueprint'/Game/FirstPersonCPP/Blueprints/MyRecipeCard.MyRecipeCard_C'"));
	TArray<FIngredientCardStruct> ingredientFileData = UAPIClass::makeIngredientCards();
	TSubclassOf<AIngredientCard> ingredientCard = LoadClass<AIngredientCard>(nullptr, TEXT("Blueprint'/Game/FirstPersonCPP/Blueprints/MyIngredientCard.MyIngredientCard_C'"));
	TArray<FCookingCardStruct> cookingFileData = UAPIClass::makeCookingCards();
	TSubclassOf<ACookingCard> cookingCard = LoadClass<ACookingCard>(nullptr, TEXT("Blueprint'/Game/FirstPersonCPP/Blueprints/MyCookingCard.MyCookingCard_C'"));
	for (int i = 0; i < 5; i += 1)
	{
		float x = 40 + i % 5 * 16.5;
		float y = 30 + i / 5 * 27;

		ACookingCard* card = GetWorld()->SpawnActor<ACookingCard>(cookingCard, FVector(x, y, 64.5), FRotator(0, 180, 90), FActorSpawnParameters());
		card->data = cookingFileData[i];
		cooking.Add(card);
	}
	for (int i = 0; i < 4; i += 1)
	{
		float x = 95 + i % 5 * 16.5;
		float y = -28 + i / 5 * 27;

		ARecipeCard* card = GetWorld()->SpawnActor<ARecipeCard>(recipeCard, FVector(x, y, 53.5), FRotator(0, 0, 0), FActorSpawnParameters());
		card->data = recipeFileData[i];
		recipes.Add(card);

		y = y + 25;
		AIngredientCard* card2 = GetWorld()->SpawnActor<AIngredientCard>(ingredientCard, FVector(x, y, 53.5), FRotator(0, 0, 0), FActorSpawnParameters());
		card2->data = ingredientFileData[i];
		ingredients.Add(card2);
	}
	//for (int i = 0; i < recipeFileData.Num(); i += 1)
	//{
	//	float x = 40 + i % 5 * 16.5;
	//	float y = -28 + i / 5 * 27;

	//	AIngredientCard* card = GetWorld()->SpawnActor<AIngredientCard>(recipeCard, FVector(x, y, 53.5), FRotator(0, 0, 0), FActorSpawnParameters());
	//	card->data = recipeFileData[i];
	//	ingredients.Add(card);
	//}
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