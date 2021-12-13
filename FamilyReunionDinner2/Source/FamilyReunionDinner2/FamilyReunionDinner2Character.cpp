// Copyright Epic Games, Inc. All Rights Reserved.

#include "FamilyReunionDinner2Character.h"
#include "FamilyReunionDinner2Projectile.h"
#include "MyGameStateBase.h"
#include "MyPlayerState.h"
#include "CardBorderActor.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/InputSettings.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "MotionControllerComponent.h"
#include "XRMotionControllerBase.h" // for FXRMotionControllerBase::RightHandSourceId

DEFINE_LOG_CATEGORY_STATIC(LogFPChar, Warning, All);

//////////////////////////////////////////////////////////////////////////
// AFamilyReunionDinner2Character

AFamilyReunionDinner2Character::AFamilyReunionDinner2Character()
{
	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;
	
	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-39.56f, 1.75f, 64.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;
}

void AFamilyReunionDinner2Character::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();
}

void AFamilyReunionDinner2Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetPlayerState() == NULL) 
	{
		return;
	}

	if (Cast<AMyPlayerState>(GetPlayerState())->inDragging) 
	{
		FVector startPos = holdingItem->GetActorLocation();

		FHitResult hit(ForceInit);
		UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, true, hit);

		//init arrow
		if (toPotArrowHelper == NULL) 
		{
			TSubclassOf<AToPotArrow> toPotArrowClass = LoadClass<AToPotArrow>(nullptr, TEXT("Blueprint'/Game/FirstPersonCPP/Blueprints/MyToPotArrow.MyToPotArrow_C'"));
			toPotArrowHelper = GetWorld()->SpawnActor<AToPotArrow>(toPotArrowClass, FVector(0, 0, 0), FRotator(0, 0, 0), FActorSpawnParameters());
		}

		toPotArrowHelper->arrowInit(startPos, hit.Location);

		AActor* curTarget = pickFromEye();

		if (curTarget != NULL && curTarget->Tags.Num() != 0) 
		{
			if (curTarget->Tags[0] == TEXT("pot") || curTarget->Tags[0] == TEXT("potCard"))
			{
				//show UI of pot
				ARecipeCard* card = NULL;

				if (curTarget->Tags[0] == TEXT("pot"))
				{
					card = Cast<ARecipeCard>(Cast<APot>(curTarget)->associatedCard);
				}
				else
				{
					card = Cast<ARecipeCard>(curTarget);
				}

				FRotator rotation = FRotator(0, originalZRotation, 0);
				FVector direction = UKismetMathLibrary::GetForwardVector(rotation);

				if (potBorder == NULL)
				{
					potBorder = GetWorld()->SpawnActor<APotBorder>(APotBorder::StaticClass(), curTarget->GetActorLocation(), rotation.Add(0, -90, 0), FActorSpawnParameters());
				}
				
				potBorder->SetActorLocation(curTarget->GetActorLocation() - direction * 7);
			}
		}
		else 
		{
			if (potBorder != NULL) 
			{
				potBorder->Destroy();
				potBorder = NULL;
			}
		}
	}
}

void AFamilyReunionDinner2Character::setLocationByIndex(int index)
{
	float z = -35.049309f;

	switch (index)
	{
	case 1:
		SetActorLocation(FVector(-322.28421, -164.444977, z));
		setZRotation(90);

		originalZRotation = 90;
		setZRotationValue(90);

		break;

	case 2:
		SetActorLocation(FVector(-426.462799, 1.346313, z));
		setZRotation(-112.874084 + 90);

		originalZRotation = -112.874084 + 90;
		setZRotationValue(-112.874084 + 90);

		break;

	case 3:
		SetActorLocation(FVector(-215.042343, 1.346313, z));
		setZRotation(112.958496 + 90);

		originalZRotation = 112.958496 + 90;
		setZRotationValue(112.958496 + 90);

		break;

	case 4:
		SetActorLocation(FVector(-420.9422, -106.940002, z));
		setZRotation(-54.066353 + 90);

		originalZRotation = -54.066353 + 90;
		setZRotationValue(-54.066353 + 90);

		break;

	case 5:
		SetActorLocation(FVector(-224.810547, -104.113152, z));
		setZRotation(57.664375 + 90);

		originalZRotation = 57.664375 + 90;
		setZRotationValue(57.664375 + 90);

		break;
	}
}

void AFamilyReunionDinner2Character::setZRotation_Implementation(float val)
{
	AddControllerYawInput(val / 2.5f);
}

void AFamilyReunionDinner2Character::setZRotationValue_Implementation(float val)
{
	originalZRotation = val;
}

//////////////////////////////////////////////////////////////////////////
// Input

void AFamilyReunionDinner2Character::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	check(PlayerInputComponent);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AFamilyReunionDinner2Character::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AFamilyReunionDinner2Character::LookUpAtRate);

	PlayerInputComponent->BindAction("TestAction", IE_Released, this, &AFamilyReunionDinner2Character::useSpecialAction);
	PlayerInputComponent->BindAction("Picking", IE_Pressed, this, &AFamilyReunionDinner2Character::pickingItem);
	PlayerInputComponent->BindAction("Picking", IE_Released, this, &AFamilyReunionDinner2Character::releaseItem);
}

void AFamilyReunionDinner2Character::useSpecialAction() 
{
	//UE_LOG(LogTemp, Warning, TEXT("name is %s"), *test[3]->GetStringField("Name"));
	//startGame();
}

void AFamilyReunionDinner2Character::endTurn_Implementation() 
{
	Cast<AMyGameStateBase>(GetWorld()->GetGameState())->nextTurn();
}

AActor* AFamilyReunionDinner2Character::pickFromEye()
{
	FHitResult hit(ForceInit);
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, true, hit);
	
	return hit.GetActor();
}

