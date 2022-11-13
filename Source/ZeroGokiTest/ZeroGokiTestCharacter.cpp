// Copyright Epic Games, Inc. All Rights Reserved.

#include "ZeroGokiTestCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Net/UnrealNetwork.h"
#include "Engine/Engine.h"
#include "ThirdPersonMPProjectile.h"

//////////////////////////////////////////////////////////////////////////
// AZeroGokiTestCharacter

AZeroGokiTestCharacter::AZeroGokiTestCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rate for input
	TurnRateGamepad = 50.f;

	// Projectile spawning point configs init
	SpawnDirectionMultiplier = 100.f;
	SpawnUpVectorMultiplier = 50.f;

	//Weapon heat init
	WeaponHeatLimit = 100.f;
	WeaponHeatLevel = 0.f;
	WeaponChillingMultiplier = 5.f;
	// Default value for first projectile
	CurrentWeaponHeatAmount = 5.f; 

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = true;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = true;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 1500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)

	//Initialize the player's Health
	MaxHealth = 100.0f;
	CurrentHealth = MaxHealth;

	//Initialize projectile class
	ProjectileClass = AThirdPersonMPProjectile::StaticClass();
	//Initialize fire rate
	FireRate = 0.25f;
	bIsFiringWeapon = false;
}

void AZeroGokiTestCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	//Cooling weapon with time
	WeaponCooling(DeltaSeconds);


}

void AZeroGokiTestCharacter::WeaponCooling(float DeltaSeconds)
{
	WeaponHeatLevel -= DeltaSeconds * WeaponChillingMultiplier;
	if (WeaponHeatLevel <= 0) WeaponHeatLevel = 0;
}

//////////////////////////////////////////////////////////////////////////
// Input

void AZeroGokiTestCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	
	//PlayerInput Bindings (Currently implemented in BP)
	/*PlayerInputComponent->BindAxis("Move Forward / Backward", this, &AZeroGokiTestCharacter::MoveForward);*/
	/*PlayerInputComponent->BindAxis("Move Right / Left", this, &AZeroGokiTestCharacter::MoveRight);*/

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	
	/*PlayerInputComponent->BindAxis("Turn Right / Left Gamepad", this, &AZeroGokiTestCharacter::TurnAtRate);
	
	PlayerInputComponent->BindAxis("Look Up / Down Gamepad", this, &AZeroGokiTestCharacter::LookUpAtRate);*/

	// Handle firing projectiles
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AZeroGokiTestCharacter::StartFire);
}

//////////////////////////////////////////////////////////////////////////
// Replicated Properties

void AZeroGokiTestCharacter::GetLifetimeReplicatedProps(TArray <FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//Replicate current health.
	DOREPLIFETIME(AZeroGokiTestCharacter, CurrentHealth);
}

void AZeroGokiTestCharacter::OnHealthUpdate()
{
	//Client-specific functionality
	if (IsLocallyControlled())
	{
		FString healthMessage = FString::Printf(TEXT("You now have %f health remaining."), CurrentHealth);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, healthMessage);

		if (CurrentHealth <= 0.f)
		{
			FString deathMessage = FString::Printf(TEXT("You have been killed. You will respawn in 5 seconds."));
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, deathMessage);
		}
	}

	//Server-specific functionality
	if (GetLocalRole() == ROLE_Authority)
	{
		FString healthMessage = FString::Printf(TEXT("%s now has %f health remaining."), *GetFName().ToString(), CurrentHealth);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, healthMessage);
	}

	//Functions that occur on all machines. 
	if (CurrentHealth <= 0.f) {
		Respawn(); //TODO: Move to Client-only logic when building a project
	}
}

void AZeroGokiTestCharacter::Respawn()
{
	this->SetActorHiddenInGame(true);
	this->CurrentHealth = MaxHealth;

	float randomMultiplier = FMath::FRandRange(8000.f, 10000.f);
	FVector newPosition = FMath::VRand() * randomMultiplier;

	this->SetActorLocation(newPosition);
	this->SetActorHiddenInGame(false);
}


void AZeroGokiTestCharacter::OnRep_CurrentHealth()
{
	OnHealthUpdate();
}

void AZeroGokiTestCharacter::SetCurrentHealth(float healthValue)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		CurrentHealth = FMath::Clamp(healthValue, 0.f, MaxHealth);
		OnHealthUpdate();
	}
}

float AZeroGokiTestCharacter::TakeDamage(float DamageTaken, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float damageApplied = CurrentHealth - DamageTaken;
	SetCurrentHealth(damageApplied);
	return damageApplied;
}


void AZeroGokiTestCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * TurnRateGamepad * GetWorld()->GetDeltaSeconds());
}

void AZeroGokiTestCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * TurnRateGamepad * GetWorld()->GetDeltaSeconds());
}

void AZeroGokiTestCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is forward
		//const FRotator Rotation = Controller->GetControlRotation();
		//const FRotator YawRotation(0, Rotation.Yaw, 0);

		//// get forward vector
		//const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void AZeroGokiTestCharacter::MoveRight(float Value)
{
	if ( (Controller != nullptr) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void AZeroGokiTestCharacter::StartFire()
{
	if (!bIsFiringWeapon)
	{
		bIsFiringWeapon = true;
		UWorld* World = GetWorld();
		World->GetTimerManager().SetTimer(FiringTimer, this, &AZeroGokiTestCharacter::StopFire, FireRate, false);
		HandleFire();
	}
}

void AZeroGokiTestCharacter::StopFire()
{
	bIsFiringWeapon = false;
}

void AZeroGokiTestCharacter::HandleFire_Implementation()
{
	//Return if the projectile will overheat a weapon
	if (!(WeaponHeatLevel + CurrentWeaponHeatAmount <= WeaponHeatLimit)) return;

	//Get Spawn Properties
	FVector spawnLocation = GetActorLocation() + (GetActorRotation().Vector() * SpawnDirectionMultiplier) + (GetActorUpVector() * SpawnUpVectorMultiplier);
	FRotator spawnRotation =  GetActorRotation();

	FActorSpawnParameters spawnParameters;
	spawnParameters.Instigator = GetInstigator();
	spawnParameters.Owner = this;

	//Spawn Projectile Object
	AThirdPersonMPProjectile* spawnedProjectile = GetWorld()->SpawnActor<AThirdPersonMPProjectile>(spawnLocation, spawnRotation, spawnParameters);

	//Increase heat amount on the weapon
	CurrentWeaponHeatAmount = spawnedProjectile->ProjectileHeatAmount;
	WeaponHeatLevel += CurrentWeaponHeatAmount;
}
