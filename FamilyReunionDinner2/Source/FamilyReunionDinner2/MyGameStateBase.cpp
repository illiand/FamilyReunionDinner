// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameStateBase.h"
#include "APIClass.h"
#include "CardBorderActor.h"
#include "FamilyReunionDinner2Character.h"
#include "Kismet/KismetMathLibrary.h"
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

	int cookingCardCount = 2;
	// cookingCardCount = 7 if PlayerArray.Num() = 3/4

	//randomly create replicatable cooking cards
	for (int i = 0; i < PlayerArray.Num(); i += 1)
	{
		for (int j = 0; j < cookingCardCount; j += 1)
		{
			FRotator rotation = FRotator(0, Cast<AFamilyReunionDinner2Character>(PlayerArray[i]->GetPawn())->originalZRotation, 0);
			FVector direction = UKismetMathLibrary::GetForwardVector(rotation);
			direction.Normalize();

			FVector position = direction * 30 + Cast<AFamilyReunionDinner2Character>(PlayerArray[i]->GetPawn())->GetActorLocation();
			FVector offset = direction * (-15.0 * ((cookingCardCount - 1) / 2.0) + j * 15.0);

			position += offset.RotateAngleAxis(90, FVector(0, 0, 1));
			position.Z = -25;

			rotation.Add(0, -90, 90);

			addCookingCardInGame(PlayerArray[i], position, rotation, j);
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

	GetWorldTimerManager().SetTimer(turnTimer, this, &AMyGameStateBase::turnTimeUp, 1, false, 90);

	for (int i = 0; i < PlayerArray.Num(); i += 1)
	{
		Cast<AMyPlayerState>(PlayerArray[i])->activeTurnTimer();
		Cast<AMyPlayerState>(PlayerArray[i])->setInTurnPlayerName(Cast<AMyPlayerState>(PlayerArray[0])->FamilyReunionDinner2PlayerID);

		if (i != currentTurnIndex)
		{
			FString message = TEXT("Player ");
			message.Append(Cast<AMyPlayerState>(PlayerArray[0])->FamilyReunionDinner2PlayerID);
			message.Append(TEXT("'s Turn"));
			Cast<AFamilyReunionDinner2Character>(PlayerArray[i]->GetPawn())->setWaitingTextUI(message);
		}
		else
		{
			Cast<AFamilyReunionDinner2Character>(PlayerArray[i]->GetPawn())->setWaitingTextUI(TEXT("Your Turn"));
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
	Cast<AMyPlayerState>(PlayerArray[currentTurnIndex])->hintShowed = false;
	Cast<AMyPlayerState>(PlayerArray[currentTurnIndex])->setTurn(true);

	GetWorldTimerManager().ClearTimer(turnTimer);
	GetWorldTimerManager().SetTimer(turnTimer, this, &AMyGameStateBase::turnTimeUp, 1, false, 90);

	for (int i = 0; i < PlayerArray.Num(); i += 1)
	{
		Cast<AMyPlayerState>(PlayerArray[i])->activeTurnTimer();
		Cast<AMyPlayerState>(PlayerArray[i])->setInTurnPlayerName(Cast<AMyPlayerState>(PlayerArray[currentTurnIndex])->FamilyReunionDinner2PlayerID);

		if (i != currentTurnIndex) 
		{
			FString message = TEXT("Player ");
			message.Append(Cast<AMyPlayerState>(PlayerArray[currentTurnIndex])->FamilyReunionDinner2PlayerID);
			message.Append(TEXT("'s Turn"));
			Cast<AFamilyReunionDinner2Character>(PlayerArray[i]->GetPawn())->setWaitingTextUI(message);
		}
		else 
		{
			Cast<AFamilyReunionDinner2Character>(PlayerArray[i]->GetPawn())->setWaitingTextUI(TEXT("Your Turn"));
		}
	}
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

		FString message = "Player ";
		message.Append(Cast<AMyPlayerState>(PlayerArray[currentTurnIndex])->FamilyReunionDinner2PlayerID);
		message.Append(" Added ");
		message.Append(ingredientCardOnTableFileData[cardIndex].name);
		message.Append(" to ");
		message.Append(recipeCardOnTableFileData[potIndex].name);

		Cast<AMyPlayerState>(PlayerArray[i])->showWorldMessage(message, FVector(0, 1, 0));
	}

	recipeCardOnTableFileData[potIndex].addedIngredientCards.Add(ingredientCardOnTableFileData[cardIndex]);
}

void AMyGameStateBase::castCookingCardEffect(ACookingCard* card, int potIndex)
{
	for (int i = 0; i < PlayerArray.Num(); i += 1)
	{
		Cast<AMyPlayerState>(PlayerArray[i])->addCookingCardToPot(card->data, potIndex);

		FString message = "Player ";
		message.Append(Cast<AMyPlayerState>(PlayerArray[currentTurnIndex])->FamilyReunionDinner2PlayerID);
		message.Append(" Added ");
		message.Append(card->data.name);
		message.Append(" to ");
		message.Append(recipeCardOnTableFileData[potIndex].name);

		Cast<AMyPlayerState>(PlayerArray[i])->showWorldMessage(message, FVector(0, 1, 0));
	}

	recipeCardOnTableFileData[potIndex].addedCookingCards.Add(card->data);
}

void AMyGameStateBase::castRecipeCardEffect(int index)
{	
	completedDishFileData.Add(recipeCardOnTableFileData[index]);
}

void AMyGameStateBase::turnTimeUp() 
{
	nextTurn();
}

void AMyGameStateBase::activeCompleteDishTimer(int index)
{
	GetWorldTimerManager().ClearTimer(actionTimer);

	FTimerDelegate TimerDel = FTimerDelegate::CreateUObject(this, &AMyGameStateBase::reactionTimeUpWithCompleteDish, index);
	GetWorldTimerManager().SetTimer(actionTimer, TimerDel, 1, false, 10);
	
	for (int i = 0; i < GetWorld()->GetGameState()->PlayerArray.Num(); i += 1)
	{
		Cast<AMyPlayerState>(GetWorld()->GetGameState()->PlayerArray[i])->activeReactionTimer();
	}
}

void AMyGameStateBase::activeRemoveItemInPotTimer(int index, int potIndex)
{
	GetWorldTimerManager().ClearTimer(actionTimer);

	FTimerDelegate TimerDel = FTimerDelegate::CreateUObject(this, &AMyGameStateBase::reactionTimeUpWithRemoveItemInPot, index, potIndex);
	GetWorldTimerManager().SetTimer(actionTimer, TimerDel, 1, false, 10);

	for (int i = 0; i < GetWorld()->GetGameState()->PlayerArray.Num(); i += 1)
	{
		Cast<AMyPlayerState>(GetWorld()->GetGameState()->PlayerArray[i])->activeReactionTimer();
	}
}

void AMyGameStateBase::reactionTimeUpWithCompleteDish(int index)
{
	getDishActionResult(index);

	for (int i = 0; i < GetWorld()->GetGameState()->PlayerArray.Num(); i += 1)
	{
		if (succussAction)
		{
			FString message = TEXT("Finish Recipe Action Completed");
			Cast<AMyPlayerState>(GetWorld()->GetGameState()->PlayerArray[i])->showWorldMessage(message, FVector(0, 1, 0));
		}
		else
		{
			FString message = TEXT("Finish Recipe Action Rejected");
			Cast<AMyPlayerState>(GetWorld()->GetGameState()->PlayerArray[i])->showWorldMessage(message, FVector(1, 0, 0));
		}
	}

}

void AMyGameStateBase::getDishActionResult(int index) 
{
	GetWorldTimerManager().ClearTimer(actionTimer);

	if (succussAction)
	{
		castRecipeCardEffect(index);

		TArray<int> parameters = Cast<AMyPlayerState>(PlayerArray[0])->calculateParameter(recipeCardOnTableFileData[index]);

		for (int i = 0; i < PlayerArray.Num(); i += 1)
		{
			Cast<AMyPlayerState>(PlayerArray[i])->addToCompletedRecipeUI(recipeCardOnTableFileData[index].path, parameters[0], parameters[1], parameters[2], false, TEXT(""));
		}

		removeRecipeCardInGame(index);
		addRecipeCardInGame(index);
	}

	for (int i = 0; i < PlayerArray.Num(); i += 1)
	{
		Cast<AMyPlayerState>(PlayerArray[i])->setReaction(false);
		Cast<AMyPlayerState>(PlayerArray[i])->destroyReactionTimer();
		Cast<AFamilyReunionDinner2Character>(PlayerArray[i]->GetPawn())->clearUI();
	}

	GetWorldTimerManager().UnPauseTimer(turnTimer);
}

void AMyGameStateBase::reactionTimeUpWithRemoveItemInPot(int index, int potIndex)
{
	GetWorldTimerManager().ClearTimer(actionTimer);

	getRemoveItemActionResult(index, potIndex);

	for (int i = 0; i < PlayerArray.Num(); i += 1)
	{
		if (succussAction)
		{
			FString message = TEXT("Remove Item in Pot Action Completed");
			Cast<AMyPlayerState>(PlayerArray[i])->showWorldMessage(message, FVector(0, 1, 0));
		}
		else
		{
			FString message = TEXT("Remove Item in Pot Action Rejected");
			Cast<AMyPlayerState>(PlayerArray[i])->showWorldMessage(message, FVector(1, 0, 0));
		}
	}
}
void AMyGameStateBase::getRemoveItemActionResult(int index, int potIndex) 
{
	GetWorldTimerManager().ClearTimer(actionTimer);

	if (succussAction)
	{
		if (index <= recipeCardOnTableFileData[potIndex].addedCookingCards.Num() - 1) 
		{
			recipeCardOnTableFileData[potIndex].addedCookingCards.RemoveAt(index);
		}
		else 
		{
			int trueIndex = index - recipeCardOnTableFileData[potIndex].addedCookingCards.Num();
			recipeCardOnTableFileData[potIndex].addedIngredientCards.RemoveAt(trueIndex);
		}

		for (int i = 0; i < PlayerArray.Num(); i += 1)
		{
			Cast<AMyPlayerState>(PlayerArray[i])->removePotItem(index, potIndex);
		}
	}

	for (int i = 0; i < PlayerArray.Num(); i += 1)
	{
		Cast<AMyPlayerState>(PlayerArray[i])->setReaction(false);
		Cast<AMyPlayerState>(PlayerArray[i])->destroyReactionTimer();
		Cast<AFamilyReunionDinner2Character>(GetWorld()->GetGameState()->PlayerArray[i]->GetPawn())->setObservingPotItemIndex(-1);
		Cast<AFamilyReunionDinner2Character>(PlayerArray[i]->GetPawn())->clearUI();
	}

	GetWorldTimerManager().UnPauseTimer(turnTimer);
}

bool AMyGameStateBase::checkRecipeSuccuss(FRecipeCardStruct toCheck, FString& failedReason) 
{
	bool isok = true;

	TArray<FString> flavorRange;
	toCheck.flavorRange.ParseIntoArray(flavorRange, TEXT("-"));

	TArray<FString> heatRange;
	toCheck.heatRange.ParseIntoArray(heatRange, TEXT("-"));

	if (flavorRange.Num() != 2)
	{
		FString max = flavorRange[0];
		flavorRange.Add(max);
	}

	if (heatRange.Num() != 2)
	{
		FString max = heatRange[0];
		heatRange.Add(max);
	}

	//special effect
	for (int i = 0; i < monsterPreferenceInGame.Num(); i += 1)
	{
		if (monsterPreferenceInGame[i].name == "Mouth Ulcer")
		{
			if (toCheck.type == "Spicy" || toCheck.type == "Sour") 
			{
				flavorRange[0] = FString::FromInt(FCString::Atoi(*flavorRange[0]) - 1);
				flavorRange[1] = FString::FromInt(FCString::Atoi(*flavorRange[1]) - 1);
			}
		}
		else if (monsterPreferenceInGame[i].name == "Spitfire Mouth")
		{
			heatRange[0] = FString::FromInt(FCString::Atoi(*heatRange[0]) + 1);
			heatRange[1] = FString::FromInt(FCString::Atoi(*heatRange[1]) + 1);
		}
		else if (monsterPreferenceInGame[i].name == "Raw is healthy")
		{
			heatRange[0] = FString::FromInt(FCString::Atoi(*heatRange[0]) - 1);
			heatRange[1] = FString::FromInt(FCString::Atoi(*heatRange[1]) - 1);
		}
		else if (monsterPreferenceInGame[i].name == "Go on a diet")
		{
			if (toCheck.type == "Salty" || toCheck.type == "Sweet")
			{
				flavorRange[0] = FString::FromInt(FCString::Atoi(*flavorRange[0]) - 1);
				flavorRange[1] = FString::FromInt(FCString::Atoi(*flavorRange[1]) - 1);
			}
		}
	}

	//check basic element
	TArray<int> parameter = Cast<AMyPlayerState>(PlayerArray[0])->calculateParameter(toCheck);
	
	if (parameter[0] < FCString::Atoi(*flavorRange[0]))
	{
		failedReason.Append(TEXT("! Flavor Too Low !\n"));
		isok = false;
	}

	if (parameter[0] > FCString::Atoi(*flavorRange[0]))
	{
		failedReason.Append(TEXT("! Flavor Too High !\n"));
		isok = false;
	}

	if (parameter[1] < FCString::Atoi(*heatRange[0]))
	{
		failedReason.Append(TEXT("! Heat Too Low !\n"));
		isok = false;
	}

	if (parameter[1] > FCString::Atoi(*heatRange[1]))
	{
		failedReason.Append(TEXT("! Heat Too High !\n"));
		isok = false;
	}

	//attribute
	for (int i = 0; i < monsterPreferenceInGame.Num(); i += 1) 
	{
		if (monsterPreferenceInGame[i].name == "Vegetarian")
		{
			if (toCheck.type == "Meat") 
			{
				failedReason.Append(TEXT("! Contain Meat !\n"));
				isok = false;
			}
		}
		else if (monsterPreferenceInGame[i].name == "Seafood_Allergy")
		{
			if (toCheck.type == "Seafood")
			{
				failedReason.Append("! Contain Seafood !\n");
				isok = false;
			}
		}

	}

	return isok;
}

bool AMyGameStateBase::checkPreferenceSuccuss(FMonsterPreferenceStruct toCheck, FString& failedReason)
{
	bool isok = true;

	if (toCheck.name == "ÐÁµ³[¤«¤é¤È¤¦]") 
	{
		if (!hasPot(TEXT("Spicy"))) 
		{
			failedReason.Append(TEXT("! No Spicy Dish !\n"));
			isok = false;
		}
	}
	else if (toCheck.name == "Lemon Squeezer") 
	{
		if (!hasPot(TEXT("Sour")))
		{
			failedReason.Append(TEXT("! No Sour Dish !\n"));
			isok = false;
		}
	}
	else if (toCheck.name == "¸Êµ³[¤¢¤Þ¤È¤¦]")
	{
		if (!hasPot(TEXT("Sweet")))
		{
			failedReason.Append(TEXT("! No Sweet Dish !\n"));
			isok = false;
		}
	}
	else if (toCheck.name == "Supertaster !\n")
	{
		if (!hasPot(TEXT("Salty")))
		{
			failedReason.Append(TEXT("! No Salty Dish"));
			isok = false;
		}
	}

	return isok;
}

bool AMyGameStateBase::hasPot(FString type)
{
	for (int i = 0; i < completedDishFileData.Num(); i += 1) 
	{
		if (completedDishFileData[i].type == type) 
		{
			return true;
		}
	}

	return false;
}