void AFamilyReunionDinner2Character::releaseItem() 
{
	puttingItem();

	holdingItem = NULL;
}

void AFamilyReunionDinner2Character::pickingItem() 
{
	if (Cast<AMyPlayerState>(GetPlayerState())->inReaction) 
	{
		return;
	}

	MainUI->clearUI();

	if (UIOn)
	{
		UIOn = false;

		return;
	}

	AActor* curTarget = pickFromEye();

	if (curTarget != NULL && curTarget->Tags.Num() != 0)
	{
		if (curTarget->Tags[0] == TEXT("card"))
		{
			if (curTarget->IsA(ACardBorderActor::StaticClass())) 
			{
				curTarget = Cast<ACardBorderActor>(curTarget)->parent;
			}

			//other players' cooking card 
			if (curTarget->GetLocalRole() != 3 && curTarget->IsA(ACookingCard::StaticClass()) && !Cast<AMyPlayerState>(GetPlayerState())->cookingCards.Contains(curTarget))
			{
				//show give a hint panal
				UIOn = true;
				requestCertainHandInfo(Cast<ACookingCard>(curTarget));
			}
			else 
			{
				GetWorld()->GetGameViewport()->GetMousePosition(mousePos);

				mouseDown = true;
				GetWorldTimerManager().ClearTimer(previewTimer);
				FTimerDelegate TimerDel = FTimerDelegate::CreateUObject(this, &AFamilyReunionDinner2Character::showItemPreview, curTarget);
				GetWorldTimerManager().SetTimer(previewTimer, TimerDel, 0.0167, true, 0);
			}
		}
		else if (curTarget->Tags[0] == TEXT("pot") || curTarget->Tags[0] == TEXT("potCard")) 
		{
			//show UI of pot
			ARecipeCard* card = NULL;
			UIOn = true;

			if (curTarget->Tags[0] == TEXT("pot"))
			{
				card = Cast<ARecipeCard>(Cast<APot>(curTarget)->associatedCard);
			}
			else 
			{
				card = Cast<ARecipeCard>(curTarget);
			}

			for (int i = 0; i < Cast<AMyPlayerState>(GetPlayerState())->recipeCards.Num(); i += 1)
			{
				if (card->data.name == Cast<AMyPlayerState>(GetPlayerState())->recipeCards[i]->data.name)
				{
					observingPotIndex = i;

					break;
				}
			}

			sendReactionRequest_Implementation(observingPotIndex, card->data.path, TEXT("Finish Dish"), FVector(0, 1, 0));
		}
		else if (curTarget->Tags[0] == "CharacterPlaceHolder") 
		{
			for (int i = 0; i < Cast<AMyPlayerState>(GetPlayerState())->characterPlaceHolders.Num(); i += 1) 
			{
				if (curTarget == Cast<AMyPlayerState>(GetPlayerState())->characterPlaceHolders[i]) 
				{
					requestCertainHandInfoWithIndex(i);
					UIOn = true;

					break;
				}
			}
		}
		else if (curTarget->Tags[0] == "MonsterPlaceHolder") 
		{
			requestMonsterInfo();

			UIOn = true;
		}
	}
}

void AFamilyReunionDinner2Character::puttingItem()
{
	if (Cast<AMyPlayerState>(GetPlayerState())->inReaction)
	{
		return;
	}

	mouseDown = false;

	if (Cast<AMyPlayerState>(GetPlayerState())->inDragging)
	{
		Cast<AMyPlayerState>(GetPlayerState())->inDragging = false;
		clearUI_Implementation();
	}

	if (toPotArrowHelper != NULL)
	{
		toPotArrowHelper->Destroy();
		toPotArrowHelper = NULL;
	}

	if (holdingItem == NULL) 
	{
		return;
	}

	AActor* curTarget = pickFromEye();

	if (curTarget != NULL && curTarget->Tags.Num() != 0)
	{
		if (curTarget->Tags[0] == TEXT("pot") || curTarget->Tags[0] == TEXT("potCard")) 
		{
			if (potBorder != NULL)
			{
				potBorder->Destroy();
				potBorder = NULL;
			}

			ARecipeCard* card = NULL;

			if (curTarget->Tags[0] == TEXT("pot"))
			{
				card = Cast<ARecipeCard>(Cast<APot>(curTarget)->associatedCard);
			}
			else
			{
				card = Cast<ARecipeCard>(curTarget);
			}

			//find index of recipe card
			TArray<ARecipeCard*> cardsArray = Cast<AMyPlayerState>(GetPlayerState())->recipeCards;

			for (int i = 0; i < 4; i += 1)
			{
				if (cardsArray[i]->data.name == card->data.name) 
				{
					if (holdingItem->IsA(ACookingCard::StaticClass())) 
					{
						addCookingCardToPot(Cast<ACookingCard>(holdingItem), i);
					}
					else 
					{
						TArray<AIngredientCard*> cardsArrayInHolding = Cast<AMyPlayerState>(GetPlayerState())->ingredientCards;

						for (int j = 0; j < 4; j++)
						{
							if (cardsArrayInHolding[j]->data.name == Cast<AIngredientCard>(holdingItem)->data.name) 
							{
								addIngredientCardToPot(j, i);

								break;
							}
						}
					}

					break;
				}
			}
		}
	}
}

