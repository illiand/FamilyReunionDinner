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

//////////////////////////////////////////////////////////////////////////
// Input

void AFamilyReunionDinner2Character::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	check(PlayerInputComponent);

	// Bind jump events
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	// Bind movement events
	PlayerInputComponent->BindAxis("MoveForward", this, &AFamilyReunionDinner2Character::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AFamilyReunionDinner2Character::MoveRight);

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

void AFamilyReunionDinner2Character::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(AFamilyReunionDinner2Character, fileData);
}

void AFamilyReunionDinner2Character::useSpecialAction() 
{
	//GEngine->AddOnScreenDebugMessage(-1, 8.f, FColor::Red, TEXT("Test Action"));
	//UE_LOG(LogTemp, Warning, TEXT("name is %s"), *test[3]->GetStringField("Name"));
	//moveActor();
	UE_LOG(LogTemp, Warning, TEXT("found %d"), GetLocalRole());
	startGame();
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

		if (curCard->GetActorLocation().Z == 80)
		{
			curCard->SetActorLocation(FVector(curCard->GetActorLocation().X, curCard->GetActorLocation().Y, 130));
		}
		else
		{
			curCard->SetActorLocation(FVector(curCard->GetActorLocation().X, curCard->GetActorLocation().Y, 80));
		}
	}
}

void AFamilyReunionDinner2Character::startGame_Implementation() 
{
	fileData = UAPIClass::makeRecipeCards();

	for (int i = 0; i < fileData.Num(); i += 1) 
	{
		int x = 40 - i / 5 * 200;
		int y = -320 + i % 5 * 100;
		ARecipeCard* card = GetWorld()->SpawnActor<ARecipeCard>(recipeCard, FVector(x, y, 80), FRotator(0, 90, 0), FActorSpawnParameters());
		card->data = fileData[i];
	}
}

void AFamilyReunionDinner2Character::moveActor_Implementation()
{
	moveActorRA();
}

void AFamilyReunionDinner2Character::moveActorAnother_Implementation()
{
	TArray<AActor*> allActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), allActors);
	//UGameplayStatics::GetAllActorsWithTag(GetWorld(), TEXT("Rock"), AllActors);

	AActor* testObject = NULL;

	for (size_t i = 0; i < allActors.Num(); i++)
	{
		//UE_LOG(LogTemp, Warning, TEXT("rock is %d"), allActors[i]->ActorHasTag("Rock"));

		if (allActors[i]->GetName() == "Rock")
		{
			testObject = allActors[i];
		}
	}

	testObject->SetReplicates(true);

	GEngine->AddOnScreenDebugMessage(-1, 8.f, FColor::Red, FString::Printf(TEXT("rock is %s"), *testObject->Tags[0].ToString()));
	testObject->SetActorLocation(FVector(0, 0, testObject->GetActorLocation().Z + 10));

	UE_LOG(LogTemp, Warning, TEXT("%d %d"), testObject->GetActorLocation().X, testObject->GetActorLocation().Z);
}

void AFamilyReunionDinner2Character::moveActorRA_Implementation()
{
	TArray<AActor*> allActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), allActors);
	//UGameplayStatics::GetAllActorsWithTag(GetWorld(), TEXT("Rock"), AllActors);

	AActor* testObject = NULL;

	for (size_t i = 0; i < allActors.Num(); i++)
	{
		//UE_LOG(LogTemp, Warning, TEXT("rock is %d"), allActors[i]->ActorHasTag("Rock"));

		if (allActors[i]->GetName() == "Rock")
		{
			testObject = allActors[i];
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("something runs"));

	testObject->SetReplicates(true);

	GEngine->AddOnScreenDebugMessage(-1, 8.f, FColor::Red, FString::Printf(TEXT("rock is %s"), *testObject->Tags[0].ToString()));
	testObject->SetActorLocation(FVector(0, 0, testObject->GetActorLocation().Z + 10));
}

void AFamilyReunionDinner2Character::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void AFamilyReunionDinner2Character::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
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
