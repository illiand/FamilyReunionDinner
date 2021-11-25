// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RecipeCardStruct.h"
#include "Components/TextRenderComponent.h"
#include "Kismet/GameplayStatics.h"
#include "RecipeCard.generated.h"

UCLASS()
class FAMILYREUNIONDINNER2_API ARecipeCard : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARecipeCard();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	AActor* border;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FRecipeCardStruct data;

	void assignInfo();
};