void AFamilyReunionDinner2Character::showItemPreview(AActor* item)
{
	FVector2D curMousePos;
	GetWorld()->GetGameViewport()->GetMousePosition(curMousePos);

	if (mousePos != curMousePos) 
	{
		if (Cast<AMyPlayerState>(GetPlayerState())->inTurn)
		{
			holdingItem = item;
			Cast<AMyPlayerState>(GetPlayerState())->inDragging = true;
		}

		//show highlight of the position that can put the card
		//draw Arrow?

		GetWorldTimerManager().ClearTimer(previewTimer);

		MainUI->clearUI();
		previewTimerCount = 0;

		return;
	}

	if (!mouseDown) 
	{
		GetWorldTimerManager().ClearTimer(previewTimer);

		previewTimerCount = 0;
		MainUI->clearUI();

		return;
	}

	if (previewTimerCount >= 0.5f) 
	{
		FString picPath;

		if (item->IsA(ACookingCard::StaticClass()))
		{
			picPath = Cast<ACookingCard>(item)->data.path;
		}
		else
		{
			picPath = Cast<AIngredientCard>(item)->data.path;
		}

		FVector2D screenSize;
		GetWorld()->GetGameViewport()->GetViewportSize(screenSize);

		FVector2D canvasPreMultiplier(screenSize.X / 1920.0f, screenSize.Y / 1080.0f);
		float targetX = 0;
		float targetY = 0;

		if (mousePos.X < 375 * canvasPreMultiplier.X)
		{
			targetX = mousePos.X + 50 * canvasPreMultiplier.X;
		}
		else 
		{
			targetX = mousePos.X - 375 * canvasPreMultiplier.X;
		}

		targetY = FMath::Clamp(mousePos.Y - 315.0f * canvasPreMultiplier.Y, 0.0f, screenSize.Y - 630.0f * canvasPreMultiplier.Y);

		MainUI->draggingCardPreviewOnScreen(picPath, targetX / screenSize.X * 1920.0f, targetY / screenSize.Y * 1080.0f);
		previewTimerCount = -8753;

		return;
	}

	previewTimerCount += GetWorldTimerManager().GetTimerRate(previewTimer);
}

void AFamilyReunionDinner2Character::giveTypeHint_Implementation(ACookingCard* card)
{
	for (int i = 0; i < GetWorld()->GetGameState()->PlayerArray.Num(); i += 1)
	{
		TArray<ACookingCard*> curCards = Cast<AMyPlayerState>(GetWorld()->GetGameState()->PlayerArray[i])->cookingCards;

		for (int j = 0; j < curCards.Num(); j += 1)
		{
			if (curCards[j]->GetUniqueID() == card->GetUniqueID())
			{
				for (int k = 0; k < curCards.Num(); k += 1) 
				{
					if (curCards[k]->data.type == card->data.type && !curCards[k]->data.typeHinted)
					{
						curCards[k]->data.typeHinted = true;

						TSubclassOf<ACardBorderActor> cardBorderClass = LoadClass<ACardBorderActor>(nullptr, TEXT("Blueprint'/Game/FirstPersonCPP/Blueprints/BP_CardBorder.BP_CardBorder_C'"));
						ACardBorderActor* cardBorder = GetWorld()->SpawnActor<ACardBorderActor>(cardBorderClass, FVector(0, 0, 0), FRotator(0, 0, 0), FActorSpawnParameters());

						cardBorder->parent = curCards[k];
						curCards[k]->border = cardBorder;

						FLinearColor color;

						if (curCards[k]->data.type == "Spicy")
						{
							color = FLinearColor::Red;
						}
						else if (curCards[k]->data.type == "Sour")
						{
							color = FLinearColor::Yellow;
						}
						else if (curCards[k]->data.type == "Sweet")
						{
							color.R = 1.f;
							color.G = 0.412f;
							color.B = 0.706f;
						}
						else if (curCards[k]->data.type == "Salty")
						{
							color = FLinearColor::Blue;
						}
						else if (curCards[k]->data.type == "Heat")
						{
							color = FLinearColor::White;
						}

						cardBorder->setBorderColor(color.R, color.G, color.B, 1);
					}
				}

				for (int k = 0; k < Cast<AMyGameStateBase>(GetWorld()->GetGameState())->PlayerArray.Num(); k += 1)
				{
					FString message = "Player ";
					message.Append(Cast<AMyPlayerState>(GetPlayerState())->FamilyReunionDinner2PlayerID);
					message.Append(" Gived a Type Hint of ");
					message.Append(card->data.type);
					message.Append(" to ");
					message.Append(Cast<AMyPlayerState>(Cast<AMyGameStateBase>(GetWorld()->GetGameState())->PlayerArray[i])->FamilyReunionDinner2PlayerID);

					Cast<AMyPlayerState>(Cast<AMyGameStateBase>(GetWorld()->GetGameState())->PlayerArray[k])->showWorldMessage(message, FVector(0, 1, 0));
				}

				clearUI();
				setHintShowed(true);
				Cast<AMyGameStateBase>(GetWorld()->GetGameState())->nextTurn();

				return;
			}
		}
	}
}

void AFamilyReunionDinner2Character::giveDegreeHint_Implementation(ACookingCard* card)
{
	for (int i = 0; i < GetWorld()->GetGameState()->PlayerArray.Num(); i += 1)
	{
		TArray<ACookingCard*> curCards = Cast<AMyPlayerState>(GetWorld()->GetGameState()->PlayerArray[i])->cookingCards;

		for (int j = 0; j < curCards.Num(); j += 1)
		{
			if (curCards[j]->GetUniqueID() == card->GetUniqueID())
			{
				for (int k = 0; k < curCards.Num(); k += 1)
				{
					if (curCards[k]->data.degree == card->data.degree && !curCards[k]->data.degreeHinted)
					{
						curCards[k]->data.degreeHinted = true;
						curCards[k]->changeDegreeHintStatus(true);
					}
				}

				for (int k = 0; k < Cast<AMyGameStateBase>(GetWorld()->GetGameState())->PlayerArray.Num(); k += 1)
				{
					FString message = "Player ";
					message.Append(Cast<AMyPlayerState>(GetPlayerState())->FamilyReunionDinner2PlayerID);
					message.Append(" Gived a Degree Hint of +");
					message.Append(card->data.degree);
					message.Append(" to ");
					message.Append(Cast<AMyPlayerState>(Cast<AMyGameStateBase>(GetWorld()->GetGameState())->PlayerArray[i])->FamilyReunionDinner2PlayerID);

					Cast<AMyPlayerState>(Cast<AMyGameStateBase>(GetWorld()->GetGameState())->PlayerArray[k])->showWorldMessage(message, FVector(0, 1, 0));
				}

				clearUI();
				setHintShowed(true);
				Cast<AMyGameStateBase>(GetWorld()->GetGameState())->nextTurn();

				return;
			}
		}
	}
}

