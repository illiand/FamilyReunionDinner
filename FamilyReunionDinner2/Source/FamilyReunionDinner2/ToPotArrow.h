// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"
#include "ToPotArrow.generated.h"

UCLASS()
class FAMILYREUNIONDINNER2_API AToPotArrow : public AActor
{
	GENERATED_BODY()

private:
	UStaticMeshComponent* body;
	UStaticMeshComponent* head;
	UMaterialInstanceDynamic* material;

	FVector startPos;
	FVector endPos;

public:	
	// Sets default values for this actor's properties
	AToPotArrow();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void arrowInit(FVector start, FVector end);
};
