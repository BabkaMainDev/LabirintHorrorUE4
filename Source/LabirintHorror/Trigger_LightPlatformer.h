// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Trigger_LightPlatformer.generated.h"

UCLASS()
class LABIRINTHORROR_API ATrigger_LightPlatformer : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATrigger_LightPlatformer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UBoxComponent* Trigger_LightPlatformerComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<class ALight_Trigger*> ListOfLights;

	UFUNCTION()
		void Triggered(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void UnTriggered(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex);

};
