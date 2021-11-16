// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMenuPlayerState.h"
#include "Net/UnrealNetwork.h"

void AGameMenuPlayerState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AGameMenuPlayerState, serverListNeedUpdate);
	DOREPLIFETIME(AGameMenuPlayerState, serverInfo);
	DOREPLIFETIME(AGameMenuPlayerState, FamilyReunionDinner2PlayerID);
}

void AGameMenuPlayerState::updateComplete_Implementation() 
{
	serverListNeedUpdate = false;
}