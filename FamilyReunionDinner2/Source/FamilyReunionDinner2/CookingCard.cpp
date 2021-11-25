// Fill out your copyright notice in the Description page of Project Settings.

#include "CookingCard.h"
#include "Net/UnrealNetwork.h"
#include "Components/BoxComponent.h"

// Sets default values
ACookingCard::ACookingCard()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ACookingCard::BeginPlay()
{
	Super::BeginPlay();

	assignInfo();
}

// Called every frame
void ACookingCard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACookingCard::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACookingCard, data);
	DOREPLIFETIME(ACookingCard, border);
}

void ACookingCard::assignInfo_Implementation()
{
	TArray<UTextRenderComponent*> attributes;
	GetComponents(attributes);

	for (int i = 0; i < attributes.Num(); i++)
	{
		if (attributes[i]->GetName().Equals("cardName"))
		{
			attributes[i]->SetText(data.name.Append(" + ").Append(data.degree));
		}

		if (attributes[i]->GetName().Equals("cardType"))
		{
			attributes[i]->SetText(data.type);
		}
	}
}

