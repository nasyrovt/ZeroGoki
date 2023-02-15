// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MenuBaseWidget.h"
#include "MainMenu.generated.h"



USTRUCT()
struct FServerData
{
	GENERATED_BODY()

	UPROPERTY()
	FString Name;

	UPROPERTY()
	uint32 CurrentPlayersCount;

	UPROPERTY()
	uint32 MaxPlayersCount;

	UPROPERTY()
	FString HostUserName;
};


/**
 * 
 */
UCLASS()
class ZEROGOKI_API UMainMenu : public UMenuBaseWidget
{
	GENERATED_BODY()
	
public:
	UMainMenu(const FObjectInitializer& ObjectInitializer);
	
	void SetSessionsList(TArray<FServerData> SessionsNames);

	void SelectIndex(uint32 Index);

protected:
	virtual bool Initialize();

private:

#pragma region Buttons

	UPROPERTY(meta = (BindWidget))
		class UButton* ConfirmHostMenuButton;

	UPROPERTY(meta = (BindWidget))
		class UButton* CancelHostMenuButton;

	UPROPERTY(meta = (BindWidget))
		class UButton* HostButton;

	UPROPERTY(meta = (BindWidget))
		class UButton* JoinButton;

	UPROPERTY(meta = (BindWidget))
		class UButton* JoinByIpButton;

	UPROPERTY(meta = (BindWidget))
		class UButton* CancelButton;

	UPROPERTY(meta = (BindWidget))
		class UButton* QuitGameButton;

#pragma endregion

#pragma region Menu References

	UPROPERTY(meta = (BindWidget))
		class UWidget* JoinMenu;

	UPROPERTY(meta = (BindWidget))
		class UWidget* MainMenu;

	UPROPERTY(meta = (BindWidget))
		class UWidget* HostMenu;

#pragma endregion

#pragma region Containers Widgets and Widget Switcher

	UPROPERTY(meta = (BindWidget))
		class UEditableTextBox* ServerHostName;

	UPROPERTY(meta = (BindWidget))
		class UWidgetSwitcher* WidgetSwitcher;

	UPROPERTY(meta = (BindWidget))
		class UPanelWidget* SessionsScrollBox;

#pragma endregion

#pragma region Widget Switcher Actions

	UFUNCTION()
		void OpenJoinMenu();

	UFUNCTION()
		void OpenHostMenu();

	UFUNCTION()
		void OpenMainMenu();

#pragma endregion

#pragma region Buttons Callbacks

	UFUNCTION()
		void HostServer();

	UFUNCTION()
		void QuitGame();

	UFUNCTION()
		void JoinServerByIp();

#pragma endregion

#pragma region Available Session Lines Related

	TOptional<uint32> SelectedIndex;

	void UpdateChildren();

	UPROPERTY()
	TSubclassOf<class UUserWidget> SessionLineClass;

#pragma endregion
	
};
