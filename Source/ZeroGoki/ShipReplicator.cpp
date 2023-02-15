// Fill out your copyright notice in the Description page of Project Settings.


#include "ShipReplicator.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UShipReplicator::UShipReplicator()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);
	
}


// Called when the game starts
void UShipReplicator::BeginPlay()
{
	Super::BeginPlay();

	MovementComponent = GetOwner()->FindComponentByClass<UShipMovement>();
	HealthComponent = GetOwner()->FindComponentByClass<UHealthComponent>();
	
}


// Called every frame
void UShipReplicator::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (MovementComponent == nullptr) return;

	if (GetOwnerRole() == ROLE_AutonomousProxy) {
		FShipMove Move = MovementComponent->CreateMove(DeltaTime);
		MovementComponent->SimulateMove(Move);
		UnacknolegedMoves.Add(Move);
		Server_SendMove(Move);
	}
	if (GetOwnerRole() == ROLE_Authority /*&& GetOwner()->GetRemoteRole() == ROLE_SimulatedProxy*/)
	{
		FShipMove Move = MovementComponent->CreateMove(DeltaTime);
		Server_SendMove(Move);
	}
	if (GetOwnerRole() == ROLE_SimulatedProxy)
	{
		MovementComponent->SimulateMove(ServerState._LastMove);
	}
}

void UShipReplicator::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UShipReplicator, ServerState);

}


/// <summary>
///  Simulate Moves received from the server
/// </summary>
void UShipReplicator::OnRep_ServerState()
{
	if (MovementComponent == nullptr || HealthComponent == nullptr) return;

	GetOwner()->SetActorTransform(ServerState._Transform);
	HealthComponent->SetCurrentHealth(ServerState._Health);
	HealthComponent->SetCurrentShield(ServerState._Shield);

	ClearAcknolegedMoves(ServerState._LastMove);

	for (const FShipMove& Move : UnacknolegedMoves)
	{
		MovementComponent->SimulateMove(Move);
	}
}

/// <summary>
///  Sends move to the Server for verification
/// </summary>
/// <param name="Move"></param>
void UShipReplicator::Server_SendMove_Implementation(FShipMove Move)
{
	if (MovementComponent == nullptr || HealthComponent == nullptr) return;

	MovementComponent->SimulateMove(Move);

	ServerState._LastMove = Move;
	ServerState._Transform = GetOwner()->GetActorTransform();
	ServerState._Health = HealthComponent->GetCurrentHealth();
	ServerState._Shield = HealthComponent->GetCurrentShield();

}


/// <summary>
///		Disconect the Player if he is attempting to cheat
/// </summary>
/// <param name="Move">Last performed move</param>
/// <returns>Boolean</returns>
bool UShipReplicator::Server_SendMove_Validate(FShipMove Move)
{
	//TODO: Implement this, when all properties are chosen
	return true;

	/*
	if (MovementComponent == nullptr || HealthComponent == nullptr) return false;
	if (Move._bIsBoosting) 
	{
		if (Move._CurrentSpeed <= 45000 && Move._TargetSpeed <= 45000)
			return true;
		return false;
	}
	else 
	{
		if (Move._CurrentSpeed <= 25000 && Move._TargetSpeed <= 25000)
			return true;
		return false;
	}*/
}


void UShipReplicator::ClearAcknolegedMoves(FShipMove LastMove)
{
	TArray<FShipMove> NewMoves;

	for (const FShipMove& Move : UnacknolegedMoves)
	{
		if (Move._StartTime > LastMove._StartTime)
		{
			NewMoves.Add(Move);
		}
	}

	UnacknolegedMoves = NewMoves;
}

