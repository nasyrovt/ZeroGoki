// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include <Camera/CameraComponent.h>
#include "ShipMovement.h"
#include <GameFramework/SpringArmComponent.h>
#include "CameraManager.generated.h"



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ZEROGOKI_API UCameraManager : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCameraManager();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	DECLARE_EVENT(UCameraManager, ViewSwapEvent)
		ViewSwapEvent OnViewSwap() { return OnViewSwaped; }

	void SwapCameraView();

	void HandleViewSwap();

	void UpdateCameraFieldOfView();
	void UpdateSpringArm();

	void DollyCamera();

private:
	UPROPERTY(EditAnywhere, Category="FPS Settings")
		float SpringArmFPSPitch;

	UPROPERTY(EditAnywhere, Category = "FPS Settings")
		float SpringArmFPSLength;

	UPROPERTY(EditAnywhere, Category = "TPS Settings")
		float SpringArmTPSLength;

	UPROPERTY(EditAnywhere, Category = "TPS Settings")
		float SpringArmTPSPitch;

	UPROPERTY(EditAnywhere, Category = "Dolly Camera Settings")
		float SpringArmStandardLength;

	UPROPERTY(EditAnywhere, Category = "Dolly Camera Settings")
		float SpringArmStandardPitch;

	UPROPERTY(EditAnywhere, Category = "Dolly Camera Settings")
		float SpringArmDollyLength;

	UPROPERTY(EditAnywhere, Category = "Dolly Camera Settings")
		float SpringArmDollyPitch;

	bool bIsFPSView;

	ViewSwapEvent OnViewSwaped;

	USpringArmComponent* SpringArm;
	UCameraComponent* Camera;
	UShipMovement* MovementComponent;
	

};