void AFamilyReunionDinner2Character::setHintShowed_Implementation(bool showed)
{
	Cast<AMyPlayerState>(GetPlayerState())->hintShowed = showed;
}

void AFamilyReunionDinner2Character::clearUI_Implementation() 
{
	MainUI->clearUI();
	UIOn = false;
}

void AFamilyReunionDinner2Character::requestCertainHandInfoWithIndex_Implementation(int playerIndex)
{
	sendCertainHandInfo(Cast<AMyPlayerState>(GetWorld()->GetGameState()->PlayerArray[playerIndex])->cookingCards, 0);
}

void AFamilyReunionDinner2Character::requestCertainHandInfo_Implementation(ACookingCard* card)
{
	for (int i = 0; i < GetWorld()->GetGameState()->PlayerArray.Num(); i += 1) 
	{
		TArray<ACookingCard*> curCards = Cast<AMyPlayerState>(GetWorld()->GetGameState()->PlayerArray[i])->cookingCards;

		for (int j = 0; j < curCards.Num(); j += 1)
		{
			if (curCards[j]->GetUniqueID() == card->GetUniqueID())
			{
				sendCertainHandInfo(curCards, j);

				return;
			}
		}
	}
}

void AFamilyReunionDinner2Character::requestMonsterInfo_Implementation() 
{
	sendMonsterInfo(Cast<AMyGameStateBase>(GetWorld()->GetGameState())->monsterPathInGame);
}

void AFamilyReunionDinner2Character::sendMonsterInfo_Implementation(const FString& path) 
{
	FVector2D screenSize;
	GetWorld()->GetGameViewport()->GetMousePosition(mousePos);
	GetWorld()->GetGameViewport()->GetViewportSize(screenSize);

	FVector2D canvasPreMultiplier(screenSize.X / 1920.0f, screenSize.Y / 1080.0f);
	float targetX = 0;
	float targetY = 0;

	if (mousePos.X < 375 * canvasPreMultiplier.X)
	{
		targetX = mousePos.X + 50 * canvasPreMultiplier.X;
	}
	else
	{
		targetX = mousePos.X - 375 * canvasPreMultiplier.X;
	}

	targetY = FMath::Clamp(mousePos.Y - 315.0f * canvasPreMultiplier.Y, 0.0f, screenSize.Y - 630.0f * canvasPreMultiplier.Y);

	MainUI->draggingCardPreviewOnScreen(path, targetX / screenSize.X * 1920.0f, targetY / screenSize.Y * 1080.0f);
}

void AFamilyReunionDinner2Character::sendCertainHandInfo_Implementation(const TArray<ACookingCard*>& data, int focusIndex)
{
	observingCards = data;

	TArray<FString> path;

	for (int i = 0; i < data.Num(); i += 1) 
	{
		path.Add(data[i]->data.path);
	}

	MainUI->showHintLayout(path);
	showFlavorHintPreview(focusIndex);
	showDegreeHintPreview(focusIndex);
}

void AFamilyReunionDinner2Character::addCookingCardToPot_Implementation(ACookingCard* card, int index)
{
	AMyGameStateBase* gameState = Cast<AMyGameStateBase>(GetWorld()->GetGameState());

	gameState->castCookingCardEffect(card, index);

	for (int i = 0; i < Cast<AMyPlayerState>(GetPlayerState())->cookingCards.Num(); i += 1) 
	{
		if (card->GetUniqueID() == Cast<AMyPlayerState>(GetPlayerState())->cookingCards[i]->GetUniqueID())
		{
			FVector targetPosition = card->GetActorLocation();
			FRotator targetRotation = card->GetActorRotation();

			gameState->removeCookingCardInGame(GetPlayerState(), i);
			gameState->addCookingCardInGame(GetPlayerState(), targetPosition, targetRotation, i);

			gameState->nextTurn();

			return;
		}
	}
}

void AFamilyReunionDinner2Character::addIngredientCardToPot_Implementation(int ingredientCardIndex, int potIndex)
{
	AMyGameStateBase* gameState = Cast<AMyGameStateBase>(GetWorld()->GetGameState());

	if (FCString::Atoi(*gameState->ingredientCardOnTableFileData[ingredientCardIndex].size) + Cast<AMyPlayerState>(GetPlayerState())->calculateParameter(gameState->recipeCardOnTableFileData[potIndex])[3] > FCString::Atoi(*gameState->recipeCardOnTableFileData[potIndex].size))
	{
		Cast<AMyPlayerState>(GetPlayerState())->showWorldMessage("! Size Exceeded the Limit !", FVector(1, 0, 0));

		return;
	}

	gameState->castIngredientCardEffect(ingredientCardIndex, potIndex);

	gameState->removeIngredientCardInGame(ingredientCardIndex);
	gameState->addIngredientCardInGame(ingredientCardIndex);

	gameState->nextTurn();
}

void AFamilyReunionDinner2Character::setObservingPotItemIndex_Implementation(int index) 
{
	observingItemInPotIndex = index;
}

