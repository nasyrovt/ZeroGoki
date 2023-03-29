// Fill out your copyright notice in the Description page of Project Settings.


#include "ShootingComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "SpaceShip.h"

// Sets default values for this component's properties
UShootingComponent::UShootingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;


	//Initialize fire rate
	FireRate = 0.2f;
	bIsFiringWeapon = false;
	nbProjectile = 0;
}


// Called when the game starts
void UShootingComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

}


// Called every frame
void UShootingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UShootingComponent::SetCameraComponent(UCameraComponent* CameraComponent)
{
	CameraComp = CameraComponent;
}

void UShootingComponent::StartFire()
{
	if (!bIsFiringWeapon)
	{
		UWorld* World = GetWorld();
		switch (mode)
		{
		case Rafale:
			bIsFiringWeapon = true;
			World->GetTimerManager().SetTimer(FiringTimer, this, &UShootingComponent::HandleFireRafale, 0.1f, true);
			break;
		case SemiAuto:
			bIsFiringWeapon = true;
			World->GetTimerManager().SetTimer(FiringTimer, this, &UShootingComponent::StopFire, FireRate, true);
			HandleFire();
			break;
		default:
			break;
		}
	}
}

void UShootingComponent::StopFire()
{
	bIsFiringWeapon = false;
}

void UShootingComponent::HandleFire_Implementation()
{

	//TODO: Return if the projectile will overheat a weapon
	//if (!(WeaponHeatLevel + CurrentWeaponHeatAmount <= WeaponHeatLimit)) return;

	//Get Spawn Properties
	if (!ensure(CameraComp != nullptr)) return;

	FVector Start = CameraComp->GetComponentLocation();
	FVector End = Start + CameraComp->GetForwardVector() * 35000.f;

	FHitResult OutHit;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(GetOwner());

	GetWorld()->LineTraceSingleByChannel(
		OutHit,
		Start,
		End,
		ECollisionChannel::ECC_Visibility,
		CollisionParams
	);
	FVector TargetLocation = OutHit.bBlockingHit ? OutHit.ImpactPoint : OutHit.TraceEnd;
	FVector SpawnPoint = Cast<ASpaceShip>(GetOwner())->GetProjectileSpawnPoint()->GetSocketLocation(TEXT("None"));
	FRotator ProjectileRotation = UKismetMathLibrary::FindLookAtRotation(SpawnPoint, TargetLocation);


	FActorSpawnParameters spawnParameters;
	spawnParameters.Instigator = GetOwner()->GetInstigator();
	spawnParameters.Owner = GetOwner();

	////Spawn Projectile Object
	/*static ConstructorHelpers::FClassFinder<AProjectileBase> Projectile(TEXT("/Game/Projectile/BP_Projectile.BP_Projectile"));
	auto projectileClass = Projectile.Class;*/
	if (ProjectileClass != nullptr) {
		AProjectileBase* Projectile = GetWorld()->SpawnActor<AProjectileBase>(ProjectileClass, SpawnPoint, ProjectileRotation);
		Projectile->SetOwner(GetOwner());
	}


	//TODO: Increase heat amount on the weapon
	//CurrentWeaponHeatAmount = spawnedProjectile->ProjectileHeatAmount;
	//WeaponHeatLevel += CurrentWeaponHeatAmount;
}

void UShootingComponent::HandleFireRafale_Implementation() {

	if (!ensure(CameraComp != nullptr)) return;

	FVector Start = CameraComp->GetComponentLocation();
	FVector End = Start + CameraComp->GetForwardVector() * 35000.f;

	FHitResult OutHit;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(GetOwner());

	GetWorld()->LineTraceSingleByChannel(
		OutHit,
		Start,
		End,
		ECollisionChannel::ECC_Visibility,
		CollisionParams
	);
	FVector TargetLocation = OutHit.bBlockingHit ? OutHit.ImpactPoint : OutHit.TraceEnd;
	FVector SpawnPoint = Cast<ASpaceShip>(GetOwner())->GetProjectileSpawnPoint()->GetSocketLocation(TEXT("None"));
	FRotator ProjectileRotation = UKismetMathLibrary::FindLookAtRotation(SpawnPoint, TargetLocation);


	FActorSpawnParameters spawnParameters;
	spawnParameters.Instigator = GetOwner()->GetInstigator();
	spawnParameters.Owner = GetOwner();



	////Spawn Projectile Object
	/*static ConstructorHelpers::FClassFinder<AProjectileBase> Projectile(TEXT("/Game/Projectile/BP_Projectile.BP_Projectile"));
	auto projectileClass = Projectile.Class;*/

	if (ProjectileClass != nullptr) {
		AProjectileBase* Projectile = GetWorld()->SpawnActor<AProjectileBase>(ProjectileClass, SpawnPoint, ProjectileRotation);
		Projectile->SetOwner(GetOwner());
	}
	nbProjectile++;
	if (nbProjectile == 3) {
		UWorld* World = GetWorld();
		World->GetTimerManager().ClearTimer(FiringTimer);
		nbProjectile = 0;
		StopFire();
	}
}


