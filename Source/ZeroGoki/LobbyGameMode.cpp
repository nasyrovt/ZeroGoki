// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameMode.h"

#include "TimerManager.h"
#include "SpaceBattleGameInstance.h"


/// <summary>
///		Overriden PostLogin Event Handler
/// </summary>
/// <param name="PlayerController"></param>
void ALobbyGameMode::PostLogin(APlayerController * PlayerController) 
{
	Super::PostLogin(PlayerController);

	NumberOfPlayers++;

	bUseSeamlessTravel = true;
	if (NumberOfPlayers == 2) {
		
		GetWorldTimerManager().SetTimer(
			GameStartTimerHandle, 
			this, 
			&ALobbyGameMode::HandleStartGame,
			StartDelay);
	}
	
}


/// <summary>
///		Overriden Logout Event Handler
/// </summary>
/// <param name="Exiting"></param>
void ALobbyGameMode::Logout(AController* Exiting) 
{
	Super::Logout(Exiting);

	NumberOfPlayers--;

	if (NumberOfPlayers < 3) 
	{
		GameStartTimerHandle.Invalidate();
	}
}

/// <summary>
///		Timer Handler for Starting the Game and Travel to Game Map
/// </summary>
void ALobbyGameMode::HandleStartGame() 
{
	USpaceBattleGameInstance* GameInstance = Cast<USpaceBattleGameInstance>(GetGameInstance());
	if (GameInstance == nullptr) return;

	GameInstance->StartSession();

	UWorld* World = GetWorld();

	if (!ensure(World != nullptr)) return;

	World->ServerTravel("/Game/Maps/DeathMatch?listen");
}