void AFamilyReunionDinner2Character::removePotItemRequest_Implementation(int potIndex, int index)
{
	if (Cast<AMyPlayerState>(GetPlayerState())->actionPoint == 0)
	{
		FString message = TEXT("! Action Point is Insuffient !");
		Cast<AMyPlayerState>(GetPlayerState())->showWorldMessage(message, FVector(1, 0, 0));

		return;
	}

	Cast<AMyPlayerState>(GetPlayerState())->setActionPoint(Cast<AMyPlayerState>(GetPlayerState())->actionPoint - 1);

	for (int i = 0; i < GetWorld()->GetGameState()->PlayerArray.Num(); i += 1)
	{
		Cast<AFamilyReunionDinner2Character>(GetWorld()->GetGameState()->PlayerArray[i]->GetPawn())->clearUI();
		Cast<AFamilyReunionDinner2Character>(GetWorld()->GetGameState()->PlayerArray[i]->GetPawn())->setObservingPotItemIndex(index);
	}

	Cast<AMyGameStateBase>(GetWorld()->GetGameState())->inActionPotIndex = potIndex;
	Cast<AMyGameStateBase>(GetWorld()->GetGameState())->inActionPotItemIndex = index;

	Cast<AMyPlayerState>(GetPlayerState())->setReaction(true);
	Cast<AMyPlayerState>(GetPlayerState())->setReactionComplete(true);

	FString potItemPath;

	if (index <= Cast<AMyGameStateBase>(GetWorld()->GetGameState())->recipeCardOnTableFileData[potIndex].addedCookingCards.Num() - 1) 
	{
		potItemPath = Cast<AMyGameStateBase>(GetWorld()->GetGameState())->recipeCardOnTableFileData[potIndex].addedCookingCards[index].path;
	}
	else 
	{
		int trueIndex = index - Cast<AMyGameStateBase>(GetWorld()->GetGameState())->recipeCardOnTableFileData[potIndex].addedCookingCards.Num();
		potItemPath = Cast<AMyGameStateBase>(GetWorld()->GetGameState())->recipeCardOnTableFileData[potIndex].addedIngredientCards[trueIndex].path;
	}

	for (int i = 0; i < GetWorld()->GetGameState()->PlayerArray.Num(); i += 1)
	{
		if (GetPlayerState() != GetWorld()->GetGameState()->PlayerArray[i])
		{
			Cast<AFamilyReunionDinner2Character>(GetWorld()->GetGameState()->PlayerArray[i]->GetPawn())->sendReactionRequest(potIndex, potItemPath, TEXT("Stop->Remove Item"), FVector(1, 0, 0));
			Cast<AFamilyReunionDinner2Character>(GetWorld()->GetGameState()->PlayerArray[i]->GetPawn())->setObservingPotItemIndex(index);
			Cast<AMyPlayerState>(GetWorld()->GetGameState()->PlayerArray[i])->setReaction(true);
			Cast<AMyPlayerState>(GetWorld()->GetGameState()->PlayerArray[i])->setReactionComplete(false);
			Cast<AMyPlayerState>(GetWorld()->GetGameState()->PlayerArray[i])->setPreReaction(false);

			FString message = TEXT("Player ");
			message.Append(Cast<AMyPlayerState>(GetPlayerState())->FamilyReunionDinner2PlayerID);
			message.Append(TEXT(" Want to Remove Item in Pot..."));
			Cast<AMyPlayerState>(GetWorld()->GetGameState()->PlayerArray[i])->showWorldMessage(message, FVector(0, 1, 0));
		}
	}

	GetWorldTimerManager().PauseTimer(Cast<AMyGameStateBase>(GetWorld()->GetGameState())->turnTimer);
	Cast<AMyGameStateBase>(GetWorld()->GetGameState())->succussAction = true;
	Cast<AMyGameStateBase>(GetWorld()->GetGameState())->activeRemoveItemInPotTimer(index, potIndex);
}

