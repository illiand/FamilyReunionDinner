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
	//mesh
	UStaticMeshComponent* mesh = Cast<UStaticMeshComponent>(GetDefaultSubobjectByName(TEXT("Plane")));
	FLinearColor color;

	if (data.type == "Spicy")
	{
		color = FLinearColor::Red;
	}
	else if (data.type == "Sour")
	{
		color = FLinearColor::Yellow;
	}
	else if (data.type == "Sweet")
	{
		color.R = 1.f;
		color.G = 0.412f;
		color.B = 0.706f;
	}
	else if (data.type == "Salty")
	{
		color = FLinearColor::Blue;
	}
	else if (data.type == "Heat")
	{
		color = FLinearColor::White;
	}

	material = UMaterialInstanceDynamic::Create(mesh->GetMaterial(0), NULL);
	material->SetVectorParameterValue(TEXT("CardDiffuse"), color);
	mesh->SetMaterial(0, material);

	//text on mesh
	TArray<UTextRenderComponent*> attributes;
	GetComponents(attributes);

	for (int i = 0; i < attributes.Num(); i++)
	{
		if (attributes[i]->GetName().Equals("degreeHint"))
		{
			hintTextMaterial = UMaterialInstanceDynamic::Create(attributes[i]->GetMaterial(0), 0);
			attributes[i]->SetMaterial(0, hintTextMaterial);
			attributes[i]->SetText(data.degree);
		}
		else if (attributes[i]->GetName().Equals("degreeHintFront"))
		{
			hintFrontTextMaterial = UMaterialInstanceDynamic::Create(attributes[i]->GetMaterial(0), 0);

			FLinearColor curColor;

			if (data.type != "Heat")
			{
				curColor = FLinearColor::White;
			}
			else
			{
				curColor = FLinearColor::Red;
			}

			hintFrontTextMaterial = UMaterialInstanceDynamic::Create(attributes[i]->GetMaterial(0), 0);
			hintFrontTextMaterial->SetVectorParameterValue(TEXT("Color"), curColor);

			attributes[i]->SetMaterial(0, hintFrontTextMaterial);
			attributes[i]->SetText(data.degree);
		}
	}
}

void ACookingCard::changeDegreeHintStatus_Implementation(bool isOn)
{
	if (GetLocalRole() == 3)
	{
		return;
	}

	if (isOn) 
	{
		hintFrontTextMaterial->SetScalarParameterValue(TEXT("Strength"), 25);
		hintTextMaterial->SetVectorParameterValue(TEXT("Color"), FLinearColor::Green);
	}
	else 
	{
		FLinearColor color = FLinearColor::Black;
		color.A = 0.f;

		hintFrontTextMaterial->SetScalarParameterValue(TEXT("Strength"), 1);
		hintTextMaterial->SetVectorParameterValue(TEXT("Color"), color);
	}

}

