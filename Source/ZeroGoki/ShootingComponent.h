// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ProjectileBase.h"

#include "ShootingComponent.generated.h"

UENUM() enum FireMode
{
	Rafale     UMETA(DisplayName = "Rafale"),
	SemiAuto   UMETA(DisplayName = "SemiAuto")
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class ZEROGOKI_API UShootingComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UShootingComponent();

	/** Function for beginning weapon fire.*/
	UFUNCTION(BlueprintCallable, Category = "Shooting")
		void StartFire();

	/** Function for ending weapon fire. Once this is called, the player can use StartFire again.*/
	UFUNCTION(BlueprintCallable, Category = "Shooting")
		void StopFire();

	void HandleFireRafale_Implementation();


protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
		TEnumAsByte<FireMode> mode;

	UPROPERTY(EditAnywhere, Category = "Combat")
		float WeaponHeatLimit;

	UPROPERTY(EditAnywhere, Category = "Combat")
		float CurrentWeaponHeatAmount;

	UPROPERTY(EditAnywhere, Category = "Combat")
		float WeaponHeatLevel;

	UPROPERTY(EditAnywhere, Category = "Combat")
		float WeaponChillingMultiplier;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
		TSubclassOf<AProjectileBase> ProjectileClass;

	/** Delay between shots in seconds. Used to control fire rate for your test projectile, but also to prevent an overflow of server functions from binding SpawnProjectile directly to input.*/
	UPROPERTY(EditDefaultsOnly, Category = "Shooting")
		float FireRate;

	/** If true, you are in the process of firing projectiles. */
	bool bIsFiringWeapon;

	int nbProjectile;

	class UCameraComponent* CameraComp;

	/** Server function for spawning projectiles.*/
	UFUNCTION(Server, Reliable)
		void HandleFire();

	UFUNCTION(Server, Reliable)
		void HandleFireRafale();

	/** A timer handle used for providing the fire rate delay in-between spawns.*/
	FTimerHandle FiringTimer;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SetCameraComponent(UCameraComponent* CameraComponent);

#pragma region Combat

	UFUNCTION(BlueprintCallable, Category = "Combat")
		FORCEINLINE float GetCurrentHeat() const { return CurrentWeaponHeatAmount; }

	UFUNCTION(BlueprintCallable, Category = "Combat")
		FORCEINLINE float GetMaxHeat() const { return WeaponHeatLimit; }

#pragma endregion
};
