// Fill out your copyright notice in the Description page of Project Settings.


#include "PointingArrow.h"

// Sets default values
APointingArrow::APointingArrow()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	UStaticMeshComponent* mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisualRepresentation"));
	RootComponent = mesh;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> findAsset(TEXT("StaticMesh'/Game/Assets/Models/Arrow/Arrow.Arrow'"));
	if (findAsset.Succeeded())
	{
		mesh->SetStaticMesh(findAsset.Object);
		mesh->SetCastShadow(false);
	}

}

// Called when the game starts or when spawned
void APointingArrow::BeginPlay()
{
	Super::BeginPlay();
	
	SetActorScale3D(FVector(0.1, 0.1, 0.1));
	curDegree = PI / 4.f;
}

// Called every frame
void APointingArrow::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	float heightChange = 2 * (FMath::Sin(curDegree + 3 * DeltaTime) - FMath::Sin(3 * curDegree));
	SetActorLocation(FVector(targetLocation.X, targetLocation.Y, targetLocation.Z + heightChange));

	curDegree += DeltaTime;
}

