// Fill out your copyright notice in the Description page of Project Settings.


#include "Light_Trigger.h"
#include "Components/RectLightComponent.h"

// Sets default values
ALight_Trigger::ALight_Trigger()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Light_TriggerComponent = CreateDefaultSubobject<URectLightComponent>(TEXT("Light_Rect"));
	Light_TriggerComponent->SetupAttachment(RootComponent);
	Light_TriggerComponent->Intensity = 100000.0f;
}

// Called when the game starts or when spawned
void ALight_Trigger::BeginPlay()
{
	Super::BeginPlay();

	SetLightDisabled();
}

// Called every frame
void ALight_Trigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALight_Trigger::SetLightEnabled()
{
	Light_TriggerComponent->SetVisibility(true);
}

void ALight_Trigger::SetLightDisabled()
{
	Light_TriggerComponent->SetVisibility(false);
}