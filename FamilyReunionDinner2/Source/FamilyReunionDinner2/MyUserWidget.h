// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MyUserWidget.generated.h"


/**
 * 
 */
UCLASS()
class FAMILYREUNIONDINNER2_API UMyUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintImplementableEvent)
	void createRoomCallback(int32 id, const FString& name, int32 curPlayerNum, int32 maxPlayerNum);

	UFUNCTION(BlueprintImplementableEvent)
	void LeaveRoomCallback();

	UFUNCTION(BlueprintImplementableEvent)
	void addServerListUI(int32 id, const FString& name, int32 curPlayerNum, int32 maxPlayerNum);

	UFUNCTION(BlueprintImplementableEvent)
	void removeServerListUI(int32 id);

	UFUNCTION(BlueprintImplementableEvent)
	void addRoomPlayerUI(int32 id, const FString& name);

	UFUNCTION(BlueprintImplementableEvent)
	void removeRoomPlayerUI(int32 id);

	UFUNCTION(BlueprintImplementableEvent)
	void serverListRoomPlayerChangedUI(int32 id, int32 curPlayerNum, int32 maxPlayerNum);

	UFUNCTION(BlueprintImplementableEvent)
	void roomPlayerChangedUI(int32 curPlayerNum, int32 maxPlayerNum);
};
