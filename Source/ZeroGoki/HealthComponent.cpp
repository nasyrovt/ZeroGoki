// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"
#include <Net/UnrealNetwork.h>

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	CurrentHealth = MaxHealth;
	CurrentShield = MaxShield;
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

}


// Called every frame
void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UHealthComponent::GetLifetimeReplicatedProps(TArray <FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	//Replicate current health.
	DOREPLIFETIME(UHealthComponent, CurrentHealth);
}

void UHealthComponent::OnHealthUpdate()
{
	//Client-specific functionality
	if (Cast<APawn>(GetOwner())->IsLocallyControlled())
	{
		
		if (CurrentHealth <= 0)
		{
			
		}
	}

	//Server-specific functionality
	if (GetOwner()->GetLocalRole() == ROLE_Authority)
	{
	
	}

	//Functions that occur on all machines. 
	if (CurrentHealth <= 0) 
	{
		//GetOwner()->Destroy();
		

	}
}


void UHealthComponent::OnRep_CurrentHealth()
{
	OnHealthUpdate();
}

void UHealthComponent::SetCurrentHealth(float healthValue)
{
	CurrentHealth = FMath::Clamp(healthValue, 0.f, MaxHealth);
	OnHealthUpdate();
}

void UHealthComponent::OnShieldUpdate()
{
	//Client-specific functionality
	if (Cast<APawn>(GetOwner())->IsLocallyControlled())
	{
		FString healthMessage = FString::Printf(TEXT("You now have %f health remaining."), CurrentHealth);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, healthMessage);

		if (CurrentHealth <= 0)
		{
			FString deathMessage = FString::Printf(TEXT("You have been killed."));
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, deathMessage);
		}
	}

	//Server-specific functionality
	if (GetOwner()->GetLocalRole() == ROLE_Authority)
	{
		FString healthMessage = FString::Printf(TEXT("%s now has %f health remaining."), *GetFName().ToString(), CurrentHealth);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, healthMessage);
	}

	//Functions that occur on all machines. 
	if (CurrentHealth <= 0) {
		//GetOwner()->Destroy();
	}
}


void UHealthComponent::OnRep_CurrentShield()
{
	OnShieldUpdate();
}

void UHealthComponent::SetCurrentShield(float ShieldValue)
{
	if (GetOwner()->GetLocalRole() == ROLE_Authority)
	{
		CurrentShield = FMath::Clamp(ShieldValue, 0.f, MaxShield);
		OnShieldUpdate();
	}
}


