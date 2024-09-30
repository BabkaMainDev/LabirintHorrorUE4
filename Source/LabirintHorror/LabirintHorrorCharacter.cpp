// Copyright Epic Games, Inc. All Rights Reserved.

#include "LabirintHorrorCharacter.h"
#include "LabirintHorrorProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/InputSettings.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "MotionControllerComponent.h"
#include "XRMotionControllerBase.h" // for FXRMotionControllerBase::RightHandSourceId
#include "Components/SpotLightComponent.h"
#include "Blueprint/UserWidget.h"
#include <Runtime/Engine/Public/DrawDebugHelpers.h>
#include "Blueprint/UserWidget.h"

DEFINE_LOG_CATEGORY_STATIC(LogFPChar, Warning, All);

//////////////////////////////////////////////////////////////////////////
// ALabirintHorrorCharacter WidgetBlueprint'/Game/Widget/WBP_Death.WBP_Death'

ALabirintHorrorCharacter::ALabirintHorrorCharacter()
{
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-39.56f, 1.75f, 64.f));
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	Flashlight = CreateDefaultSubobject<USpotLightComponent>(TEXT("Flashlight"));
	Flashlight->SetupAttachment(FirstPersonCameraComponent);
	Flashlight->SetIntensity(10000.0f);
	Flashlight->SetVisibility(false);

	bFlashlightOn = false;

	bDead = false;
	GetCharacterMovement()->SetMovementMode(MOVE_Walking);

	CurrentStamina = 100;
	DefaultStamina = 100;

	FlashlightCharge = 100;
	FlashlightDefaultCharge = 100;

	static ConstructorHelpers::FClassFinder<UUserWidget> WidgetAsset(TEXT("/Game/Widget/WBP_Death"));
	if (WidgetAsset.Succeeded())
	{
		WidgetClass = WidgetAsset.Class;
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> WinWidgetAsset(TEXT("/Game/Widget/WBP_Win"));
	if (WinWidgetAsset.Succeeded())
	{
		WinWidgetClass = WinWidgetAsset.Class;
	}


}

void ALabirintHorrorCharacter::BeginPlay()
{
	Super::BeginPlay();

	bFlashlightOn = false;

	bDead = false;
	GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	APlayerController* Mouse = UGameplayStatics::GetPlayerController(this, 0);
	if (Mouse)
	{
		Mouse->bShowMouseCursor = false;
		Mouse->SetInputMode(FInputModeGameOnly());
	}

	SpawnLocation = GetActorLocation();
	SpawnRotation = GetActorRotation();
}

void ALabirintHorrorCharacter::Respawn()
{
	SetActorLocation(SpawnLocation);
	SetActorRotation(SpawnRotation);
	CurrentStamina = DefaultStamina;
	FlashlightCharge = FlashlightDefaultCharge;
	bDead = false;

	GetCharacterMovement()->SetMovementMode(MOVE_Walking);

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	if (PlayerController)
	{
		PlayerController->bShowMouseCursor = false;
		PlayerController->SetInputMode(FInputModeGameOnly());
	}

	// Убираем меню смерти
	if (ActiveWidget)
	{
		ActiveWidget->RemoveFromParent();
		ActiveWidget = nullptr;
	}
}

void ALabirintHorrorCharacter::Die()
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	if (PlayerController)
	{
		PlayerController->bShowMouseCursor = true;
		PlayerController->SetInputMode(FInputModeUIOnly());
	}

	bDead = true;
	if (WidgetClass)
	{
		ActiveWidget = CreateWidget<UUserWidget>(GetWorld(), WidgetClass);
		if (ActiveWidget)
		{
			ActiveWidget->AddToViewport();
		}
	}
	GetCharacterMovement()->DisableMovement();
}

void ALabirintHorrorCharacter::Win()
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	if (PlayerController)
	{
		PlayerController->bShowMouseCursor = true;
		PlayerController->SetInputMode(FInputModeUIOnly());
	}

	bDead = true;

	if (WinWidgetClass)
	{
		ActiveWidget = CreateWidget<UUserWidget>(GetWorld(), WinWidgetClass);
		if (ActiveWidget)
		{
			ActiveWidget->AddToViewport();
		}
	}
	GetCharacterMovement()->DisableMovement();
}


void ALabirintHorrorCharacter::ToggleFlashlight()
{
	// Переключаем состояние фонаря (включен/выключен)
	bFlashlightOn = !bFlashlightOn;

	if (bFlashlightOn && FlashlightCharge > 0)
	{
		Flashlight->SetVisibility(true);
		DecreaseFlashlightCharge();
	}
	else
	{
		Flashlight->SetVisibility(false);
		IncreaseFlashlightCharge();
	}
}

void ALabirintHorrorCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bDead)
	{
		if (IsPlayerControlled())
		{
			Collide();
		}

		if (bIsRunning && CurrentStamina > 0)
		{
			DecreaseStamina();
		}
		else if (!bIsRunning && CurrentStamina < 100)
		{
			IncreaseStamina();
		}

		if (bFlashlightOn && FlashlightCharge > 0)
		{
			DecreaseFlashlightCharge();
		}
		else if (!bFlashlightOn && FlashlightCharge < 100)
		{
			IncreaseFlashlightCharge();
		}
		if (FlashlightCharge <= 0)
		{
			//bFlashlightOn = false;
			bCanCharge = false;
			Flashlight->SetVisibility(false);
			//IncreaseFlashlightCharge();
		}
	}
}

