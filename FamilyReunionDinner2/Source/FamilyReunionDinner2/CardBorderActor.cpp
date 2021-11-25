// Fill out your copyright notice in the Description page of Project Settings.


#include "CardBorderActor.h"
#include "Net/UnrealNetwork.h"

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
		material = UMaterialInstanceDynamic::Create(borderMesh->GetMaterial(0), NULL);
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

void ACardBorderActor::setBorderColor(float r, float g, float b, float a) 
{
	FLinearColor newColor;

	newColor.R = r;
	newColor.G = g;
	newColor.B = b;
	newColor.A = a;

	material->SetVectorParameterValue(TEXT("color"), newColor);
}

void ACardBorderActor::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACardBorderActor, parent);
}