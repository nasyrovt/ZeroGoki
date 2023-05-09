// Fill out your copyright notice in the Description page of Project Settings.


#include "SpaceBattleGameInstance.h"
#include "MainMenu.h"
#include "MenuBaseWidget.h"
#include "OnlineSessionSettings.h"
#include "OnlineSubsystem.h"
#include "Blueprint/UserWidget.h"
#include "Engine/Engine.h"
#include "UObject/ConstructorHelpers.h"

//"GameSession" as name is a work-around to allow Null Online Subsystem to decrement NumPublicConnections
const static FName SESSION_NAME = TEXT("GameSession");
const static FName SERVER_NAME_SETTINGS_KEY = TEXT("ServerName");

#pragma region Init + Constructor


/// <summary>
///		Constructor:
///			Initialize MainMenu and InGame Widgets
/// </summary>
/// <param name="ObjectInitializer"></param>
USpaceBattleGameInstance::USpaceBattleGameInstance(const FObjectInitializer& ObjectInitializer)
{
	ConstructorHelpers::FClassFinder<UUserWidget> MainMenuWidgetClass(TEXT("/Game/UI/Blueprint/WBP_MainMenu"));
	if (!ensure(MainMenuWidgetClass.Class != nullptr)) return;
	MenuClass = MainMenuWidgetClass.Class;

	ConstructorHelpers::FClassFinder<UUserWidget> InGameMenuWidgetClass(TEXT("/Game/UI/Blueprint/WBP_InGameMenu"));
	if (!ensure(InGameMenuWidgetClass.Class != nullptr)) return;
	InGameMenuClass = InGameMenuWidgetClass.Class;
}

/// <summary>
///		Overriden methode from UGameInstance to initialize some properties, callbacks, etc...
/// </summary>
void USpaceBattleGameInstance::Init()
{
	IOnlineSubsystem* OSS = IOnlineSubsystem::Get();
	if (OSS != nullptr) {
		SessionInterface = OSS->GetSessionInterface();
		if (SessionInterface.IsValid()) {
			SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &USpaceBattleGameInstance::OnCreateSessionComplete);
			SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &USpaceBattleGameInstance::OnDestroySessionComplete);
			SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &USpaceBattleGameInstance::OnFindSessionComplete);
			SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &USpaceBattleGameInstance::OnJoinSessionComplete);
		}
	}
	else
		UE_LOG(LogTemp, Warning, TEXT("Found no subsystem."));
	//Handle host disconnection
	if (GEngine != nullptr)
		GEngine->OnNetworkFailure().AddUObject(this, &USpaceBattleGameInstance::OnNetworkFailure);
}


#pragma endregion

#pragma region Session Create/Join/Host


/// <summary>
///		Joins a session by Index and destroys MainMenu
/// </summary>
/// <param name="Index"></param>
void USpaceBattleGameInstance::Join(uint32 Index)
{
	if (!SessionInterface.IsValid() || !SessionSearch.IsValid()) return;

	if (Menu != nullptr)
	{
		Menu->TearDown();
	}

	SessionInterface->JoinSession(0, SESSION_NAME, SessionSearch->SearchResults[Index]);
}


/// <summary>
///		Destroys an existing session if exists, and creates a session for a given ServerName
/// </summary>
/// <param name="ServerName"></param>
void USpaceBattleGameInstance::Host(FString ServerName)
{
	DesiredServerName = ServerName;

	if (SessionInterface.IsValid())
	{
		FNamedOnlineSession* ExistingSession = SessionInterface->GetNamedSession(SESSION_NAME);
		if (ExistingSession != nullptr)
			SessionInterface->DestroySession(SESSION_NAME);
		else
			CreateSession();
	}
}


/// <summary>
///		Creates a session with specified settings
///		TODO: Make some fields customizeble
/// </summary>
void USpaceBattleGameInstance::CreateSession()
{
	if (SessionInterface.IsValid())
	{
		FOnlineSessionSettings SessionSettings;

		//if (IOnlineSubsystem::Get()->GetSubsystemName() == "NULL")

		//	SessionSettings.bIsLANMatch = true; //For local testing
		//else
		SessionSettings.bIsLANMatch = false;
		SessionSettings.bUseLobbiesIfAvailable = true;
		SessionSettings.NumPublicConnections = 5;
		SessionSettings.bShouldAdvertise = true; //Lets Session be Public
		SessionSettings.bUsesPresence = true;
		SessionSettings.Set(SERVER_NAME_SETTINGS_KEY, DesiredServerName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

		SessionInterface->CreateSession(0, SESSION_NAME, SessionSettings);
	}

}


/// <summary>
///		Start a session
///		This function is public and is called from LobbyGameMode to start a game to travel to
/// </summary>
void USpaceBattleGameInstance::StartSession()
{
	if (SessionInterface.IsValid())
	{
		SessionInterface->StartSession(SESSION_NAME);
	}
}


/// <summary>
///		Initialize SessionSearch Options
///		Uncomment bIsLanQuery for LAN tests
/// </summary>
void USpaceBattleGameInstance::RefreshSessionsList()
{
	SessionSearch = MakeShareable(new FOnlineSessionSearch());

	if (SessionSearch.IsValid()) {
		UE_LOG(LogTemp, Warning, TEXT("Start Finding Sessions"));
		//SessionSearch->bIsLanQuery = true;
		SessionSearch->MaxSearchResults = 100;
		SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
		SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
	}
}


#pragma endregion

#pragma region Events Callbacks


/// <summary>
///		Callback tries to get connection string and performs a Client Travel to a connected session if successfull
/// </summary>
/// <param name="SessionName"></param>
/// <param name="Result"></param>
void USpaceBattleGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	if (!SessionInterface.IsValid()) return;

	FString Address;

	if (!SessionInterface->GetResolvedConnectString(SessionName, Address))
		UE_LOG(LogTemp, Warning, TEXT("Could not get a connection string from session interface"));


	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (!ensure(PlayerController != nullptr)) return;

	PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
}