void ALabirintHorrorCharacter::DecreaseFlashlightCharge()
{
	FlashlightCharge -= 0.1f; // Уменьшение заряда аналогично стамине
	Flashlight->SetIntensity(FlashlightCharge * 100); // Интенсивность пропорциональна заряду
}

void ALabirintHorrorCharacter::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}

void ALabirintHorrorCharacter::IncreaseFlashlightCharge()
{
	if (bCanCharge)
	{
		FlashlightCharge += 0.08f;
		if (FlashlightCharge > 100.0f)
		{
			FlashlightCharge = 100.0f;
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// Input

void ALabirintHorrorCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	check(PlayerInputComponent);

	// Bind jump events
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("Run", IE_Pressed, this, &ALabirintHorrorCharacter::Runing);
	PlayerInputComponent->BindAction("Run", IE_Released, this, &ALabirintHorrorCharacter::StopRuning);

	// Enable touchscreen input
	EnableTouchscreenMovement(PlayerInputComponent);

	PlayerInputComponent->BindAction("ToggleFlashlight", IE_Pressed, this, &ALabirintHorrorCharacter::ToggleFlashlight);

	// Bind movement events
	PlayerInputComponent->BindAxis("MoveForward", this, &ALabirintHorrorCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ALabirintHorrorCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &ALabirintHorrorCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ALabirintHorrorCharacter::LookUpAtRate);
}


void ALabirintHorrorCharacter::BeginTouch(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	if (TouchItem.bIsPressed == true)
	{
		return;
	}
	if ((FingerIndex == TouchItem.FingerIndex) && (TouchItem.bMoved == false))
	{
		return;
	}
	TouchItem.bIsPressed = true;
	TouchItem.FingerIndex = FingerIndex;
	TouchItem.Location = Location;
	TouchItem.bMoved = false;
}

void ALabirintHorrorCharacter::EndTouch(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	if (TouchItem.bIsPressed == false)
	{
		return;
	}
	TouchItem.bIsPressed = false;
}

void ALabirintHorrorCharacter::Collide()
{
	FVector TraceStart = GetActorLocation();
	FRotator CharRotation = GetActorRotation();
	FVector TraceEnd = TraceStart + (CharRotation.Vector() * 100.f);

	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Blue, false, 1, 0, 1);

	bool bIsHit = GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility, QueryParams);

	if (bIsHit)
	{
		AActor* Target = HitResult.GetActor();
		if (Target && Target != this)
		{
			if (Target->ActorHasTag(FName(TEXT("Wall"))) || (Target->ActorHasTag(FName(TEXT("Guard"))) && Target != this))
			{
				Die();
			}

			if (Target->ActorHasTag(FName(TEXT("Treasure"))) && Target != this)
			{
				Win();
			}
		}
	}
}

void ALabirintHorrorCharacter::MoveForward(float Value)
{
	if (Value != 0.0f && !bDead)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void ALabirintHorrorCharacter::MoveRight(float Value)
{
	if (Value != 0.0f && !bDead)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void ALabirintHorrorCharacter::TurnAtRate(float Rate)
{
	if (!bDead)
	{
		// calculate delta for this frame from the rate information
		AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
	}
}

void ALabirintHorrorCharacter::Runing()
{
	bIsRunning = true;
	if (CurrentStamina >= 10)
	{
		GetCharacterMovement()->MaxWalkSpeed = 545;
	}
	else if (CurrentStamina >= 10 && CurrentStamina < 1)
	{
		GetCharacterMovement()->MaxWalkSpeed = 308;
	}
	else if (CurrentStamina <= 0)
	{
		GetCharacterMovement()->MaxWalkSpeed = 0;
	}

	if (CurrentStamina > 0)
	{
		DecreaseStamina();
	}
}

void ALabirintHorrorCharacter::StopRuning()
{
	bIsRunning = false;
	GetCharacterMovement()->MaxWalkSpeed = 308;
	IncreaseStamina();
}


void ALabirintHorrorCharacter::DecreaseStamina()
{
	CurrentStamina -= 0.15;
}
void ALabirintHorrorCharacter::IncreaseStamina()
{
	CurrentStamina += 0.05;
}

void ALabirintHorrorCharacter::LookUpAtRate(float Rate)
{
	if (!bDead)
	{
		// calculate delta for this frame from the rate information
		AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
	}
}

bool ALabirintHorrorCharacter::EnableTouchscreenMovement(class UInputComponent* PlayerInputComponent)
{
	if (FPlatformMisc::SupportsTouchInput() || GetDefault<UInputSettings>()->bUseMouseForTouch)
	{
		PlayerInputComponent->BindTouch(EInputEvent::IE_Pressed, this, &ALabirintHorrorCharacter::BeginTouch);
		PlayerInputComponent->BindTouch(EInputEvent::IE_Released, this, &ALabirintHorrorCharacter::EndTouch);

		//Commenting this out to be more consistent with FPS BP template.
		//PlayerInputComponent->BindTouch(EInputEvent::IE_Repeat, this, &ALabirintHorrorCharacter::TouchUpdate);
		return true;
	}
	
	return false;
}
