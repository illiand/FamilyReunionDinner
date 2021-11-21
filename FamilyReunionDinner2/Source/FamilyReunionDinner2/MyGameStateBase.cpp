// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameStateBase.h"

void AMyGameStateBase::initGame()
{
	// LOADING THE RECIPE CARDS ON PRESSING 'T'
	//load recipe/ingredients cards' data and create them into the scene
	//TArray<FRecipeCardStruct> recipeFileData = UAPIClass::makeRecipeCards();
	//TSubclassOf<ARecipeCard> recipeCard = LoadClass<ARecipeCard>(nullptr, TEXT("Blueprint'/Game/FirstPersonCPP/Blueprints/MyRecipeCard.MyRecipeCard_C'"));
	//for (int i = 0; i < recipeFileData.Num(); i += 1)
	//{
	//	float x = 40 + i % 5 * 16.5;
	//	float y = -28 + i / 5 * 27;

	//	AIngredientCard* card = GetWorld()->SpawnActor<AIngredientCard>(recipeCard, FVector(x, y, 53.5), FRotator(0, 0, 0), FActorSpawnParameters());
	//	card->data = recipeFileData[i];
	//	ingredients.Add(card);
	//}
	// LOADING THE INGREDIENT CARDS ON PRESSING 'T'
	//TArray<FIngredientCardStruct> ingredientFileData = UAPIClass::makeIngredientCards();
	//TSubclassOf<AIngredientCard> ingredientCard = LoadClass<AIngredientCard>(nullptr, TEXT("Blueprint'/Game/FirstPersonCPP/Blueprints/MyIngredientCard.MyIngredientCard_C'"));
	//for (int i = 0; i < ingredientFileData.Num(); i += 1)
	//{
	//	float x = 40 + i % 5 * 16.5;
	//	float y = -28 + i / 5 * 27;

	//	AIngredientCard* card = GetWorld()->SpawnActor<AIngredientCard>(ingredientCard, FVector(x, y, 53.5), FRotator(0, 0, 0), FActorSpawnParameters());
	//	card->data = ingredientFileData[i];
	//	ingredients.Add(card);
	//}
	// LOADING THE COOKING CARDS ON PRESSING 'T'
	TArray<FCookingCardStruct> ingredientFileData = UAPIClass::makeCookingCards();
	TSubclassOf<ACookingCard> ingredientCard = LoadClass<ACookingCard>(nullptr, TEXT("Blueprint'/Game/FirstPersonCPP/Blueprints/MyCookingCard.MyCookingCard_C'"));
	for (int i = 0; i < ingredientFileData.Num(); i += 1)
	{
		float x = 40 + i % 5 * 16.5;
		float y = -28 + i / 5 * 27;

		ACookingCard* card = GetWorld()->SpawnActor<ACookingCard>(ingredientCard, FVector(x, y, 53.5), FRotator(0, 0, 0), FActorSpawnParameters());
		card->data = ingredientFileData[i];
		ingredients.Add(card);
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