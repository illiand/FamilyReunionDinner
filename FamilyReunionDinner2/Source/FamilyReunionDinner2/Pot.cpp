// Fill out your copyright notice in the Description page of Project Settings.


#include "Pot.h"
#include "Runtime/Engine/Classes/Particles/ParticleSystemComponent.h"
#include "Components/TextRenderComponent.h"

// Sets default values
APot::APot()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APot::BeginPlay()
{
	Super::BeginPlay();
	
	UStaticMeshComponent* potMesh = Cast<UStaticMeshComponent>(GetDefaultSubobjectByName(TEXT("PotUnder")));
	
	if (potMesh != NULL)
	{
		heatMaterial = UMaterialInstanceDynamic::Create(potMesh->GetMaterial(5), NULL);
		flavorMaterial = UMaterialInstanceDynamic::Create(potMesh->GetMaterial(6), NULL);
		potMesh->SetMaterial(5, heatMaterial);
		potMesh->SetMaterial(6, flavorMaterial);

		heatMaterial->SetScalarParameterValue(TEXT("Degree"), 0);
		flavorMaterial->SetScalarParameterValue(TEXT("Degree"), 0);

		Cast<UParticleSystemComponent>(GetDefaultSubobjectByName(TEXT("Fire")))->SetIntParameter(FName("HeatStrength"), 0);
	}
}

// Called every frame
void APot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APot::setPotFlavorDegree(int degree) 
{
	flavorMaterial->SetScalarParameterValue(TEXT("Degree"), degree);
}

void APot::setPotHeatDegree(int degree) 
{
	heatMaterial->SetScalarParameterValue(TEXT("Degree"), degree);
	Cast<UParticleSystemComponent>(GetDefaultSubobjectByName(TEXT("Fire")))->SetIntParameter(FName("HeatStrength"), degree * 25);
}

void APot::setPotBonusPoint(int curPoint) 
{
	Cast<UTextRenderComponent>(GetDefaultSubobjectByName(TEXT("bonusPoint")))->SetText(FString::FromInt(curPoint));
}