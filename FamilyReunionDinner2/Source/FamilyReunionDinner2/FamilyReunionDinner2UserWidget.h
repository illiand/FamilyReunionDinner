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
	void showPotInfo(const FString& path, int32 currentFlavor, int32 currentHeat, int32 currentPoint, int32 currentSize, int32 maxSize, const TArray<FString>& addedItemInPotPath, const FString& actionPath, const FString& actionDes, const FVector& actionColor);
	UFUNCTION(BlueprintImplementableEvent)
	void clearUI();
	UFUNCTION(BlueprintImplementableEvent)
	void showHintLayout(const TArray<FString>& path);

	UFUNCTION(BlueprintImplementableEvent)
	void showFlavorHintPreviewUI(const TArray<int32>& index);
	UFUNCTION(BlueprintImplementableEvent)
	void showDegreeHintPreviewUI(const TArray<int32>& index);

	UFUNCTION(BlueprintImplementableEvent)
	void setMonsterPreferencePic(const FString& path);

	UFUNCTION(BlueprintImplementableEvent)
	void setTurnTimerUI(float remain, float total);
	UFUNCTION(BlueprintImplementableEvent)
	void setActionTimerUI(float remain, float total);

	UFUNCTION(BlueprintImplementableEvent)
	void setWaitingText(const FString& text);
	UFUNCTION(BlueprintImplementableEvent)
	void playWorldMessageText(const FString& text, const FVector& color);
};
