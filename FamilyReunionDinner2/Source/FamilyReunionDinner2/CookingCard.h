// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CookingCardStruct.h"
#include "Components/TextRenderComponent.h"
#include "Kismet/GameplayStatics.h"
#include "CookingCard.generated.h"

UCLASS()
class FAMILYREUNIONDINNER2_API ACookingCard : public AActor
{
	GENERATED_BODY()

private:
	UMaterialInstanceDynamic* material;

public:
	// Sets default values for this actor's properties
	ACookingCard();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(Replicated)
	AActor* border;

	UPROPERTY(Replicated, BlueprintReadOnly)
	FCookingCardStruct data;

	UFUNCTION(Client, Reliable)
	void assignInfo();
	void assignInfo_Implementation();

	UFUNCTION(NetMulticast, Reliable)
	void changeDegreeHintStatus(bool isOn);
	void changeDegreeHintStatus_Implementation(bool isOn);

private:
	UMaterialInstanceDynamic* hintFrontTextMaterial;
	UMaterialInstanceDynamic* hintTextMaterial;
};
