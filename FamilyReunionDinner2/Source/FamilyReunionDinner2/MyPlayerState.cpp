// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerState.h"
#include "FamilyReunionDinner2Character.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/KismetMathLibrary.h"

void AMyPlayerState::BeginPlay()
{
	Super::BeginPlay();

	if (GetLocalRole() != 3 && GetOwner() != NULL)
	{
		this->requestUserID();
	}
}

void AMyPlayerState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMyPlayerState, cookingCards);
}

void AMyPlayerState::createIngredientCard_Implementation(FIngredientCardStruct data, int positionIndex)
{
	TSubclassOf<AIngredientCard> recipeCard = LoadClass<AIngredientCard>(nullptr, TEXT("Blueprint'/Game/FirstPersonCPP/Blueprints/MyIngredientCard.MyIngredientCard_C'"));
	FRotator rotation = FRotator(0, Cast<AFamilyReunionDinner2Character>(GetPawn())->originalZRotation, 0);
	FVector direction = UKismetMathLibrary::GetForwardVector(rotation);
	direction.Normalize();

	FVector position;
	FVector offset;

	if (positionIndex <= 1) 
	{
		position = direction * 80 + GetPawn()->GetActorLocation();
		offset = direction * (-12.5 + positionIndex * 25.0);
	}
	else 
	{
		position = direction * 50 + GetPawn()->GetActorLocation();
		offset = direction * (-12.5 + (positionIndex - 2) * 25.0);
	}

	AIngredientCard* card = GetWorld()->SpawnActor<AIngredientCard>(recipeCard, position, rotation, FActorSpawnParameters());
	card->AddActorWorldOffset(offset.RotateAngleAxis(90, FVector(0, 0, 1)));
	card->AddActorWorldRotation(FRotator(0, 90, 0));
	card->data = data;
	card->assignInfo();

	ingredientCards.Insert(card, positionIndex);
}

void AMyPlayerState::createRecipeCard_Implementation(FRecipeCardStruct data, int positionIndex)
{
	TSubclassOf<ARecipeCard> recipeCard = LoadClass<ARecipeCard>(nullptr, TEXT("Blueprint'/Game/FirstPersonCPP/Blueprints/MyRecipeCard.MyRecipeCard_C'"));
	FRotator rotation = FRotator(0, Cast<AFamilyReunionDinner2Character>(GetPawn())->originalZRotation, 0);
	
	FVector position;
	FVector direction;
	
	if (positionIndex == 0) 
	{
		direction = rotation.RotateVector(FVector(20, -11.5, 0));
		direction.Normalize();

		position = direction * 90 + GetPawn()->GetActorLocation();
	}
	else if (positionIndex == 1)
	{
		direction = rotation.RotateVector(FVector(20, -5, 0));
		direction.Normalize();

		position = direction * 120 + GetPawn()->GetActorLocation();
	}
	else if (positionIndex == 2)
	{
		direction = rotation.RotateVector(FVector(20, 5, 0));
		direction.Normalize();

		position = direction * 120 + GetPawn()->GetActorLocation();
	}
	else 
	{
		direction = rotation.RotateVector(FVector(20, 11.5, 0));
		direction.Normalize();

		position = direction * 90 + GetPawn()->GetActorLocation();
	}

	position.Z = -35.049339f;

	FVector cardOffset = position - UKismetMathLibrary::GetForwardVector(rotation) * 15;
	cardOffset.Z = -12.5;

	ARecipeCard* card = GetWorld()->SpawnActor<ARecipeCard>(recipeCard, cardOffset, rotation.Add(0, 90, 90), FActorSpawnParameters());
	card->SetActorScale3D(FVector(1, 1, 1));
	card->data = data;
	card->assignInfo();

	recipeCards.Insert(card, positionIndex);

	TSubclassOf<APot> recipePot = LoadClass<APot>(nullptr, TEXT("Blueprint'/Game/FirstPersonCPP/Blueprints/MyPot.MyPot_C'"));
	APot* pot = GetWorld()->SpawnActor<APot>(recipePot, position, rotation.Add(0, 0, -90), FActorSpawnParameters());
	pot->associatedCard = card;
	pot->setPotBonusPoint(FCString::Atoi(*card->data.point));

	recipePots.Insert(pot, positionIndex);
	//setCardRotationBasedOnPlayerLocation(pot);
}

