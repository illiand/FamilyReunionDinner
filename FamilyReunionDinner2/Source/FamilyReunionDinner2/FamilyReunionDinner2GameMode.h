// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FamilyReunionDinner2GameMode.generated.h"

UCLASS(minimalapi)
class AFamilyReunionDinner2GameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AFamilyReunionDinner2GameMode();

public:
	virtual void PostLogin(APlayerController* NewPlayer);
	virtual void Logout(AController* Exiting);

	void initGameCountDown();
};



