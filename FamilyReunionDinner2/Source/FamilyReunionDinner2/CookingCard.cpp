// Fill out your copyright notice in the Description page of Project Settings.


#include "CookingCard.h"

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
	
}

// Called every frame
void ACookingCard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

