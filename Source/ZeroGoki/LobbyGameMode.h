// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "LobbyGameMode.generated.h"

/**
 * 
 */
UCLASS()
class ZEROGOKI_API ALobbyGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	void PostLogin(APlayerController* PlayerController) override;

	void Logout(AController* Exiting) override;

private:
	
	uint32 NumberOfPlayers = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess="true"))
	float StartDelay = 10.f;

	FTimerHandle GameStartTimerHandle;
	void HandleStartGame();
};