void AMyPlayerState::addIngredientCardToPot_Implementation(FIngredientCardStruct data, int index) 
{
	recipeCards[index]->data.addedIngredientCards.Add(data);
	recipePots[index]->setPotBonusPoint(calculateBonusPoint(recipeCards[index]->data));
}

void AMyPlayerState::addCookingCardToPot_Implementation(FCookingCardStruct data, int index)
{
	recipeCards[index]->data.addedCookingCards.Add(data);

	TArray<int> parameters = calculateParameter(recipeCards[index]->data);

	if (data.type == "Heat") 
	{
		recipePots[index]->setPotHeatDegree(parameters[1]);
	}
	else 
	{
		recipePots[index]->setPotFlavorDegree(parameters[0]);
	}
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

	recipePots[index]->Destroy();
	recipePots.RemoveAt(index);
}

void AMyPlayerState::removePotItem_Implementation(int index, int potIndex)
{
	if (index <= recipeCards[potIndex]->data.addedCookingCards.Num() - 1)
	{
		recipeCards[potIndex]->data.addedCookingCards.RemoveAt(index);
	}
	else
	{
		int trueIndex = index - recipeCards[potIndex]->data.addedCookingCards.Num();
		recipeCards[potIndex]->data.addedIngredientCards.RemoveAt(trueIndex);
	}

	TArray<int> parameters = calculateParameter(recipeCards[potIndex]->data);
	recipePots[potIndex]->setPotFlavorDegree(parameters[0]);
	recipePots[potIndex]->setPotHeatDegree(parameters[1]);
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

void AMyPlayerState::setInTurnPlayerName_Implementation(const FString& name)
{
	inTurnPlayerName = name;
}

void AMyPlayerState::setReaction_Implementation(bool ifReaction)
{
	inReaction = ifReaction;
}

void AMyPlayerState::setReactionComplete_Implementation(bool ifReactionComplete)
{
	reactionComplete = ifReactionComplete;
}

void AMyPlayerState::setPreReaction_Implementation(bool ifPreReaction)
{
	preReaction = ifPreReaction;
}

void AMyPlayerState::setActionPoint_Implementation(int curPoint) 
{
	actionPoint = curPoint;
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

	if (inTurn) 
	{
		Cast<AFamilyReunionDinner2Character>(GetPawn())->MainUI->setWaitingText("Your Turn");
	}
	else
	{
		FString nameText = inTurnPlayerName;
		nameText.Append(TEXT("'s Turn"));
		Cast<AFamilyReunionDinner2Character>(GetPawn())->MainUI->setWaitingText(nameText);
	}
}

void AMyPlayerState::showWorldMessage_Implementation(const FString& text, const FVector& color)
{
	Cast<AFamilyReunionDinner2Character>(GetPawn())->MainUI->playWorldMessageText(text, color);
}

void AMyPlayerState::requestUserID_Implementation()
{
	FamilyReunionDinner2PlayerID = FString::FromInt(FMath::RandRange(10000000, 99999999));
	sendUserID(FamilyReunionDinner2PlayerID);
}

void AMyPlayerState::sendUserID_Implementation(const FString& id)
{
	FamilyReunionDinner2PlayerID = id;
}

void AMyPlayerState::addToCompletedRecipeUI_Implementation(const FString& path, int flavor, int heat, int point, bool failed, const FString& failedReason)
{
	Cast<AFamilyReunionDinner2Character>(GetPawn())->MainUI->addToCompletedRecipeUI(path, flavor, heat, point, failed, failedReason);
}

TArray<int> AMyPlayerState::calculateParameter(FRecipeCardStruct data)
{
	TArray<int> parameters;

	int flavor = 0;
	int size = 0;

	for (int i = 0; i < data.addedCookingCards.Num(); i += 1)
	{
		if (data.addedCookingCards[i].type != "Heat")
		{
			TArray<FString> recipeTypeArray;
			data.type.ParseIntoArray(recipeTypeArray, TEXT("/"));

			if (recipeTypeArray.Contains(data.addedCookingCards[i].type))
			{
				flavor += FCString::Atoi(*data.addedCookingCards[i].degree);
			}
		}
	}

	for (int i = 0; i < data.addedIngredientCards.Num(); i += 1)
	{
		size += FCString::Atoi(*data.addedIngredientCards[i].size);
	}

	parameters.Add(flavor);
	parameters.Add(calculateHeat(data));
	parameters.Add(calculateBonusPoint(data));
	parameters.Add(size);

	return parameters;
}

int AMyPlayerState::calculateBonusPoint(FRecipeCardStruct data)
{
	int total = FCString::Atoi(*data.point);

	for (int i = 0; i < data.addedIngredientCards.Num(); i += 1)
	{
		FIngredientCardStruct curData = data.addedIngredientCards[i];

		if (curData.name == "Chicken")
		{
			total += 3;
		}
		else if (curData.name == "Mutton")
		{
			if (potHasType("Seafoods", data)) 
			{
				total += 5;
			}
			else 
			{
				total += 3;
			}
		}
		else if (curData.name == "Pork")
		{
			if (curData.type == "Salty" || curData.type == "Spicy")
			{
				total += 6;
			}
			else 
			{
				total += 2;
			}
		}
		else if (curData.name == "Chicken Beast")
		{
			total += 1;
		}
		else if (curData.name == "Chicken Wings")
		{
			total += 2;
		}
		else if (curData.name == "Tomatoes")
		{
			total += 1;
		}
		else if (curData.name == "Potatoes")
		{
			total += 2;
		}
		else if (curData.name == "Cabbage")
		{
			if (potHasType("Meat", data))
			{
				total += 3;
			}
			else
			{
				total += 1;
			}
		}
		else if (curData.name == "Onion")
		{
			total += 2;
		}
		else if (curData.name == "Broccoli")
		{
			total += 3;
		}
		else if (curData.name == "Cauliflower")
		{
			total += 3;
		}
		else if (curData.name == "Shrimps")
		{
			total += 4;
		}
		else if (curData.name == "Seafish")
		{
			if (potHasType("Seafoods", data))
			{
				total += 1;
			}
			else if (potHasType("Vegetables", data))
			{
				total += 2;
			}
			else 
			{
				total += 4;
			}
		}
		else if (curData.name == "Lobster")
		{
			if (calculateHeat(data) < 4)
			{
				total += 0;
			}
			else 
			{
				total += 5;
			}
		}
		else if (curData.name == "Crab")
		{
			if (calculateHeat(data) < 3)
			{
				total += 0;
			}
			else
			{
				total += 4;
			}
		}
		else if (curData.name == "Clams") 
		{
			if (calculateHeat(data) < 3)
			{
				total += 1;
			}
			else
			{
				total += 3;
			}
		}
		else if (curData.name == "Eggplants")
		{
			if (calculateHeat(data) <= 3)
			{
				total += 2;
			}
			else
			{
				total += 0;
			}
		}
	}

	return total;
}

bool AMyPlayerState::potHasType(FString type, FRecipeCardStruct data)
{
	for (int i = 0; i < data.addedIngredientCards.Num(); i += 1)
	{
		if (data.addedIngredientCards[i].type == type)
		{
			return true;
		}
	}

	for (int i = 0; i < data.addedCookingCards.Num(); i += 1)
	{
		if (data.addedCookingCards[i].type == type)
		{
			return true;
		}
	}

	return false;
}

int AMyPlayerState::calculateHeat(FRecipeCardStruct data) 
{
	int heat = 0;

	for (int i = 0; i < data.addedCookingCards.Num(); i += 1)
	{
		if (data.addedCookingCards[i].type == "Heat")
		{
			heat += FCString::Atoi(*data.addedCookingCards[i].degree);
		}
	}

	return heat;
}

void AMyPlayerState::sendGameOverData_Implementation(const FString& result, const TArray<FCompletedRecipeInfo>& recipeData, const TArray<FCompletedPreferenceInfo>& preferenceData, const TArray<FString>& playersIDData)
{
	gameResult = result;
	playersID = playersIDData;
	preferenceResult = preferenceData;
	recipeResult = recipeData;

	inReaction = true;
	Cast<AFamilyReunionDinner2Character>(GetPawn())->MainUI->drawGameResultOnScreen();
}

void AMyPlayerState::setRound_Implementation(int curRound, int maxRound)
{
	Cast<AFamilyReunionDinner2Character>(GetPawn())->MainUI->setRoundText(curRound, maxRound);
}