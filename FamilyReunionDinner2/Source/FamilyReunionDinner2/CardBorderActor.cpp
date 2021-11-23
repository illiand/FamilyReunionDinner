// Fill out your copyright notice in the Description page of Project Settings.


#include "CardBorderActor.h"

// Sets default values
ACardBorderActor::ACardBorderActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ACardBorderActor::BeginPlay()
{
	Super::BeginPlay();

	UStaticMeshComponent* borderMesh = Cast<UStaticMeshComponent>(GetDefaultSubobjectByName(TEXT("cardBorder")));

	if (borderMesh != NULL)
	{
		UMaterialInstanceDynamic* material = UMaterialInstanceDynamic::Create(borderMesh->GetMaterial(0), NULL);

		FLinearColor newColor;
		int randValue = FMath::RandRange(0, 2);

		if (randValue == 0) 
		{
			newColor.R = 1;
		}
		else if (randValue == 1) 
		{
			newColor.G = 1;
		}
		else 
		{
			newColor.B = 1;
		}

		material->SetVectorParameterValue(TEXT("color"), newColor);
		borderMesh->SetMaterial(0, material);
	}
}

// Called every frame
void ACardBorderActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (parent != NULL) 
	{
		SetActorLocation(FVector(parent->GetActorLocation().X, parent->GetActorLocation().Y, parent->GetActorLocation().Z));
		SetActorRotation(FRotator(parent->GetActorRotation().Pitch, parent->GetActorRotation().Yaw, parent->GetActorRotation().Roll));
		SetActorScale3D(FVector(parent->GetActorScale().X, parent->GetActorScale().Y, parent->GetActorScale().Z));
	}
}

void ACardBorderActor::appendCard(AActor* card) 
{
	parent = card;
}