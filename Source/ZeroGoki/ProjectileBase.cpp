// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileBase.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/DamageType.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Niagara/Public/NiagaraComponent.h"
#include "Niagara/Public/NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

// Sets default values
AProjectileBase::AProjectileBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
	
	
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Projectile Collision"));
	SphereComponent->InitSphereRadius(10.0f);
	SphereComponent->SetCollisionProfileName(TEXT("BlockAllDynamic"));
	RootComponent = SphereComponent;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile Mesh"));
	StaticMesh->SetupAttachment(RootComponent);

	if (GetLocalRole() == ROLE_Authority)
	{
		SphereComponent->OnComponentHit.AddDynamic(this, &AProjectileBase::OnHit);
	}

	//Definition for the Projectile Movement Component.
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement"));
	ProjectileMovementComponent->SetUpdatedComponent(SphereComponent);
	ProjectileMovementComponent->InitialSpeed = 24000.0f;
	ProjectileMovementComponent->MaxSpeed = 50000.0f;
	//ProjectileMovementComponent->bInitialVelocityInLocalSpace = true;
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->ProjectileGravityScale = 0.0f;

	DamageType = UDamageType::StaticClass();
	Damage = 10.0f;

	ProjectileHeatAmount = 10.f;
}

// Called when the game starts or when spawned
void AProjectileBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProjectileBase::Destroyed()
{
	FVector spawnLocation = GetActorLocation();
	
	if (ExplosionEffect) {
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			this,
			ExplosionEffect,
			spawnLocation,
			FRotator::ZeroRotator,
			FVector(1),
			true,
			true,
			ENCPoolMethod::AutoRelease);
	}
	
}

void AProjectileBase::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	AActor* MyOwner = GetOwner();
	if (MyOwner == nullptr)
	{
		Destroy();
		return;
	}

	AController* MyOwnerInstigator = MyOwner->GetInstigatorController();
	UClass* DamageTypeClass = UDamageType::StaticClass();

	if (OtherActor && OtherActor != this && OtherActor != MyOwner)
	{
		UGameplayStatics::ApplyDamage(OtherActor, Damage, MyOwnerInstigator, this, DamageTypeClass);
		//TODO: Implement particles and sound when assets are ready
		/*if (HitParticles)
		{
			UGameplayStatics::SpawnEmitterAtLocation(
				this,
				HitParticles,
				GetActorLocation(),
				GetActorRotation());
		}
		if (HitSound)
		{
			UGameplayStatics::PlaySoundAtLocation(
				this,
				HitSound,
				GetActorLocation());
		}*/

	}
	Destroy();

}
