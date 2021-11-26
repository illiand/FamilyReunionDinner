// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameStateBase.h"
#include "APIClass.h"
#include "CardBorderActor.h"
#include "MyPlayerState.h"

void AMyGameStateBase::initGame()
{
	recipeCardFileData = UAPIClass::makeRecipeCards();
	TSubclassOf<ARecipeCard> recipeCard = LoadClass<ARecipeCard>(nullptr, TEXT("Blueprint'/Game/FirstPersonCPP/Blueprints/MyRecipeCard.MyRecipeCard_C'"));
	ingredientCardFileData = UAPIClass::makeIngredientCards();
	TSubclassOf<AIngredientCard> ingredientCard = LoadClass<AIngredientCard>(nullptr, TEXT("Blueprint'/Game/FirstPersonCPP/Blueprints/MyIngredientCard.MyIngredientCard_C'"));
	cookingCardFileData = UAPIClass::makeCookingCards();
	TSubclassOf<ACookingCard> cookingCard = LoadClass<ACookingCard>(nullptr, TEXT("Blueprint'/Game/FirstPersonCPP/Blueprints/MyCookingCard.MyCookingCard_C'"));
	monsterPreferenceFileData = UAPIClass::makeMonsterPreference();

	int cookingCardCount = 6;
	// cookingCardCount = 7 if PlayerArray.Num() = 3/4

	//randomly create replicatable cooking cards
	for (int j = 0; j < PlayerArray.Num(); j += 1)
	{//create cards for each player
		FVector playerLocation = PlayerArray[j]->GetPawn()->GetActorLocation();
		
		for (int i = 0; i < cookingCardCount; i += 1) {
			float y = 30 + i / 5 * 27;
			float x = playerLocation.X - 20 + (i % 5 * 16.5);
			if (j % 2 != 0){
				y = -(30 + (i / 5 * 27));
			}
			
			ACookingCard* card = GetWorld()->SpawnActor<ACookingCard>(cookingCard, FVector(x,y, 64.5), FRotator(0, (j-1)*180, 90), FActorSpawnParameters());
			int curCardIndex = FMath::RandRange(0, cookingCardFileData.Num() - 1);
			card->data = cookingCardFileData[curCardIndex];
			cookingCardFileData.RemoveAt(curCardIndex);
			Cast<AMyPlayerState>(PlayerArray[j])->cookingCards.Add(card);
		}
	}

	for (int i = 0; i < 4; i += 1)
	{
		addRecipeCardInGame(i);
		addIngredientCardInGame(i);
	}

	for (int i = 0; i < PlayerArray.Num(); i += 1) 
	{
		int curCardIndex = FMath::RandRange(0, monsterPreferenceFileData.Num() - 1);
		monsterPreferenceInGame.Add(monsterPreferenceFileData[curCardIndex]);
		Cast<AMyPlayerState>(PlayerArray[i])->setMonsterPreferenceUI(monsterPreferenceFileData[curCardIndex].path);

		monsterPreferenceFileData.RemoveAt(curCardIndex);
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
	Cast<AMyPlayerState>(PlayerArray[currentTurnIndex])->hintShowed = false;
	Cast<AMyPlayerState>(PlayerArray[currentTurnIndex])->setTurn(true);
}

void AMyGameStateBase::addRecipeCardInGame(int index) 
{
	if (recipeCardFileData.Num() == 0)
	{
		return;
	}

	int curCardIndex = FMath::RandRange(0, recipeCardFileData.Num() - 1);
	recipeCardOnTableFileData.Insert(recipeCardFileData[curCardIndex], index);
	recipeCardFileData.RemoveAt(curCardIndex);

	for (int i = 0; i < PlayerArray.Num(); i += 1)
	{
		Cast<AMyPlayerState>(PlayerArray[i])->createRecipeCard(recipeCardOnTableFileData[index], index);
	}
}

void AMyGameStateBase::addIngredientCardInGame(int index)
{
	if (ingredientCardFileData.Num() == 0)
	{
		return;
	}

	int curCardIndex = FMath::RandRange(0, ingredientCardFileData.Num() - 1);
	ingredientCardOnTableFileData.Insert(ingredientCardFileData[curCardIndex], index);
	ingredientCardFileData.RemoveAt(curCardIndex);

	for (int i = 0; i < PlayerArray.Num(); i += 1)
	{
		Cast<AMyPlayerState>(PlayerArray[i])->createIngredientCard(ingredientCardOnTableFileData[index], index);
	}
}

void AMyGameStateBase::addCookingCardInGame(APlayerState* playerState, FVector position, FRotator rotation, int index)
{
	if (cookingCardFileData.Num() == 0)
	{
		return;
	}

	TSubclassOf<ACookingCard> cookingCard = LoadClass<ACookingCard>(nullptr, TEXT("Blueprint'/Game/FirstPersonCPP/Blueprints/MyCookingCard.MyCookingCard_C'"));
	ACookingCard* card = GetWorld()->SpawnActor<ACookingCard>(cookingCard, position, rotation, FActorSpawnParameters());
	int curCardIndex = FMath::RandRange(0, cookingCardFileData.Num() - 1);

	card->data = cookingCardFileData[curCardIndex];
	cookingCardFileData.RemoveAt(curCardIndex);

	Cast<AMyPlayerState>(playerState)->cookingCards.Add(card);
}

void AMyGameStateBase::removeRecipeCardInGame(int index)
{
	recipeCardOnTableFileData.RemoveAt(index);

	for (int i = 0; i < PlayerArray.Num(); i += 1)
	{
		Cast<AMyPlayerState>(PlayerArray[i])->destroyRecipeCard(index);
	}
}

void AMyGameStateBase::removeIngredientCardInGame(int index)
{
	ingredientCardOnTableFileData.RemoveAt(index);

	for (int i = 0; i < PlayerArray.Num(); i += 1)
	{
		Cast<AMyPlayerState>(PlayerArray[i])->destroyIngredientCard(index);
	}
}

void AMyGameStateBase::removeCookingCardInGame(APlayerState* playerState, int index)
{
	if (Cast<AMyPlayerState>(playerState)->cookingCards[index]->border != NULL) 
	{
		Cast<AMyPlayerState>(playerState)->cookingCards[index]->border->Destroy();
	}
	
	Cast<AMyPlayerState>(playerState)->cookingCards[index]->Destroy();
	Cast<AMyPlayerState>(playerState)->cookingCards.RemoveAt(index);
}

void AMyGameStateBase::castIngredientCardEffect(int cardIndex, int potIndex)
{
	for (int i = 0; i < PlayerArray.Num(); i += 1)
	{
		Cast<AMyPlayerState>(PlayerArray[i])->addIngredientCardToPot(ingredientCardOnTableFileData[cardIndex], potIndex);
	}

	recipeCardOnTableFileData[potIndex].addedIngredientCards.Add(ingredientCardOnTableFileData[cardIndex]);

	//TODO PARSE EFFECT
}

void AMyGameStateBase::castCookingCardEffect(ACookingCard* card, int potIndex)
{
	for (int i = 0; i < PlayerArray.Num(); i += 1)
	{
		Cast<AMyPlayerState>(PlayerArray[i])->addCookingCardToPot(card->data, potIndex);
	}

	recipeCardOnTableFileData[potIndex].addedCookingCards.Add(card->data);
}

void AMyGameStateBase::castRecipeCardEffect(int index)
{	
	completedDishFileData.Add(recipeCardOnTableFileData[index]);
}