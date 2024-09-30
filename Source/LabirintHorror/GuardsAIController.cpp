// Fill out your copyright notice in the Description page of Project Settings.

#include "GuardsAIController.h"
#include "NavigationSystem.h"
#include "TimerManager.h"

void AGuardsAIController::BeginPlay()
{
	Super::BeginPlay();

	NavigationMesh = FNavigationSystem::GetCurrent<UNavigationSystemV1>(this);

	RandomPatrol();
}

void AGuardsAIController::RandomPatrol()
{
	if (NavigationMesh && GetPawn())
	{
		FNavLocation NewRandomLocation;

		bool bFoundLocation = NavigationMesh->GetRandomReachablePointInRadius(GetPawn()->GetActorLocation(), 25000.f, NewRandomLocation);

		if (bFoundLocation)
		{
			this->RandomLocation = NewRandomLocation.Location;
			MoveToLocation(this->RandomLocation);
			GetWorld()->GetTimerManager().ClearTimer(PointTimer);
			GetWorld()->GetTimerManager().SetTimer(PointTimer, this, &AGuardsAIController::RandomPatrol, 2.f, false);
		}
	}
}

void AGuardsAIController::ClearTimer()
{
	GetWorld()->GetTimerManager().ClearTimer(PointTimer);
}

/*#include "GuardsAIController.h"
#include "NavigationSystem.h"

void AGuardsAIController::BeginPlay()
{
	Super::BeginPlay();

	NavigationMesh = FNavigationSystem::GetCurrent<UNavigationSystemV1>(this);

	RandomPatrol();
}

void AGuardsAIController::RandomPatrol()
{
	if (NavigationMesh)
	{
		NavigationMesh->K2_GetRandomReachablePointInRadius(GetWorld(), GetPawn()->GetActorLocation(), RandomLocation, 2500.f);

		MoveToLocation(RandomLocation);

		GetWorld()->GetTimerManager().SetTimer(PointTimer, this, &AGuardsAIController::RandomPatrol, 2.f, true, -1.f);
		ClearTimer();
	}
}

void AGuardsAIController::ClearTimer()
{
	GetWorld()->GetTimerManager().ClearTimer(PointTimer);
	RandomPatrol();
}
*/