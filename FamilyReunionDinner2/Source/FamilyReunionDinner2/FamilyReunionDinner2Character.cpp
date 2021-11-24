// Copyright Epic Games, Inc. All Rights Reserved.

#include "FamilyReunionDinner2Character.h"
#include "FamilyReunionDinner2Projectile.h"
#include "MyGameStateBase.h"
#include "MyPlayerState.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/InputSettings.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
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
}

void AFamilyReunionDinner2Character::setLocationByIndex(int index)
{
	switch (index)
	{
	case 1:
		SetActorLocation(FVector(60, 60, 40));
		break;

	case 2:
		SetActorLocation(FVector(60, -90, 40));
		break;

	case 3:
		SetActorLocation(FVector(180, -90, 40));
		break;
	
	case 4:
		SetActorLocation(FVector(180, 60, 40));
		break;
	}
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
	startGame();
}

void AFamilyReunionDinner2Character::endTurn_Implementation() 
{
	Cast<AMyGameStateBase>(GetWorld()->GetGameState())->nextTurn();
}

AActor* AFamilyReunionDinner2Character::pickFromEye()
{
	//FVector CameraLoc = GetFirstPersonCameraComponent()->GetComponentLocation();
	//FRotator CameraRot = GetFirstPersonCameraComponent()->GetComponentRotation();

	//FVector Start = CameraLoc;
	//FVector End = CameraLoc + CameraRot.Vector() * 2000;
	
	FHitResult hit(ForceInit);
	//FCollisionObjectQueryParams objectParams;
	//FCollisionQueryParams params;
	//params.AddIgnoredActor(this);
	//
	//GetWorld()->LineTraceSingleByObjectType(hit, Start, End, objectParams, params);

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
	MainUI->clearUI();

	AActor* curTarget = pickFromEye();

	if (curTarget != NULL && curTarget->Tags.Num() != 0)
	{
		if (curTarget->Tags[0] == TEXT("card"))
		{
			//other players' cooking card 
			if (curTarget->GetLocalRole() != 3 && Cast<AMyPlayerState>(GetPlayerState())->cookingCards.Contains(curTarget))
			{
				//show give a hint panal
				if (!Cast<AMyPlayerState>(GetPlayerState())->hintShowed && Cast<AMyPlayerState>(GetPlayerState())->inTurn)
				{
					MainUI->showHintLayout();
				}
			}
			else 
			{
				holdingItem = curTarget;
				//show highlight of the position that can put the card
				//draw Arrow?
			}
		}
		else if (curTarget->Tags[0] == TEXT("pot") || curTarget->Tags[0] == TEXT("potCard")) 
		{
			//show UI of pot
			ARecipeCard* card = NULL;

			if (curTarget->Tags[0] == TEXT("pot"))
			{
				//TODO
			}
			else 
			{
				card = Cast<ARecipeCard>(curTarget);
			}
			
			MainUI->showPotInfo(TEXT("Texture2D'/Game/Assets/Texture/uv.uv'"), card->curFlavor, card->curHeat, card->curSize, FCString::Atoi(*card->data.size));
		}
	}
}

void AFamilyReunionDinner2Character::puttingItem()
{
	if (holdingItem == NULL) 
	{
		return;
	}

	AActor* curTarget = pickFromEye();

	if (curTarget != NULL && curTarget->Tags.Num() != 0)
	{
		if (curTarget->Tags[0] == TEXT("pot") || curTarget->Tags[0] == TEXT("potCard")) 
		{
			ARecipeCard* card = NULL;

			if (curTarget->Tags[0] == TEXT("pot"))
			{
				//TODO
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

void AFamilyReunionDinner2Character::drawItemHint() 
{

}

void AFamilyReunionDinner2Character::addCookingCardToPot_Implementation(ACookingCard* card, int index)
{
	AMyGameStateBase* gameState = Cast<AMyGameStateBase>(GetWorld()->GetGameState());

	gameState->castCookingCardEffect(card, index);

	gameState->removeCookingCardInGame(GetPlayerState(), index);
	gameState->addCookingCardInGame(GetPlayerState(), index);
}

void AFamilyReunionDinner2Character::addIngredientCardToPot_Implementation(int ingredientCardIndex, int potIndex)
{
	AMyGameStateBase* gameState = Cast<AMyGameStateBase>(GetWorld()->GetGameState());

	gameState->castIngredientCardEffect(ingredientCardIndex, potIndex);

	gameState->removeIngredientCardInGame(ingredientCardIndex);
	gameState->addIngredientCardInGame(ingredientCardIndex);
}

void AFamilyReunionDinner2Character::finishRecipeCard_Implementation(int index)
{
	AMyGameStateBase* gameState = Cast<AMyGameStateBase>(GetWorld()->GetGameState());

	gameState->castRecipeCardEffect(index);

	gameState->removeRecipeCardInGame(index);
	gameState->addRecipeCardInGame(index);
}

void AFamilyReunionDinner2Character::moveToDeck_Implementation(AActor* hitActor) 
{
	UE_LOG(LogTemp, Warning, TEXT("find: %d %d"), hitActor, true);

	if (true)return;

	if (hitActor->Tags.Num() > 0 && hitActor->Tags[0] == TEXT("card"))
	{
		ARecipeCard* curCard = Cast<ARecipeCard>(hitActor);

		if (curCard->GetActorLocation().Z == 53.5)
		{
			curCard->SetActorLocation(FVector(curCard->GetActorLocation().X, curCard->GetActorLocation().Y, 75));
		}
		else
		{
			curCard->SetActorLocation(FVector(curCard->GetActorLocation().X, curCard->GetActorLocation().Y, 53.5));
		}
	}
}

void AFamilyReunionDinner2Character::startGame_Implementation() 
{
	Cast<AMyGameStateBase>(GetWorld()->GetGameState())->initGame();
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