void AFamilyReunionDinner2Character::replyRemovePotItemAction_Implementation() 
{
	if (Cast<AMyPlayerState>(GetPlayerState())->actionPoint == 0) 
	{
		FString message = TEXT("! Action Point is Insuffient !");
		Cast<AMyPlayerState>(GetPlayerState())->showWorldMessage(message, FVector(1, 0, 0));

		return;
	}

	Cast<AMyPlayerState>(GetPlayerState())->setActionPoint(Cast<AMyPlayerState>(GetPlayerState())->actionPoint - 1);

	clearUI();

	Cast<AMyPlayerState>(GetPlayerState())->setReactionComplete(true);

	Cast<AMyGameStateBase>(GetWorld()->GetGameState())->succussAction = !Cast<AMyGameStateBase>(GetWorld()->GetGameState())->succussAction;
	Cast<AMyGameStateBase>(GetWorld()->GetGameState())->activeRemoveItemInPotTimer(Cast<AMyGameStateBase>(GetWorld()->GetGameState())->inActionPotIndex, Cast<AMyGameStateBase>(GetWorld()->GetGameState())->inActionPotItemIndex);

	for (int i = 0; i < GetWorld()->GetGameState()->PlayerArray.Num(); i += 1)
	{
		if (!Cast<AMyPlayerState>(GetWorld()->GetGameState()->PlayerArray[i])->reactionComplete)
		{
			break;
		}

		if (i == GetWorld()->GetGameState()->PlayerArray.Num() - 1)
		{
			Cast<AMyGameStateBase>(GetWorld()->GetGameState())->getRemoveItemActionResult(Cast<AMyGameStateBase>(GetWorld()->GetGameState())->inActionPotIndex, Cast<AMyGameStateBase>(GetWorld()->GetGameState())->inActionPotItemIndex);

			for (int j = 0; j < GetWorld()->GetGameState()->PlayerArray.Num(); j += 1)
			{
				if (Cast<AMyGameStateBase>(GetWorld()->GetGameState())->succussAction)
				{
					FString message = TEXT("Player ");
					message.Append(Cast<AMyPlayerState>(GetPlayerState())->FamilyReunionDinner2PlayerID);
					message.Append(TEXT(" Countered Stopping Removing Item Action\nRecipe Finished: No One is Able to Use More Action"));
					Cast<AMyPlayerState>(GetWorld()->GetGameState()->PlayerArray[j])->showWorldMessage(message, FVector(0, 1, 0));
				}
				else
				{
					FString message = TEXT("Player ");
					message.Append(Cast<AMyPlayerState>(GetPlayerState())->FamilyReunionDinner2PlayerID);
					message.Append(TEXT(" Stopped Removing Item Action\nRecipe Rejected: No One is Able to Use More Action"));
					Cast<AMyPlayerState>(GetWorld()->GetGameState()->PlayerArray[j])->showWorldMessage(message, FVector(1, 0, 0));
				}
			}

			return;
		}
	}

	FString potItemPath;
	int index = Cast<AMyGameStateBase>(GetWorld()->GetGameState())->inActionPotItemIndex;
	int potIndex = Cast<AMyGameStateBase>(GetWorld()->GetGameState())->inActionPotIndex;

	if (index <= Cast<AMyGameStateBase>(GetWorld()->GetGameState())->recipeCardOnTableFileData[potIndex].addedCookingCards.Num() - 1)
	{
		potItemPath = Cast<AMyGameStateBase>(GetWorld()->GetGameState())->recipeCardOnTableFileData[potIndex].addedCookingCards[index].path;
	}
	else
	{
		int trueIndex = index - Cast<AMyGameStateBase>(GetWorld()->GetGameState())->recipeCardOnTableFileData[potIndex].addedCookingCards.Num();
		potItemPath = Cast<AMyGameStateBase>(GetWorld()->GetGameState())->recipeCardOnTableFileData[potIndex].addedIngredientCards[trueIndex].path;
	}

	for (int i = 0; i < GetWorld()->GetGameState()->PlayerArray.Num(); i += 1)
	{
		if (GetPlayerState() != GetWorld()->GetGameState()->PlayerArray[i])
		{
			if (Cast<AMyGameStateBase>(GetWorld()->GetGameState())->succussAction)
			{
				FString message = TEXT("Player ");
				message.Append(Cast<AMyPlayerState>(GetPlayerState())->FamilyReunionDinner2PlayerID);
				message.Append(TEXT(" Countered Stopping Removing Item Action"));
				Cast<AMyPlayerState>(GetWorld()->GetGameState()->PlayerArray[i])->showWorldMessage(message, FVector(0, 1, 0));
			}
			else
			{
				FString message = TEXT("Player ");
				message.Append(Cast<AMyPlayerState>(GetPlayerState())->FamilyReunionDinner2PlayerID);
				message.Append(TEXT(" Stopped Removing Item Action"));
				Cast<AMyPlayerState>(GetWorld()->GetGameState()->PlayerArray[i])->showWorldMessage(message, FVector(1, 0, 0));
			}
		}

		if (!Cast<AMyPlayerState>(GetWorld()->GetGameState()->PlayerArray[i])->reactionComplete)
		{
			Cast<AMyPlayerState>(GetWorld()->GetGameState()->PlayerArray[i])->setPreReaction(false);

			if (Cast<AMyGameStateBase>(GetWorld()->GetGameState())->succussAction)
			{
				Cast<AFamilyReunionDinner2Character>(GetWorld()->GetGameState()->PlayerArray[i]->GetPawn())->sendReactionRequest(Cast<AMyGameStateBase>(GetWorld()->GetGameState())->inActionPotIndex, potItemPath, TEXT("Stop->Removing Dish Item"), FVector(1, 0, 0));
			}
			else
			{
				Cast<AFamilyReunionDinner2Character>(GetWorld()->GetGameState()->PlayerArray[i]->GetPawn())->sendReactionRequest(Cast<AMyGameStateBase>(GetWorld()->GetGameState())->inActionPotIndex, potItemPath, TEXT("Counter->Removing Dish Item"), FVector(1, 0, 0));
			}
		}
	}
}

void AFamilyReunionDinner2Character::giveUpInRemovingItem_Implementation() 
{
	clearUI();
	Cast<AMyPlayerState>(GetPlayerState())->setPreReaction(true);

	for (int i = 0; i < GetWorld()->GetGameState()->PlayerArray.Num(); i += 1)
	{
		if (!Cast<AMyPlayerState>(GetWorld()->GetGameState()->PlayerArray[i])->reactionComplete && !Cast<AMyPlayerState>(GetWorld()->GetGameState()->PlayerArray[i])->preReaction)
		{
			break;
		}

		if (i == GetWorld()->GetGameState()->PlayerArray.Num() - 1)
		{
			Cast<AMyGameStateBase>(GetWorld()->GetGameState())->reactionTimeUpWithRemoveItemInPot(Cast<AMyGameStateBase>(GetWorld()->GetGameState())->inActionPotItemIndex, Cast<AMyGameStateBase>(GetWorld()->GetGameState())->inActionPotIndex);
		}
	}
}

