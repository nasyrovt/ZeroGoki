// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "ShipMovement.h"
#include "HealthComponent.h"
#include "ShootingComponent.h"

#include "ShipReplicator.generated.h"



USTRUCT()
struct FShipState
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
		FShipMove _LastMove;

	UPROPERTY()
		float _Health;

	UPROPERTY()
		float _Shield;

	UPROPERTY()
		float _WeaponHeatLevel;

	UPROPERTY()
		FTransform _Transform;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ZEROGOKI_API UShipReplicator : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UShipReplicator();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:

	TArray<FShipMove> UnacknolegedMoves;

	void ClearAcknolegedMoves(FShipMove LastMove);

	UFUNCTION(Server, Reliable, WithValidation)
		void Server_SendMove(FShipMove Move);

	UPROPERTY(ReplicatedUsing = OnRep_ServerState)
		FShipState ServerState;

	UFUNCTION()
		void OnRep_ServerState();

	UPROPERTY()
		UShipMovement* MovementComponent;
	UPROPERTY()
		UHealthComponent* HealthComponent;
	UPROPERTY()
		UShootingComponent* ShootingComponent;
};
