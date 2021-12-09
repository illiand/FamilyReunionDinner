// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PointingArrow.generated.h"

UCLASS()
class FAMILYREUNIONDINNER2_API APointingArrow : public AActor
{
	GENERATED_BODY()

public:
	float curDegree = 0;
	FVector targetLocation;

public:	
	// Sets default values for this actor's properties
	APointingArrow();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
