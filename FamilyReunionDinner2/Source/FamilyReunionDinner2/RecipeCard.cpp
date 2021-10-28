// Fill out your copyright notice in the Description page of Project Settings.

#include "RecipeCard.h"
#include "Components/BoxComponent.h"

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

	created();
}

// Called every frame
void ARecipeCard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ARecipeCard::created_Implementation() 
{
	assignInfo();
}

void ARecipeCard::assignInfo_Implementation()
{
	TArray<UTextRenderComponent*> attributes;
	GetComponents(attributes);

	TArray<TSharedPtr<FJsonObject>> fileData = UAPIClass::readFile(TEXT("RecipeCardData.txt"));

	AFamilyReunionDinner2Character* character = NULL;
	TArray<AActor*> allActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), allActors);

	for (size_t i = 0; i < allActors.Num(); i++)
	{
		UE_LOG(LogTemp, Warning, TEXT("found %s"), *allActors[i]->GetName());

		if (allActors[i]->GetName() == "FirstPersonCharacter_C_0")
		{
			character = (AFamilyReunionDinner2Character*)allActors[i];
		}
	}

	for (int i = 0; i < attributes.Num(); i++)
	{
		if (attributes[i]->GetName().Equals("name"))
		{
			attributes[i]->SetText(fileData[character->count]->GetStringField(TEXT("Name")));
		}

		if (attributes[i]->GetName().Equals("type"))
		{
			attributes[i]->SetText(fileData[character->count]->GetStringField(TEXT("Type")));
		}

		if (attributes[i]->GetName().Equals("flavorRange"))
		{
			FString prefix = "Flavor: ";
			attributes[i]->SetText(prefix.Append(fileData[character->count]->GetStringField(TEXT("Flavor_Range"))));
		}

		if (attributes[i]->GetName().Equals("heatRange"))
		{
			FString prefix = "Heat: ";
			attributes[i]->SetText(prefix.Append(fileData[character->count]->GetStringField(TEXT("Heat_Range"))));
		}

		if (attributes[i]->GetName().Equals("utensilSize"))
		{
			FString prefix = "Size: ";
			attributes[i]->SetText(prefix.Append(fileData[character->count]->GetStringField(TEXT("Utensil_Size"))));
		}

		if (attributes[i]->GetName().Equals("point"))
		{
			attributes[i]->SetText(fileData[character->count]->GetStringField(TEXT("Points ")));
		}
	}

	character->count += 1;
}

