// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerState.h"
#include "FamilyReunionDinner2Character.h"
#include "Net/UnrealNetwork.h"

void AMyPlayerState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMyPlayerState, cookingCards);
}

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

void AMyPlayerState::setMonsterPreferenceUI_Implementation(const FString& path)
{
	Cast<AFamilyReunionDinner2Character>(GetPawn())->MainUI->setMonsterPreferencePic(path);
}

void AMyPlayerState::setTurn_Implementation(bool ifTurn)
{
	inTurn = ifTurn;

	if (inTurn) 
	{
		hintShowed = false;
	}
}

void AMyPlayerState::setReaction_Implementation(bool ifReaction)
{
	inReaction = ifReaction;
}


void AMyPlayerState::setReactionComplete_Implementation(bool ifReactionComplete)
{
	reactionComplete = ifReactionComplete;
}

void AMyPlayerState::turnTimerRun()
{
	turnTimeRemaining -= GetWorldTimerManager().GetTimerRate(turnTimer);
	Cast<AFamilyReunionDinner2Character>(GetPawn())->MainUI->setTurnTimerUI(turnTimeRemaining, 90);
}

void AMyPlayerState::reactionTimerRun()
{
	actionTimerRemaining -= GetWorldTimerManager().GetTimerRate(reactionTimer);
	Cast<AFamilyReunionDinner2Character>(GetPawn())->MainUI->setTurnTimerUI(actionTimerRemaining, 10);
}

void AMyPlayerState::activeTurnTimer_Implementation()
{
	turnTimeRemaining = 90;
	GetWorldTimerManager().ClearTimer(turnTimer);

	GetWorldTimerManager().SetTimer(turnTimer, this, &AMyPlayerState::turnTimerRun, 0.0167f, true, 0);
}

void AMyPlayerState::activeReactionTimer_Implementation()
{
	GetWorldTimerManager().PauseTimer(turnTimer);

	actionTimerRemaining = 10;
	GetWorldTimerManager().ClearTimer(reactionTimer);

	if (reactionComplete) 
	{
		Cast<AFamilyReunionDinner2Character>(GetPawn())->MainUI->setWaitingText(TEXT("Waiting other' Reaction..."));
	}
	else 
	{
		Cast<AFamilyReunionDinner2Character>(GetPawn())->MainUI->setWaitingText(TEXT("Choose If Using Reaction"));
	}
	
	GetWorldTimerManager().SetTimer(reactionTimer, this, &AMyPlayerState::reactionTimerRun, 0.0167f, true, 0);
}

void AMyPlayerState::destroyReactionTimer_Implementation() 
{
	GetWorldTimerManager().ClearTimer(reactionTimer);
	GetWorldTimerManager().UnPauseTimer(turnTimer);

	Cast<AFamilyReunionDinner2Character>(GetPawn())->MainUI->setWaitingText(inTurnPlayerName.Append(TEXT("'s Turn")));
}