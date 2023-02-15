// Fill out your copyright notice in the Description page of Project Settings.


#include "SpaceShip.h"

// Constructor
ASpaceShip::ASpaceShip()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ShipMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Ship Mesh"));
	RootComponent = ShipMesh;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Camera Boom"));
	SpringArm->SetupAttachment(ShipMesh);

	SpringArm->bEnableCameraRotationLag = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Projectile Spawn Point"));
	ProjectileSpawnPoint->SetupAttachment(ShipMesh);

	BoosterParticles = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Booster Particle System"));
	BoosterParticles->SetupAttachment(ShipMesh);
	
	ExaustParticles = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Exaust Particle System"));
	ExaustParticles->SetupAttachment(BoosterParticles);

	MovementComponent = CreateDefaultSubobject<UShipMovement>(TEXT("Movement Component"));
	CameraManager= CreateDefaultSubobject<UCameraManager>(TEXT("Camera Manager"));
	ReplicatorComponent = CreateDefaultSubobject<UShipReplicator>(TEXT("Replicator Component"));
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health Component"));
	ShootingComponent = CreateDefaultSubobject<UShootingComponent>(TEXT("Shooting Component"));

	ShootingComponent->SetCameraComponent(Camera);
}

// Called when the game starts or when spawned
void ASpaceShip::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASpaceShip::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

// Called to bind functionality to input
void ASpaceShip::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	UE_LOG(LogTemp, Warning, TEXT("Setup Player Input"));
	PlayerInputComponent->BindAxis("LookUp", this, &ASpaceShip::PitchRotation);
	PlayerInputComponent->BindAxis("Turn", this, &ASpaceShip::YawRotation);
	PlayerInputComponent->BindAxis("Move Right", this, &ASpaceShip::RollRotation);
	PlayerInputComponent->BindAxis("Move Forward", this, &ASpaceShip::PassForwardValue);
	PlayerInputComponent->BindAction("Supersonic", EInputEvent::IE_Pressed, this, &ASpaceShip::HandleSupersonicBoost);
	PlayerInputComponent->BindAction("Supersonic", EInputEvent::IE_Released, this, &ASpaceShip::StopSupersonicBoost);
	PlayerInputComponent->BindAction("Swap Camera View", EInputEvent::IE_Released, this, &ASpaceShip::HandleCameraSwap);
	PlayerInputComponent->BindAction("Fire", EInputEvent::IE_Pressed, this, &ASpaceShip::HandleStartFire);
	PlayerInputComponent->BindAction("Fire", EInputEvent::IE_Released, this, &ASpaceShip::HandleStopFire);

}

float ASpaceShip::TakeDamage(float DamageTaken, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (HealthComponent == nullptr) return 0.f;
	float damageApplied = HealthComponent->GetCurrentHealth() - DamageTaken;
	HealthComponent->SetCurrentHealth(damageApplied);
	return damageApplied;
}

void ASpaceShip::PitchRotation(float Value)
{
	if (MovementComponent == nullptr) return;

	MovementComponent->SetFlightPitch(Value);
}

void ASpaceShip::YawRotation(float Value)
{
	if (MovementComponent == nullptr) return;

	MovementComponent->SetFlightYaw(Value);
}

void ASpaceShip::RollRotation(float Value)
{
	if (MovementComponent == nullptr) return;

	MovementComponent->SetFlightRoll(Value);
}

void ASpaceShip::PassForwardValue(float Value) 
{
	if (MovementComponent == nullptr) return;

	MovementComponent->SetTargetSpeed(Value);
}

void ASpaceShip::HandleSupersonicBoost()
{
	if (MovementComponent == nullptr) return;
	
	MovementComponent->SupersonicBoost();

}

void ASpaceShip::StopSupersonicBoost() 
{
	if (MovementComponent == nullptr) return;

	MovementComponent->StopBoost();
}

void ASpaceShip::HandleCameraSwap()
{
	if (CameraManager == nullptr) return;

	CameraManager->SwapCameraView();
}

void ASpaceShip::HandleStartFire()
{
	if (ShootingComponent == nullptr) return;

	ShootingComponent->StartFire();
}

void ASpaceShip::HandleStopFire()
{
	if (ShootingComponent == nullptr) return;

	ShootingComponent->StopFire();
}

