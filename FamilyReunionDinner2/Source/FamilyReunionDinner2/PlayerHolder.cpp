// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHolder.h"

// Sets default values
APlayerHolder::APlayerHolder()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	UStaticMeshComponent* mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisualRepresentation"));
	RootComponent = mesh;
}

// Called when the game starts or when spawned
void APlayerHolder::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlayerHolder::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