void AFamilyReunionDinner2Character::finishRecipeCardRequest_Implementation(int index)
{
	if (Cast<AMyPlayerState>(GetPlayerState())->actionPoint == 0)
	{
		FString message = TEXT("! Action Point is Insuffient !");
		Cast<AMyPlayerState>(GetPlayerState())->showWorldMessage(message, FVector(1, 0, 0));

		return;
	}

	Cast<AMyPlayerState>(GetPlayerState())->setActionPoint(Cast<AMyPlayerState>(GetPlayerState())->actionPoint - 1);

	for (int i = 0; i < GetWorld()->GetGameState()->PlayerArray.Num(); i += 1)
	{
		Cast<AFamilyReunionDinner2Character>(GetWorld()->GetGameState()->PlayerArray[i]->GetPawn())->clearUI();
	}

	Cast<AMyGameStateBase>(GetWorld()->GetGameState())->inActionPotIndex = index;

	Cast<AMyPlayerState>(GetPlayerState())->setReaction(true);
	Cast<AMyPlayerState>(GetPlayerState())->setReactionComplete(true);

	FRecipeCardStruct data = Cast<AMyGameStateBase>(GetWorld()->GetGameState())->recipeCardOnTableFileData[index];

	for (int i = 0; i < GetWorld()->GetGameState()->PlayerArray.Num(); i += 1) 
	{
		if (GetPlayerState() != GetWorld()->GetGameState()->PlayerArray[i])
		{
			Cast<AFamilyReunionDinner2Character>(GetWorld()->GetGameState()->PlayerArray[i]->GetPawn())->sendReactionRequest(index, data.path, TEXT("Stop->Finishing Dish"), FVector(1, 0, 0));
			Cast<AMyPlayerState>(GetWorld()->GetGameState()->PlayerArray[i])->setReaction(true);
			Cast<AMyPlayerState>(GetWorld()->GetGameState()->PlayerArray[i])->setReactionComplete(false);
			Cast<AMyPlayerState>(GetWorld()->GetGameState()->PlayerArray[i])->setPreReaction(false);

			FString message = TEXT("Player ");
			message.Append(Cast<AMyPlayerState>(GetPlayerState())->FamilyReunionDinner2PlayerID);
			message.Append(TEXT(" Want to Finish Recipe..."));
			Cast<AMyPlayerState>(GetWorld()->GetGameState()->PlayerArray[i])->showWorldMessage(message, FVector(0, 1, 0));
		}
	}

	GetWorldTimerManager().PauseTimer(Cast<AMyGameStateBase>(GetWorld()->GetGameState())->turnTimer);
	Cast<AMyGameStateBase>(GetWorld()->GetGameState())->succussAction = true;
	Cast<AMyGameStateBase>(GetWorld()->GetGameState())->activeCompleteDishTimer(index);
}

void AFamilyReunionDinner2Character::replyRecipeFinishAction_Implementation()
{
	if (Cast<AMyPlayerState>(GetPlayerState())->actionPoint == 0)
	{
		FString message = TEXT("! Action Point is Insuffient !");
		Cast<AMyPlayerState>(GetPlayerState())->showWorldMessage(message, FVector(1, 0, 0));

		return;
	}

	Cast<AMyPlayerState>(GetPlayerState())->setActionPoint(Cast<AMyPlayerState>(GetPlayerState())->actionPoint - 1);

	clearUI();

	Cast<AMyPlayerState>(GetPlayerState())->setReactionComplete(true);

	Cast<AMyGameStateBase>(GetWorld()->GetGameState())->succussAction = !Cast<AMyGameStateBase>(GetWorld()->GetGameState())->succussAction;
	Cast<AMyGameStateBase>(GetWorld()->GetGameState())->activeCompleteDishTimer(Cast<AMyGameStateBase>(GetWorld()->GetGameState())->inActionPotIndex);

	for (int i = 0; i < GetWorld()->GetGameState()->PlayerArray.Num(); i += 1)
	{
		if (!Cast<AMyPlayerState>(GetWorld()->GetGameState()->PlayerArray[i])->reactionComplete)
		{
			break;
		}

		if (i == GetWorld()->GetGameState()->PlayerArray.Num() - 1)
		{
			Cast<AMyGameStateBase>(GetWorld()->GetGameState())->getDishActionResult(Cast<AMyGameStateBase>(GetWorld()->GetGameState())->inActionPotIndex);

			for (int j = 0; j < GetWorld()->GetGameState()->PlayerArray.Num(); j += 1)
			{
				if (Cast<AMyGameStateBase>(GetWorld()->GetGameState())->succussAction)
				{
					FString message = TEXT("Player ");
					message.Append(Cast<AMyPlayerState>(GetPlayerState())->FamilyReunionDinner2PlayerID);
					message.Append(TEXT(" Countered Stopping Finising Recipe Action\nRecipe Finished: No One is Able to Use More Action"));
					Cast<AMyPlayerState>(GetWorld()->GetGameState()->PlayerArray[j])->showWorldMessage(message, FVector(0, 1, 0));
				}
				else
				{
					FString message = TEXT("Player ");
					message.Append(Cast<AMyPlayerState>(GetPlayerState())->FamilyReunionDinner2PlayerID);
					message.Append(TEXT(" Stopped Finising Recipe Action\nRecipe Rejected: No One is Able to Use More Action"));
					Cast<AMyPlayerState>(GetWorld()->GetGameState()->PlayerArray[j])->showWorldMessage(message, FVector(1, 0, 0));
				}
			}

			return;
		}
	}

	FRecipeCardStruct data = Cast<AMyGameStateBase>(GetWorld()->GetGameState())->recipeCardOnTableFileData[Cast<AMyGameStateBase>(GetWorld()->GetGameState())->inActionPotIndex];
	TArray<int> parameters = Cast<AMyPlayerState>(GetPlayerState())->calculateParameter(data);

	for (int i = 0; i < GetWorld()->GetGameState()->PlayerArray.Num(); i += 1)
	{
		if (GetPlayerState() != GetWorld()->GetGameState()->PlayerArray[i])
		{
			if (Cast<AMyGameStateBase>(GetWorld()->GetGameState())->succussAction)
			{
				FString message = TEXT("Player ");
				message.Append(Cast<AMyPlayerState>(GetPlayerState())->FamilyReunionDinner2PlayerID);
				message.Append(TEXT(" Countered Stopping Finising Recipe Action"));
				Cast<AMyPlayerState>(GetWorld()->GetGameState()->PlayerArray[i])->showWorldMessage(message, FVector(0, 1, 0));
			}
			else 
			{
				FString message = TEXT("Player ");
				message.Append(Cast<AMyPlayerState>(GetPlayerState())->FamilyReunionDinner2PlayerID);
				message.Append(TEXT(" Stopped Finising Recipe Action"));
				Cast<AMyPlayerState>(GetWorld()->GetGameState()->PlayerArray[i])->showWorldMessage(message, FVector(1, 0, 0));
			}
		}

		if (!Cast<AMyPlayerState>(GetWorld()->GetGameState()->PlayerArray[i])->reactionComplete)
		{			
			Cast<AMyPlayerState>(GetWorld()->GetGameState()->PlayerArray[i])->setPreReaction(false);

			if (Cast<AMyGameStateBase>(GetWorld()->GetGameState())->succussAction)
			{
				Cast<AFamilyReunionDinner2Character>(GetWorld()->GetGameState()->PlayerArray[i]->GetPawn())->sendReactionRequest(Cast<AMyGameStateBase>(GetWorld()->GetGameState())->inActionPotIndex, data.path, TEXT("Stop->Finishing Dish"), FVector(1, 0, 0));
			}
			else
			{
				Cast<AFamilyReunionDinner2Character>(GetWorld()->GetGameState()->PlayerArray[i]->GetPawn())->sendReactionRequest(Cast<AMyGameStateBase>(GetWorld()->GetGameState())->inActionPotIndex, data.path, TEXT("Counter->Stop Finishing Dish"), FVector(1, 0, 0));
			}
		}
	}
}

