// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "ZeroGokiPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class ZEROGOKI_API AZeroGokiPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	AZeroGokiPlayerState();

	void AddKill();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Properties")
		int32 Kills;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category = "Properties")
		int32 TeamIndex;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};