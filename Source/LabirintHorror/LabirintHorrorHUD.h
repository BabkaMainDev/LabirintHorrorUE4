// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once 

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "LabirintHorrorHUD.generated.h"

UCLASS()
class ALabirintHorrorHUD : public AHUD
{
	GENERATED_BODY()

public:
	ALabirintHorrorHUD();

	/** Primary draw call for the HUD */
	virtual void DrawHUD() override;

private:
	/** Crosshair asset pointer */
	class UTexture2D* CrosshairTex;

};

