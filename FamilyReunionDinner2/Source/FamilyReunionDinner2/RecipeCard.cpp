// Fill out your copyright notice in the Description page of Project Settings.

#include "RecipeCard.h"
#include "Components/BoxComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ARecipeCard::ARecipeCard()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ARecipeCard::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ARecipeCard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ARecipeCard::assignInfo()
{
	UStaticMeshComponent* mesh = Cast<UStaticMeshComponent>(GetDefaultSubobjectByName(TEXT("Plane")));

	if (mesh != NULL)
	{
		UTexture2D* texture = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, *(data.path)));

		material = UMaterialInstanceDynamic::Create(mesh->GetMaterial(0), NULL);

		material->SetTextureParameterValue(TEXT("CardDiffuse"), texture);
		mesh->SetMaterial(0, material);
	}
}

