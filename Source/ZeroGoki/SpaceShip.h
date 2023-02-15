// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "ShipMovement.h"
#include "CameraManager.h"
#include "ShipReplicator.h"
#include "HealthComponent.h"
#include "ShootingComponent.h"
#include "SpaceShip.generated.h"



UCLASS()
class ZEROGOKI_API ASpaceShip : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ASpaceShip();

	UFUNCTION()
	USceneComponent* GetProjectileSpawnPoint() { return ProjectileSpawnPoint; }

	//Controls values
	void PitchRotation(float Value);
	void YawRotation(float Value);
	void RollRotation(float Value);
	void PassForwardValue(float Value);
	
	//Calling Boost logic
	void HandleSupersonicBoost();
	void StopSupersonicBoost();
	
	//Camera Related
	void HandleCameraSwap();

	//Shooting
	void HandleStartFire();
	void HandleStopFire();

	/** Event for taking damage. Overridden from APawn.*/
	UFUNCTION(BlueprintCallable, Category = "Health")
		float TakeDamage(float DamageTaken, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:

#pragma region SceneComponents

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* ShipMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
		USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
		UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
		USceneComponent* ProjectileSpawnPoint;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
		UNiagaraComponent* BoosterParticles;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
		UNiagaraComponent* ExaustParticles;


#pragma endregion

#pragma region ActorComponents

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
		UShipMovement* MovementComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
		UShipReplicator* ReplicatorComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
		UCameraManager* CameraManager;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
		UHealthComponent* HealthComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
		UShootingComponent* ShootingComponent;

#pragma endregion

};
