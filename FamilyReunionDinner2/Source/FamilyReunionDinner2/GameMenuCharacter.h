// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MyUserWidget.h"
#include "GameMenuCharacter.generated.h"

UCLASS()
class FAMILYREUNIONDINNER2_API AGameMenuCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AGameMenuCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY(BlueprintReadWrite)
	UMyUserWidget* GameMenuUI;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	int findRoomIndexByID(int roomID);

	UFUNCTION(Reliable, Server, BlueprintCallable)
	void createGame(const FString& serverName, int maxPlayerNum);
	void createGame_Implementation(const FString& serverName, int maxPlayerNum);

	UFUNCTION(Reliable, Server, BlueprintCallable)
	void startGame(int roomID);
	void startGame_Implementation(int roomID);

	UFUNCTION(Reliable, Server, BlueprintCallable)
	void joinRoomRequest(int roomID);
	void joinRoomRequest_Implementation(int roomID);

	UFUNCTION(Reliable, Client)
	void joinRoomEnd(int id, const FString& name, int curNum, int maxNum);
	void joinRoomEnd_Implementation(int id, const FString& name, int curNum, int maxNum);

	UFUNCTION(Reliable, Server, BlueprintCallable)
	void leaveRoomRequest();
	void leaveRoomRequest_Implementation();

	UFUNCTION(Reliable, Client)
	void playerLeaveRoom();
	void playerLeaveRoom_Implementation();

	UFUNCTION(Reliable, Client)
	void enterSession(int portNum);
	void enterSession_Implementation(int portNum);
};
