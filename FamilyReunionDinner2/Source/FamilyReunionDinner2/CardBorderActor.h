// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CardBorderActor.generated.h"

UCLASS()
class FAMILYREUNIONDINNER2_API ACardBorderActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACardBorderActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	AActor* parent;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void appendCard(AActor* card);
};
