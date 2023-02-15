// Fill out your copyright notice in the Description page of Project Settings.


#include "CameraManager.h"

// Sets default values for this component's properties
UCameraManager::UCameraManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	
}


// Called when the game starts
void UCameraManager::BeginPlay()
{
	Super::BeginPlay();

	//Get Components References
	SpringArm = GetOwner()->FindComponentByClass<USpringArmComponent>();
	Camera = GetOwner()->FindComponentByClass<UCameraComponent>();
	MovementComponent = GetOwner()->FindComponentByClass<UShipMovement>();

	bIsFPSView = false;
	
	//Setup Delegate for a FPS/TPS View Change Event
	OnViewSwaped.AddUObject(this, &UCameraManager::HandleViewSwap);
}


// Called every frame
void UCameraManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//Simulate Dolly Camera while Boosting
	DollyCamera();
}


void UCameraManager::DollyCamera() 
{
	UpdateCameraFieldOfView();
	UpdateSpringArm();
}

void UCameraManager::UpdateSpringArm()
{
	if (SpringArm == nullptr || MovementComponent == nullptr || bIsFPSView) return;

	float targetLengthA = FMath::Lerp(SpringArmDollyLength, 
		SpringArmStandardLength, 
		MovementComponent->GetBoostAmountLevel());
	float targetLengthB = SpringArmStandardLength;

	float targetPitchA = FMath::Lerp(SpringArmDollyPitch,
		SpringArmStandardPitch,
		MovementComponent->GetBoostAmountLevel());
	float targetPitchB = SpringArmStandardPitch;

	float targetLength = MovementComponent->GetIsBoosting() ? targetLengthA : targetLengthB;
	float targetPitch = MovementComponent->GetIsBoosting() ? targetPitchA : targetPitchB;

	SpringArm->TargetArmLength = FMath::FInterpTo(
		SpringArm->TargetArmLength, 
		targetLength, 
		GetWorld()->GetDeltaSeconds(), 
		6.f);

	SpringArm->SetRelativeRotation(FMath::RInterpTo(
		SpringArm->GetRelativeRotation(), 
		FRotator(targetPitch, 0.f, 0.f),
		GetWorld()->GetDeltaSeconds(), 
		6.f));

}

void UCameraManager::UpdateCameraFieldOfView()
{
	if (Camera == nullptr || bIsFPSView) return;

	float targetFieldOfView = MovementComponent->GetIsBoosting() ?
		FMath::Lerp(120, 90, MovementComponent->GetBoostAmountLevel()) : 90;

	Camera->SetFieldOfView(
		FMath::FInterpTo(Camera->FieldOfView,
			targetFieldOfView,
			GetWorld()->GetDeltaSeconds(),
			6.f));
}

void UCameraManager::SwapCameraView()
{
	bIsFPSView = !bIsFPSView;
	OnViewSwaped.Broadcast();
}

void UCameraManager::HandleViewSwap()
{
	if (SpringArm == nullptr || Camera == nullptr ) return;

	if (bIsFPSView) 
	{
		SpringArm->TargetArmLength = SpringArmFPSLength;
		SpringArm->SetRelativeRotation(FRotator(SpringArmFPSPitch, 0.f, 0.f));
	}
	else {
		SpringArm->TargetArmLength = SpringArmTPSLength;
		SpringArm->SetRelativeRotation(FRotator(SpringArmTPSPitch, 0.f, 0.f));
	}
}

