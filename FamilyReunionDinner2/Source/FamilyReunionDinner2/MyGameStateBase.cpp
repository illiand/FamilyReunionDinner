// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameStateBase.h"
#include "APIClass.h"
#include "MyPlayerState.h"

void AMyGameStateBase::initGame()
{
	recipeCardFileData = UAPIClass::makeRecipeCards();
	TSubclassOf<ARecipeCard> recipeCard = LoadClass<ARecipeCard>(nullptr, TEXT("Blueprint'/Game/FirstPersonCPP/Blueprints/MyRecipeCard.MyRecipeCard_C'"));
	ingredientCardFileData = UAPIClass::makeIngredientCards();
	TSubclassOf<AIngredientCard> ingredientCard = LoadClass<AIngredientCard>(nullptr, TEXT("Blueprint'/Game/FirstPersonCPP/Blueprints/MyIngredientCard.MyIngredientCard_C'"));
	cookingCardFileData = UAPIClass::makeCookingCards();
	TSubclassOf<ACookingCard> cookingCard = LoadClass<ACookingCard>(nullptr, TEXT("Blueprint'/Game/FirstPersonCPP/Blueprints/MyCookingCard.MyCookingCard_C'"));
	
	int cookingCardCount = 5;
	// cookingCardCount = 7 if PlayerArray.Num() = 3/4

	//randomly create replicatable cooking cards
	for (int j = 0; j < PlayerArray.Num(); j += 1)
	{//create cards for each player
		FVector playerLocation = PlayerArray[j]->GetPawn()->GetActorLocation();
		UE_LOG(LogTemp, Warning, TEXT("%f %f"), playerLocation.X, playerLocation.Y);
		for (int i = 0; i < cookingCardCount; i += 1) {
			float y = 30 + i / 5 * 27;
			float x = playerLocation.X - 20 + (i % 5 * 16.5);
			if (j % 2 != 0){
				y = -(30 + (i / 5 * 27));
			}
			UE_LOG(LogTemp, Warning, TEXT("%f"), x);
			ACookingCard* card = GetWorld()->SpawnActor<ACookingCard>(cookingCard, FVector(x,y, 64.5), FRotator(0, (j-1)*180, 90), FActorSpawnParameters());
			int curCardIndex = FMath::RandRange(0, cookingCardFileData.Num() - 1);
			card->data = cookingCardFileData[curCardIndex];
			cookingCardFileData.RemoveAt(curCardIndex);
			Cast<AMyPlayerState>(PlayerArray[0])->cookingCards.Add(card);
		}
	}

	//pick 4 cards randomly for each
	for (int i = 0; i < 4; i += 1)
	{
		int curCardIndex = FMath::RandRange(0, recipeCardFileData.Num() - 1);
		recipeCardOnTableFileData.Add(recipeCardFileData[curCardIndex]);
		recipeCardFileData.RemoveAt(curCardIndex);

		curCardIndex = FMath::RandRange(0, ingredientCardFileData.Num() - 1);
		ingredientCardOnTableFileData.Add(ingredientCardFileData[curCardIndex]);
		ingredientCardFileData.RemoveAt(curCardIndex);
	}

	//send the picking data to all the players
	for (int i = 0; i < PlayerArray.Num(); i += 1)
	{
		for (int j = 0; j < 4; j += 1)
		{
			Cast<AMyPlayerState>(PlayerArray[i])->createRecipeCard(recipeCardOnTableFileData[j], j);
			Cast<AMyPlayerState>(PlayerArray[i])->createIngredientCard(ingredientCardOnTableFileData[j], j);
		}
	}
}

void AMyGameStateBase::nextTurn() 
{
	Cast<AMyPlayerState>(PlayerArray[currentTurnIndex])->inTurn = false;
	Cast<AMyPlayerState>(PlayerArray[currentTurnIndex])->setTurn(false);
	currentTurnIndex += 1;
	
	if (currentTurnIndex == PlayerArray.Num()) 
	{
		currentTurnIndex = 0;
	}

	Cast<AMyPlayerState>(PlayerArray[currentTurnIndex])->inTurn = true;
	Cast<AMyPlayerState>(PlayerArray[currentTurnIndex])->setTurn(true);
}