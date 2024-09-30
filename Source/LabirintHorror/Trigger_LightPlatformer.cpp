// Fill out your copyright notice in the Description page of Project Settings.


#include "Trigger_LightPlatformer.h"
#include "Containers/Array.h"
#include "Light_Trigger.h"

// Sets default values
ATrigger_LightPlatformer::ATrigger_LightPlatformer()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Trigger_LightPlatformerComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger_LightPlatformer"));
	Trigger_LightPlatformerComponent->SetupAttachment(RootComponent);
	Trigger_LightPlatformerComponent->OnComponentBeginOverlap.AddDynamic(this, &ATrigger_LightPlatformer::Triggered);
	Trigger_LightPlatformerComponent->OnComponentEndOverlap.AddDynamic(this, &ATrigger_LightPlatformer::UnTriggered);
}

// Called when the game starts or when spawned
void ATrigger_LightPlatformer::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ATrigger_LightPlatformer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATrigger_LightPlatformer::Triggered(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComponent != nullptr))
	{
		for (ALight_Trigger* Light : ListOfLights)
		{
			Light->SetLightEnabled();
		}
	}
}

void ATrigger_LightPlatformer::UnTriggered(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex)
{
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComponent != nullptr))
	{
		for (ALight_Trigger* Light : ListOfLights)
		{
			Light->SetLightDisabled();
		}
	}
}