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

	assignInfo();
}

// Called every frame
void ARecipeCard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ARecipeCard::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ARecipeCard, data);
}

void ARecipeCard::assignInfo_Implementation()
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

		if (attributes[i]->GetName().Equals("flavorRange"))
		{
			FString prefix = "Flavor: ";
			attributes[i]->SetText(prefix.Append(data.flavorRange));
		}

		if (attributes[i]->GetName().Equals("heatRange"))
		{
			FString prefix = "Heat: ";
			attributes[i]->SetText(prefix.Append(data.heatRange));
		}

		if (attributes[i]->GetName().Equals("utensilSize"))
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