void AFamilyReunionDinner2Character::giveUpReactionInRecipeFinish_Implementation() 
{
	clearUI();
	Cast<AMyPlayerState>(GetPlayerState())->setPreReaction(true);

	for (int i = 0; i < GetWorld()->GetGameState()->PlayerArray.Num(); i += 1)
	{
		if (!Cast<AMyPlayerState>(GetWorld()->GetGameState()->PlayerArray[i])->reactionComplete && !Cast<AMyPlayerState>(GetWorld()->GetGameState()->PlayerArray[i])->preReaction)
		{
			break;
		}

		if (i == GetWorld()->GetGameState()->PlayerArray.Num() - 1)
		{
			Cast<AMyGameStateBase>(GetWorld()->GetGameState())->reactionTimeUpWithCompleteDish(Cast<AMyGameStateBase>(GetWorld()->GetGameState())->inActionPotIndex);
		}
	}
}

void AFamilyReunionDinner2Character::sendReactionRequest_Implementation(int potIndex, const FString& actionPath, const FString& actionDes, FVector actionColor)
{
	FRecipeCardStruct data = Cast<AMyPlayerState>(GetPlayerState())->recipeCards[potIndex]->data;
	TArray<int> parameters = Cast<AMyPlayerState>(GetPlayerState())->calculateParameter(data);

	TArray<FString> addedItemPath;

	for (int i = 0; i < data.addedCookingCards.Num(); i += 1)
	{
		addedItemPath.Add(data.addedCookingCards[i].path);
	}
	for (int i = 0; i < Cast<AMyPlayerState>(GetPlayerState())->recipeCards[potIndex]->data.addedIngredientCards.Num(); i += 1)
	{
		addedItemPath.Add(data.addedIngredientCards[i].path);
	}

	MainUI->showPotInfo(data.path, parameters[0], parameters[1], parameters[2], parameters[3], FCString::Atoi(*data.size), addedItemPath, actionPath, actionDes, actionColor);
}

void AFamilyReunionDinner2Character::startGame_Implementation() 
{
	Cast<AMyGameStateBase>(GetWorld()->GetGameState())->initGame();
}


void AFamilyReunionDinner2Character::showFlavorHintPreview(int index)
{
	observingCardIndex = index;
	TArray<int> cardIndexForHint;

	for (int i = 0; i < observingCards.Num(); i += 1) 
	{
		if (observingCards[index]->data.type == observingCards[i]->data.type)
		{
			cardIndexForHint.Add(i);
		}
	}

	MainUI->showFlavorHintPreviewUI(cardIndexForHint);
}

void AFamilyReunionDinner2Character::showDegreeHintPreview(int index)
{
	observingCardIndex = index;
	TArray<int> cardIndexForHint;

	for (int i = 0; i < observingCards.Num(); i += 1)
	{
		if (observingCards[index]->data.degree == observingCards[i]->data.degree)
		{
			cardIndexForHint.Add(i);
		}
	}

	MainUI->showDegreeHintPreviewUI(cardIndexForHint);
}

void AFamilyReunionDinner2Character::setWaitingTextUI_Implementation(const FString& text)
{
	MainUI->setWaitingText(text);
}

void AFamilyReunionDinner2Character::requestGameOver_Implementation()
{
	AMyGameStateBase* gameState = Cast<AMyGameStateBase>(GetWorld()->GetGameState());
	FString errorMessage;

	if (!gameState->checkCanGameOver(errorMessage)) 
	{
		Cast<AMyPlayerState>(GetPlayerState())->showWorldMessage(errorMessage, FVector(1, 0, 0));

		return;
	}

	gameState->checkGameFinalStatus();
}

void AFamilyReunionDinner2Character::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AFamilyReunionDinner2Character::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}