// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerState.h"

void AMyPlayerState::createIngredientCard_Implementation(FIngredientCardStruct data, int positionIndex)
{
	TSubclassOf<AIngredientCard> recipeCard = LoadClass<AIngredientCard>(nullptr, TEXT("Blueprint'/Game/FirstPersonCPP/Blueprints/MyIngredientCard.MyIngredientCard_C'"));
	FVector position = FVector(95 + positionIndex % 5 * 16.5, -28 + positionIndex / 5 * 27 + 25, 53.5);

	AIngredientCard* card = GetWorld()->SpawnActor<AIngredientCard>(recipeCard, position, FRotator(0, 0, 0), FActorSpawnParameters());
	card->data = data;
	card->assignInfo();

	ingredientCards.Insert(card, positionIndex);
	setCardRotationBasedOnPlayerLocation(card);
}

void AMyPlayerState::createRecipeCard_Implementation(FRecipeCardStruct data, int positionIndex)
{
	TSubclassOf<ARecipeCard> recipeCard = LoadClass<ARecipeCard>(nullptr, TEXT("Blueprint'/Game/FirstPersonCPP/Blueprints/MyRecipeCard.MyRecipeCard_C'"));
	FVector position = FVector(95 + positionIndex % 5 * 16.5, -28 + positionIndex / 5 * 27, 53.5);

	ARecipeCard* card = GetWorld()->SpawnActor<ARecipeCard>(recipeCard, position, FRotator(0, 0, 0), FActorSpawnParameters());
	card->data = data;
	card->assignInfo();

	recipeCards.Insert(card, positionIndex);
	setCardRotationBasedOnPlayerLocation(card);
}

void AMyPlayerState::addIngredientCardToPot_Implementation(FIngredientCardStruct data, int index) 
{
	recipeCards[index]->data.addedIngredientCards.Add(data);
}

void AMyPlayerState::addCookingCardToPot_Implementation(FCookingCardStruct data, int index)
{
	recipeCards[index]->data.addedCookingCards.Add(data);
}

void AMyPlayerState::drawFinishedRecipeUI_Implementation(FRecipeCardStruct data) 
{

}

void AMyPlayerState::destroyIngredientCard_Implementation(int index)
{
	ingredientCards[index]->Destroy();
	ingredientCards.RemoveAt(index);
}

void AMyPlayerState::destroyRecipeCard_Implementation(int index)
{
	recipeCards[index]->Destroy();
	recipeCards.RemoveAt(index);
}

void AMyPlayerState::setCardRotationBasedOnPlayerLocation(AActor* card) 
{
	if (GetPawn()->GetActorLocation().Y == 60) 
	{

	}
	else if (GetPawn()->GetActorLocation().Y == -90)
	{
		card->SetActorRotation(FRotator(0, 180, 0));
	}
}

void AMyPlayerState::setMonsterPreference_Implementation()
{
	
}

void AMyPlayerState::setMonsterPreferenceUI(const FString& path)
{

}

void AMyPlayerState::setTurn_Implementation(bool ifTurn)
{
	inTurn = ifTurn;

	if (inTurn) 
	{
		hintShowed = false;
	}
}