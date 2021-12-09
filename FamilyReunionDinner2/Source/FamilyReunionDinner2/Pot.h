// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Pot.generated.h"

UCLASS()
class FAMILYREUNIONDINNER2_API APot : public AActor
{
	GENERATED_BODY()

public:
	AActor* associatedCard;

private:
	UMaterialInstanceDynamic* flavorMaterial;
	UMaterialInstanceDynamic* heatMaterial;

public:	
	// Sets default values for this actor's properties
	APot();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void setPotFlavorDegree(int degree);
	void setPotHeatDegree(int degree);
	void setPotBonusPoint(int curPoint);
};
