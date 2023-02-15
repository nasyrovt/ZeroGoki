// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProjectileBase.generated.h"

UCLASS()
class ZEROGOKI_API AProjectileBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectileBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Destroyed() override;

	UFUNCTION()
		void OnHit(UPrimitiveComponent* HitComp,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			FVector NormalImpulse,
			const FHitResult& Hit);


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Sphere component used to test collision.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		class USphereComponent* SphereComponent;

	// Static Mesh used to provide a visual representation of the object.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		class UStaticMeshComponent* StaticMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		class UProjectileMovementComponent* ProjectileMovementComponent;

	//The damage type and damage that will be done by this projectile
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Damage")
		TSubclassOf<class UDamageType> DamageType;

	//The damage dealt by this projectile.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Damage")
		float Damage;

	// Particle used when the projectile impacts against another object and explodes.
	UPROPERTY(EditAnywhere, Category = "Effects")
		class UNiagaraSystem* ExplosionEffect;

	//The damage dealt by this projectile.
	UPROPERTY(EditAnywhere, Category = "Effects")
		float ProjectileHeatAmount;
};
