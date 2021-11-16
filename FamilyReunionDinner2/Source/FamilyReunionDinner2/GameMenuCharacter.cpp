// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMenuCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "GameMenuPlayerState.h"
#include "GameMenuGameMode.h"
#include "APIClass.h"


// Sets default values
AGameMenuCharacter::AGameMenuCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGameMenuCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGameMenuCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//UE_LOG(LogTemp, Warning, TEXT("bug?"));
}

// Called to bind functionality to input
void AGameMenuCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AGameMenuCharacter::createGame_Implementation(AActor* host, const FString& serverName, int maxPlayerNum)
{
	FServerInfoStruct newServerInfo;
	newServerInfo.name = serverName;
	newServerInfo.maxPlayerNum = maxPlayerNum;
	newServerInfo.currentPlayers.Add(Cast<AGameMenuCharacter>(host));

	Cast<AGameMenuGameMode>(GetWorld()->GetAuthGameMode())->serverInfo.Add(newServerInfo);
	
	AGameStateBase* gameState = GetWorld()->GetGameState();

	for (int i = 0; i < gameState->PlayerArray.Num(); i++)
	{
		Cast<AGameMenuPlayerState>(gameState->PlayerArray[i])->serverInfo = Cast<AGameMenuGameMode>(GetWorld()->GetAuthGameMode())->serverInfo;
		Cast<AGameMenuPlayerState>(gameState->PlayerArray[i])->serverListNeedUpdate = true;
	}
}

void AGameMenuCharacter::startGame_Implementation(int index) 
{
	Cast<AGameMenuGameMode>(GetWorld()->GetAuthGameMode())->openServer(index);
}

void AGameMenuCharacter::enterSession_Implementation(int portNum) 
{
	FString target = UAPIClass::getIPAddress().Append(TEXT(":")).Append(FString::FromInt(portNum));
	UGameplayStatics::OpenLevel(GetWorld(), FName(*target));
}

int AGameMenuCharacter::getPlayerCurrentRoomIndex() 
{
	TArray<FServerInfoStruct> serverInfo = Cast<AGameMenuPlayerState>(GetPlayerState())->serverInfo;

	for (int i = 0; i < serverInfo.Num(); i++)
	{
		for (int j = 0; j < serverInfo[i].currentPlayers.Num(); j++)
		{
			if(serverInfo[i].currentPlayers[j] == this)
			{
				return i;
			}
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("bug?"));

	return -1;
}

void AGameMenuCharacter::joinRoom_Implementation(int roomIndex) 
{
	Cast<AGameMenuGameMode>(GetWorld()->GetAuthGameMode())->serverInfo[roomIndex].currentPlayers.Add(this);
	AGameStateBase* gameState = GetWorld()->GetGameState();

	for (int i = 0; i < gameState->PlayerArray.Num(); i++)
	{
		Cast<AGameMenuPlayerState>(gameState->PlayerArray[i])->serverInfo = Cast<AGameMenuGameMode>(GetWorld()->GetAuthGameMode())->serverInfo;
		Cast<AGameMenuPlayerState>(gameState->PlayerArray[i])->serverListNeedUpdate = true;
	}
}

void AGameMenuCharacter::leaveRoom_Implementation()
{
	int roomIndex = getPlayerCurrentRoomIndex();
	
	Cast<AGameMenuGameMode>(GetWorld()->GetAuthGameMode())->serverInfo[roomIndex].currentPlayers.Remove(this);

	if (Cast<AGameMenuGameMode>(GetWorld()->GetAuthGameMode())->serverInfo[roomIndex].currentPlayers.Num() == 0) 
	{
		Cast<AGameMenuGameMode>(GetWorld()->GetAuthGameMode())->serverInfo.RemoveAt(roomIndex);
	}
	
	AGameStateBase* gameState = GetWorld()->GetGameState();

	for (int i = 0; i < gameState->PlayerArray.Num(); i++)
	{
		Cast<AGameMenuPlayerState>(gameState->PlayerArray[i])->serverInfo = Cast<AGameMenuGameMode>(GetWorld()->GetAuthGameMode())->serverInfo;
		Cast<AGameMenuPlayerState>(gameState->PlayerArray[i])->serverListNeedUpdate = true;
	}
}