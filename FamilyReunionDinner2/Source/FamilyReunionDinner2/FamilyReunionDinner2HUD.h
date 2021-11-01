// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once 

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "FamilyReunionDinner2Character.h"
#include "FamilyReunionDinner2HUD.generated.h"

UCLASS()
class AFamilyReunionDinner2HUD : public AHUD
{
	GENERATED_BODY()

public:
	AFamilyReunionDinner2HUD();

	/** Primary draw call for the HUD */
	virtual void DrawHUD() override;

private:
	/** Crosshair asset pointer */
	class UTexture2D* CrosshairTex;

};

