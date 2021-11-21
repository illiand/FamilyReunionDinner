// Fill out your copyright notice in the Description page of Project Settings.

#include "IngredientCard.h"
#include "Components/BoxComponent.h"

// Sets default values
AIngredientCard::AIngredientCard()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AIngredientCard::BeginPlay()
{
	Super::BeginPlay();

	assignInfo();
}

// Called every frame
void AIngredientCard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AIngredientCard::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AIngredientCard, data);
}

void AIngredientCard::assignInfo_Implementation()
{

	TArray<UTextRenderComponent*> attributes;
	GetComponents(attributes);

	for (int i = 0; i < attributes.Num(); i++)
	{
		if (attributes[i]->GetName().Equals("cardName"))
		{
			attributes[i]->SetText(data.name);
		}

		if (attributes[i]->GetName().Equals("curType"))
		{
			attributes[i]->SetText(data.type);
		}
		if (attributes[i]->GetName().Equals("Size"))
		{
			FString prefix = "Size: ";
			attributes[i]->SetText(prefix.Append(data.size));
		}

		if (attributes[i]->GetName().Equals("bonusPoint"))
		{
			attributes[i]->SetText(data.point);
		}
	}
}

