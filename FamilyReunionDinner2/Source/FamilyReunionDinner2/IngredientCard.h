// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "IngredientCardStruct.h"
#include "Components/TextRenderComponent.h"
#include "Kismet/GameplayStatics.h"
#include "IngredientCard.generated.h"

UCLASS()
class FAMILYREUNIONDINNER2_API AIngredientCard : public AActor
{
	GENERATED_BODY()

private:
	UMaterialInstanceDynamic* material;

public:
	// Sets default values for this actor's properties
	AIngredientCard();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FIngredientCardStruct data;
	AActor* border;

	void assignInfo();
};