/// <summary>
///		Callback performs ServerTravel to a Lobby map and destroys Menu Widget
/// </summary>
/// <param name="SessionName"></param>
/// <param name="Success"></param>
void USpaceBattleGameInstance::OnCreateSessionComplete(FName SessionName, bool Success)
{
	if (!Success) {
		UE_LOG(LogTemp, Warning, TEXT("Couldn't create session"));
		return;
	}
	if (Menu != nullptr)
	{
		Menu->TearDown();
	}

	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;

	World->ServerTravel("/Game/Maps/Lobby?listen");

}



/// <summary>
///		Return to MainMenu if host has disconected
/// </summary>
/// <param name="World"></param>
/// <param name="NetDriver"></param>
/// <param name="FailureType"></param>
/// <param name="ErrorString"></param>
void USpaceBattleGameInstance::OnNetworkFailure(UWorld* World, UNetDriver* NetDriver, ENetworkFailure::Type FailureType, const FString& ErrorString)
{
	LoadMainMenu();
}


/// <summary>
///		Callback called after IOnlineSession::DestroySession. 
///		Here: Creates a Session, when the old one is completely destroyed
/// </summary>
/// <param name="SessionName"></param>
/// <param name="Success"></param>
void USpaceBattleGameInstance::OnDestroySessionComplete(FName SessionName, bool Success)
{
	//TODO: Gestion de destruction
	if (Success)
		CreateSession();

}


/// <summary>
///		Creates a list of available sessions meta data and sends meta data to MainMenu to populate "sessions to join" list.
/// </summary>
/// <param name="Success"></param>
void USpaceBattleGameInstance::OnFindSessionComplete(bool Success)
{
	if (Success && SessionSearch.IsValid() && Menu != nullptr)
	{
		TArray<FServerData> SessionsNames;

		for (const FOnlineSessionSearchResult& SearchResult : SessionSearch->SearchResults)
		{
			FServerData Data;
			Data.Name = SearchResult.GetSessionIdStr();
			Data.MaxPlayersCount = SearchResult.Session.SessionSettings.NumPublicConnections;
			Data.CurrentPlayersCount = Data.MaxPlayersCount - SearchResult.Session.NumOpenPublicConnections;
			Data.HostUserName = SearchResult.Session.OwningUserName;
			FString ServerName;
			if (SearchResult.Session.SessionSettings.Get(SERVER_NAME_SETTINGS_KEY, ServerName))
				Data.Name = ServerName;
			else
				Data.Name = "Could not find name.";

			SessionsNames.Add(Data);
		}

		Menu->SetSessionsList(SessionsNames);
	}
}


#pragma endregion

#pragma region Menu Widgets Loading

/// <summary>
///		Creates a In Game Menu Widget, sets control settings and active menu interface
/// </summary>
void USpaceBattleGameInstance::LoadInGameMenu()
{
	if (!ensure(InGameMenuClass != nullptr)) return;
	UMenuBaseWidget* InGameMenu = CreateWidget<UMenuBaseWidget>(this, InGameMenuClass);

	//Change control settings and sets active menu interface
	InGameMenu->Setup();
	InGameMenu->SetMenuInterface(this);

}


/// <summary>
///		Creates a Main Menu Widget, sets control settings and active menu interface
/// </summary>
void USpaceBattleGameInstance::LoadMenuWidget()
{
	if (!ensure(MenuClass != nullptr)) return;
	Menu = CreateWidget<UMainMenu>(this, MenuClass);

	//Change control settings and sets active menu interface
	Menu->Setup();
	Menu->SetMenuInterface(this);
}

#pragma endregion

#pragma region MainMenuMap Loading


/// <summary>
///		Loads a Main Menu Map
/// </summary>
void USpaceBattleGameInstance::LoadMainMenu()
{
	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (!ensure(PlayerController != nullptr)) return;

	PlayerController->ClientTravel("/Game/Maps/MainMenu", ETravelType::TRAVEL_Absolute);
}


#pragma endregion
