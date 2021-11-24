// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FamilyReunionDinner2UserWidget.generated.h"

/**
 * 
 */
UCLASS()
class FAMILYREUNIONDINNER2_API UFamilyReunionDinner2UserWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintImplementableEvent)
	void showPotInfo(const FString& path, int32 currentFlavor, int32 currentHeat, int32 currentSize, int32 maxSize);
	UFUNCTION(BlueprintImplementableEvent)
	void clearUI();
	UFUNCTION(BlueprintImplementableEvent)
	void showHintLayout();
};
