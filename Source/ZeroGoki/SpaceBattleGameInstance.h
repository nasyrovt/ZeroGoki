// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MenuInterface.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSubsystem.h"
#include "SpaceBattleGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class ZEROGOKI_API USpaceBattleGameInstance : public UGameInstance, public IMenuInterface
{
	GENERATED_BODY()

public:

	USpaceBattleGameInstance(const FObjectInitializer& ObjectInitializer);
	virtual void Init();


	UFUNCTION(BlueprintCallable)
	void LoadMenuWidget();

	UFUNCTION(BlueprintCallable)
	void LoadInGameMenu();

	void StartSession();
	

	//Interface overrides
	UFUNCTION(Exec)
	void Host(FString ServerName) override;
	
	UFUNCTION(Exec)
	void Join(uint32 Index) override;

	
	void RefreshSessionsList() override;

	virtual void LoadMainMenu() override;


private:
	UPROPERTY()
	TSubclassOf<class UUserWidget> MenuClass;

	UPROPERTY()
	TSubclassOf<class UUserWidget> InGameMenuClass;

	class UMainMenu* Menu;

	IOnlineSessionPtr SessionInterface;
	TSharedPtr<class FOnlineSessionSearch> SessionSearch;

	void OnCreateSessionComplete(FName SessionName, bool Success);
	void OnDestroySessionComplete(FName SessionName, bool Success);
	void OnFindSessionComplete(bool Success);
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
	void OnNetworkFailure(UWorld* World, UNetDriver* NetDriver, ENetworkFailure::Type FailureType, const FString& ErrorString);

	void CreateSession();

	FString DesiredServerName;
	
};

