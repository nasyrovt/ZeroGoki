// Fill out your copyright notice in the Description page of Project Settings.


#include "ShipMovement.h"
#include "Components/PrimitiveComponent.h"

// Sets default values for this component's properties
UShipMovement::UShipMovement()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	
}
/// <summary>
/// 
/// </summary>
void UShipMovement::Initialize()
{
	MinimumThrustSpeed = 0.f;
	MaximumThrustSpeed = 25000.f;
	TargetThrustSpeed = 0.f;
	CurrentThrustSpeed = 0.f;
	bIsBoosting = false;
	BoostAmountLevel = 1.f;
	BoostThrustSpeed = 50000.f;
	FlightTurnRate = 45.f;
	bIsInvertVerticalAxis = true;
	CurrentMoveForwardInput = 0.f;
}


// Called when the game starts
void UShipMovement::BeginPlay()
{
	Super::BeginPlay();
	
	Initialize();
	
}


// Called every frame
void UShipMovement::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
}


#pragma region Move Creation and Simulation


/// <summary>
///		Creates a move from the current inputs
/// </summary>
/// <param name="DeltaTime"></param>
/// <returns> A FShipMove struct containing all of inputs, deltaTime and world time of inputs</returns>
FShipMove UShipMovement::CreateMove(float DeltaTime)
{
	FShipMove Move;
	Move._bIsBoosting = bIsBoosting;
	Move._YawInput = FlightYaw;
	Move._RollInput = FlightRoll;
	Move._PitchInput = FlightPitch;
	Move._CurrentSpeed = CurrentThrustSpeed;
	Move._TargetSpeed = TargetThrustSpeed;
	//Here we should replicate WorldTimeSeconds from GameStateBase
	Move._StartTime = GetWorld()->TimeSeconds;
	Move._DeltaTime = DeltaTime;

	return Move;
}


/// <summary>
/// Replays (simulates a Move from the struct)
/// </summary>
/// <param name="LastMove" type="FShipMove"></param>
void UShipMovement::SimulateMove(const FShipMove& LastMove)
{	
	//Rotation
	SimulateYawInput(LastMove._YawInput, LastMove._DeltaTime);
	SimulatePitchInput(LastMove._PitchInput, LastMove._DeltaTime);
	SimulateRollInput(LastMove._RollInput, LastMove._DeltaTime);
	//Move Forward
	MoveForward(LastMove._CurrentSpeed, LastMove._TargetSpeed, LastMove._DeltaTime);
	UseBoostAmount(LastMove._bIsBoosting, LastMove._DeltaTime);
}

#pragma endregion

#pragma region Boost

/// <summary>
/// Stop the Boost on release left shift
/// </summary>
void UShipMovement::StopBoost() 
{
	this->bIsBoosting = false;
	MaximumThrustSpeed = 25000.f;
	TargetThrustSpeed = 25000.f;
}

/// <summary>
/// Uses Boost amount left and regenerates it depending on boolean parameter
/// </summary>
/// <param name="IsBoosting">Decrease/Increase the boost amount 
///			if player has pressed/released left shift</param>
void UShipMovement::UseBoostAmount(bool IsBoosting, float _DeltaTime)
{
	float targetBoostAmount = IsBoosting ? 0.f : 1.f;
	float interpSpeed = IsBoosting ? 0.1f : 0.05f;
	BoostAmountLevel = FMath::FInterpConstantTo(
		BoostAmountLevel,
		targetBoostAmount,
		_DeltaTime,
		interpSpeed);

	if (BoostAmountLevel == 0.f) {
		MaximumThrustSpeed = 25000.f;
		TargetThrustSpeed = 25000.f;
		this->bIsBoosting = false;
	}
}

/// <summary>
/// Enters Boost Speed Mode if left shift is pressed and has >15% of boost amount left
/// </summary>
void UShipMovement::SupersonicBoost()
{
	if (BoostAmountLevel <= .15f) return;

	bIsBoosting = true;
	MaximumThrustSpeed = BoostThrustSpeed;
	TargetThrustSpeed = BoostThrustSpeed;
	CurrentThrustSpeed = BoostThrustSpeed;
}


#pragma endregion

#pragma region MoveForward

/// <summary>
///		Pushes a ship in forward direction with interpolated speed from Current to Target
/// </summary>
/// <param name="CurrentSpeed"></param>
/// <param name="TargetSpeed"></param>
void UShipMovement::MoveForward(float CurrentSpeed, float TargetSpeed, float _DeltaTime)
{
	CurrentThrustSpeed = FMath::FInterpTo(
		CurrentSpeed,
		TargetSpeed,
		_DeltaTime,
		8.f);

	GetOwner()->AddActorWorldOffset(
		GetOwner()->GetActorForwardVector() * FVector(CurrentSpeed * _DeltaTime),
		true);
}

/// <summary>
///		Sets Target Speed
/// </summary>
/// <param name="Value"></param>
void UShipMovement::SetTargetSpeed(float Value)
{
	CurrentMoveForwardInput = Value;
	if (bIsBoosting) return;

	TargetThrustSpeed = FMath::Clamp((GetWorld()->GetDeltaSeconds() * 6000 * Value) + TargetThrustSpeed,
		MinimumThrustSpeed, 
		MaximumThrustSpeed);
}

#pragma endregion


#pragma region Rotation

void UShipMovement::SimulatePitchInput(float Value, float _DeltaTime)
{
	float newPitch = FlightTurnRate * _DeltaTime * Value;
	GetOwner()->AddActorLocalRotation(FRotator(newPitch, 0.f, 0.f));
}

void UShipMovement::SimulateYawInput(float Value, float _DeltaTime)
{
	float newYaw = FlightTurnRate * _DeltaTime * Value;
	GetOwner()->AddActorLocalRotation(FRotator(0.f, newYaw, 0.f));
}

void UShipMovement::SimulateRollInput(float Value, float _DeltaTime)
{
	float newRoll = 90 * _DeltaTime * Value;
	GetOwner()->AddActorLocalRotation(FRotator(0.f, 0.f, newRoll));
}


void UShipMovement::SetFlightYaw(float Value) 
{
	FlightYaw = FMath::FInterpTo(FlightYaw, Value, GetWorld()->GetDeltaSeconds(), 10.f);
}

void UShipMovement::SetFlightRoll(float Value) 
{
	FlightRoll = FMath::FInterpTo(FlightRoll, Value, GetWorld()->GetDeltaSeconds(), 10.f);
}

void UShipMovement::SetFlightPitch(float Value) 
{
	float verticalAxisCoeff = bIsInvertVerticalAxis ? 1.f : -1.f;
	FlightPitch = FMath::FInterpTo(
		FlightPitch,
		Value * verticalAxisCoeff,
		GetWorld()->GetDeltaSeconds(),
		10.f);
}

#pragma endregion


