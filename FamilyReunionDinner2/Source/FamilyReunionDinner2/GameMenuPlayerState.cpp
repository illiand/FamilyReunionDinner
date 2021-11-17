// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMenuPlayerState.h"
#include "Net/UnrealNetwork.h"
#include "Misc/OutputDeviceNull.h"
#include "GameMenuLevelScriptActor.h"

void AGameMenuPlayerState::BeginPlay() 
{
	Super::BeginPlay();

	if (GetLocalRole() != 3 && GetOwner() != NULL)
	{
		this->generateFamilyReunionDinner2UserID();
	}
}

void AGameMenuPlayerState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AGameMenuPlayerState, serverListNeedUpdate);
	DOREPLIFETIME(AGameMenuPlayerState, serverInfo);
}

void AGameMenuPlayerState::updateComplete_Implementation() 
{
	serverListNeedUpdate = false;
}

void AGameMenuPlayerState::generateFamilyReunionDinner2UserID_Implementation()
{
	FamilyReunionDinner2PlayerID = FMath::RandRange(10000000, 99999999);
	setFamilyReunionDinner2UserID(FamilyReunionDinner2PlayerID);
}

void AGameMenuPlayerState::setFamilyReunionDinner2UserID_Implementation(int id)
{
	FamilyReunionDinner2PlayerID = id;
	call_createMainMenu();
}

void AGameMenuPlayerState::call_createMainMenu() 
{
	FOutputDeviceNull ar;

	if (GetLevel() != NULL)
	{
		AGameMenuLevelScriptActor* levelBluePrint = Cast<AGameMenuLevelScriptActor>(GetLevel()->GetLevelScriptActor());
		levelBluePrint->CallFunctionByNameWithArguments(TEXT("initMainMenu"), ar, NULL, true);
	}	
}
