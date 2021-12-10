// Fill out your copyright notice in the Description page of Project Settings.


#include "PotBorder.h"

// Sets default values
APotBorder::APotBorder()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	UStaticMeshComponent* mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisualRepresentation"));
	RootComponent = mesh;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> findAsset(TEXT("StaticMesh'/Game/Assets/Models/PotBorder/PotBorder.PotBorder'"));
	if (findAsset.Succeeded())
	{
		mesh->SetStaticMesh(findAsset.Object);
		mesh->SetCastShadow(false);
	}
}

// Called when the game starts or when spawned
void APotBorder::BeginPlay()
{
	Super::BeginPlay();
	
	SetActorScale3D(FVector(0.6, 0.6, 0.6));
}

// Called every frame
void APotBorder::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

