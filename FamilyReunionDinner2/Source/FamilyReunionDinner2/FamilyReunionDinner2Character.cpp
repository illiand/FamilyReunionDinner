// Copyright Epic Games, Inc. All Rights Reserved.

#include "FamilyReunionDinner2Character.h"
#include "FamilyReunionDinner2Projectile.h"
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
		SetActorLocation(FVector(-10, 0, 40));
		break;

	case 2:
		SetActorLocation(FVector(60, 60, 40));
		break;

	case 3:
		SetActorLocation(FVector(60, -90, 40));
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
	PlayerInputComponent->BindAction("MoveToDeck", IE_Released, this, &AFamilyReunionDinner2Character::pickFromEye);
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

void AFamilyReunionDinner2Character::pickFromEye()
{
	FVector CameraLoc = GetFirstPersonCameraComponent()->GetComponentLocation();
	FRotator CameraRot = GetFirstPersonCameraComponent()->GetComponentRotation();

	FVector Start = CameraLoc;
	FVector End = CameraLoc + CameraRot.Vector() * 2000;

	FHitResult hit(ForceInit);
	FCollisionObjectQueryParams objectParams;
	FCollisionQueryParams params;
	params.AddIgnoredActor(this);

	GetWorld()->LineTraceSingleByObjectType(hit, Start, End, objectParams, params);

	if (hit.GetActor() != NULL)
	{
		moveToDeck(hit.GetActor());
	}
}

void AFamilyReunionDinner2Character::moveToDeck_Implementation(AActor* hitActor) 
{
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
