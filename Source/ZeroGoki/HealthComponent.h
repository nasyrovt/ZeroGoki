// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ZEROGOKI_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(ReplicatedUsing = OnRep_CurrentHealth)
		float CurrentHealth;

	UPROPERTY(EditDefaultsOnly, Category = "Health")
		float MaxHealth = 100.f;

	UPROPERTY(ReplicatedUsing = OnRep_CurrentShield)
		float CurrentShield;

	UPROPERTY(EditDefaultsOnly, Category = "Health")
		float MaxShield = 100.f;

	/** RepNotify for changes made to current health.*/
	UFUNCTION()
		void OnRep_CurrentHealth();

	/** Response to health being updated. Called on the server immediately after modification, and on clients in response to a RepNotify*/
	void OnHealthUpdate();

	/** RepNotify for changes made to current shield.*/
	UFUNCTION()
		void OnRep_CurrentShield();

	/** Response to shield being updated. Called on the server immediately after modification, and on clients in response to a RepNotify*/
	void OnShieldUpdate();

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

#pragma region Health

	UFUNCTION(BlueprintCallable, Category = "Health")
		FORCEINLINE float GetCurrentHealth() const { return CurrentHealth; }

	UFUNCTION(BlueprintCallable, Category = "Health")
		FORCEINLINE float GetMaxHealth() const { return MaxHealth; }

	UFUNCTION(BlueprintCallable, Category = "Health")
		void SetCurrentHealth(float Value);

#pragma endregion

#pragma region Shield

	UFUNCTION(BlueprintCallable, Category = "Shield")
		FORCEINLINE float GetCurrentShield() const { return CurrentShield; }

	UFUNCTION(BlueprintCallable, Category = "Shield")
		void SetCurrentShield(float Value);

	UFUNCTION(BlueprintCallable, Category = "Shield")
		FORCEINLINE float GetMaxShield() const { return MaxShield; }

#pragma endregion
	
};
