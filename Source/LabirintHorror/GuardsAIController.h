// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Kismet/GameplayStatics.h"
#include "LabirintHorrorCharacter.h"
#include "Guard.h"
#include "GuardsAIController.generated.h"

UCLASS()
class LABIRINTHORROR_API AGuardsAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	void BeginPlay() override;

	class UNavigationSystemV1* NavigationMesh;

	FVector RandomLocation;

	FTimerHandle PointTimer;

	UFUNCTION()
	void RandomPatrol();

	void ClearTimer();

	AActor* Guard = UGameplayStatics::GetActorOfClass(GetWorld(), AGuard::StaticClass());
	AActor* Player = UGameplayStatics::GetActorOfClass(GetWorld(), ALabirintHorrorCharacter::StaticClass());
};
