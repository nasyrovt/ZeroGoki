// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ShipMovement.generated.h"


USTRUCT()
struct FShipMove
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
		float _PitchInput;

	UPROPERTY()
		float _YawInput;

	UPROPERTY()
		float _RollInput;

	UPROPERTY()
		float _TargetSpeed;

	UPROPERTY()
		float _CurrentSpeed;

	UPROPERTY()
		bool _bIsBoosting;

	UPROPERTY()
		float _DeltaTime;

	UPROPERTY()
		float _StartTime;
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ZEROGOKI_API UShipMovement : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UShipMovement();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	//Move Simulation
	FShipMove CreateMove(float DeltaTime);
	void SimulateMove(const FShipMove& Move);

	//Getters
	float GetBoostAmountLevel() { return BoostAmountLevel; }
	bool GetIsBoosting() { return bIsBoosting; }
	float GetTargetSpeed() { return CurrentMoveForwardInput; }
	float GetCurrentSpeed() { return CurrentThrustSpeed; }

	//Setters
	void SetFlightRoll(float Value);
	void SetFlightPitch(float Value);
	void SetFlightYaw(float Value);
	void SetTargetSpeed(float Value);

	//Boost
	void SupersonicBoost();
	void StopBoost();
	

private:

	void Initialize();
	
	//Movement Simulation on Client
	void MoveForward(float CurrentSpeed, float TargetSpeed, float _DeltaTime);
	void SimulatePitchInput(float FlightPitch, float _DeltaTime);
	void SimulateYawInput(float FlightYaw, float _DeltaTime);
	void SimulateRollInput(float FlightRoll, float _DeltaTime);
	void UseBoostAmount(bool IsBoosting, float _DeltaTime);

#pragma region SpeedProperties

	float CurrentMoveForwardInput;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Speed", meta = (AllowPrivateAccess = "true"))
		float MinimumThrustSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Speed", meta = (AllowPrivateAccess = "true"))
		float MaximumThrustSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Speed", meta = (AllowPrivateAccess = "true"))
		float TargetThrustSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Speed", meta = (AllowPrivateAccess = "true"))
		float CurrentThrustSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Speed", meta = (AllowPrivateAccess = "true"))
		float BoostAmountLevel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Speed", meta = (AllowPrivateAccess = "true"))
		float BoostThrustSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Speed", meta = (AllowPrivateAccess = "true"))
		bool bIsBoosting;

#pragma endregion

#pragma region RotationProperties

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rotation", meta = (AllowPrivateAccess = "true"))
		float FlightPitch;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rotation", meta = (AllowPrivateAccess = "true"))
		float FlightTurnRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rotation", meta = (AllowPrivateAccess = "true"))
		float FlightYaw;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rotation", meta = (AllowPrivateAccess = "true"))
		float FlightRoll;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rotation", meta = (AllowPrivateAccess = "true"))
		bool bIsInvertVerticalAxis;

#pragma endregion

	
	
};
