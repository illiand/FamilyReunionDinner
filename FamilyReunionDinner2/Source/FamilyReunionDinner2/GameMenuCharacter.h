// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
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
	int playerLocation = 0;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	int getPlayerCurrentRoomIndex();

	UFUNCTION(Reliable, Server, BlueprintCallable)
	void createGame(AActor* host, const FString& serverName, int maxPlayerNum);
	void createGame_Implementation(AActor* host, const FString& serverName, int maxPlayerNum);

	UFUNCTION(Reliable, Server, BlueprintCallable)
	void startGame(int index);
	void startGame_Implementation(int index);

	UFUNCTION(Reliable, Server, BlueprintCallable)
	void joinRoom(int roomIndex);
	void joinRoom_Implementation(int roomIndex);

	UFUNCTION(Reliable, Server, BlueprintCallable)
	void leaveRoom();
	void leaveRoom_Implementation();

	UFUNCTION(Reliable, Client)
	void enterSession(int portNum);
	void enterSession_Implementation(int portNum);
};
