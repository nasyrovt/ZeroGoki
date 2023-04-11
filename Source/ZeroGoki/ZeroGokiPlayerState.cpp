// Fill out your copyright notice in the Description page of Project Settings.

#include <Net/UnrealNetwork.h>
#include "ZeroGokiPlayerState.h"

AZeroGokiPlayerState::AZeroGokiPlayerState() 
{
    Kills = 0;
    TeamIndex = 0;
}

void AZeroGokiPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(AZeroGokiPlayerState, Kills);
    DOREPLIFETIME(AZeroGokiPlayerState, TeamIndex);
}

void AZeroGokiPlayerState::AddKill() 
{
    Kills++;
}