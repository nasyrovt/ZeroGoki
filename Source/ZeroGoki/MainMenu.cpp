// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"
#include "SpaceBattleGameInstance.h"
#include "UObject/ConstructorHelpers.h"
#include "OpenedSessionWidget.h"

/// <summary>
///		Constructor
/// </summary>
/// <param name="ObjectInitializer"></param>
UMainMenu::UMainMenu(const FObjectInitializer& ObjectInitializer) 
{
	ConstructorHelpers::FClassFinder<UUserWidget> SessionWidgetClass(TEXT("/Game/UI/Blueprint/WBP_OpenedSession"));
	if (!ensure(SessionWidgetClass.Class != nullptr)) return;
	SessionLineClass = SessionWidgetClass.Class;
}


/// <summary>
///		Initialize onclick callbacks and ensure all widget references
/// </summary>
/// <returns></returns>
bool UMainMenu::Initialize()
{
	if (!Super::Initialize()) return false;

	if (!ensure(HostButton != nullptr)) return false;
	HostButton->OnClicked.AddDynamic(this, &UMainMenu::OpenHostMenu);

	if (!ensure(JoinButton != nullptr)) return false;
	JoinButton->OnClicked.AddDynamic(this, &UMainMenu::OpenJoinMenu);

	if (!ensure(CancelHostMenuButton != nullptr)) return false;
	CancelHostMenuButton->OnClicked.AddDynamic(this, &UMainMenu::OpenMainMenu);

	if (!ensure(ConfirmHostMenuButton != nullptr)) return false;
	ConfirmHostMenuButton->OnClicked.AddDynamic(this, &UMainMenu::HostServer);

	if (!ensure(CancelButton != nullptr)) return false;
	CancelButton->OnClicked.AddDynamic(this, &UMainMenu::OpenMainMenu);

	if (!ensure(JoinByIpButton != nullptr)) return false;
	JoinByIpButton->OnClicked.AddDynamic(this, &UMainMenu::JoinServerByIp);

	if (!ensure(QuitGameButton != nullptr)) return false;
	QuitGameButton->OnClicked.AddDynamic(this, &UMainMenu::QuitGame);

	if (!ensure(SessionsScrollBox != nullptr)) return false;

	return true;
}


/// <summary>
///		Sets list of available servers to join
/// </summary>
/// <param name="SessionsNames"></param>
void UMainMenu::SetSessionsList(TArray<FServerData> SessionsNames)
{
	if (SessionsScrollBox == nullptr) return;
	SessionsScrollBox->ClearChildren();
	uint32 index = 0;
	for (const FServerData ServerData : SessionsNames)
	{
		if (!ensure(SessionLineClass != nullptr)) return;
		UOpenedSessionWidget* SessionLine = CreateWidget<UOpenedSessionWidget>(this, SessionLineClass);

		if (!ensure(SessionLine != nullptr)) return;

		SessionLine->SessionName->SetText(FText::FromString(ServerData.Name));
		SessionLine->UserName->SetText(FText::FromString(ServerData.HostUserName));
		FString PlayersFractionText = FString::Printf(TEXT("%d/%d"), ServerData.CurrentPlayersCount, ServerData.MaxPlayersCount);
		SessionLine->PlayersCount->SetText(FText::FromString(PlayersFractionText));
		SessionLine->Setup(this, index);
		index++;

		SessionsScrollBox->AddChild(SessionLine);
	}
}


#pragma region Widget Switcher


void UMainMenu::OpenHostMenu()
{
	if (!ensure(WidgetSwitcher != nullptr)) return;
	if (!ensure(HostMenu != nullptr)) return;

	WidgetSwitcher->SetActiveWidget(HostMenu);
}

void UMainMenu::OpenJoinMenu()
{
	if (!ensure(WidgetSwitcher != nullptr)) return;
	if (!ensure(JoinMenu != nullptr)) return;

	WidgetSwitcher->SetActiveWidget(JoinMenu);

	if (MenuInterface != nullptr)
		MenuInterface->RefreshSessionsList();
}

void UMainMenu::OpenMainMenu()
{
	if (!ensure(WidgetSwitcher != nullptr)) return;
	if (!ensure(MainMenu != nullptr)) return;

	WidgetSwitcher->SetActiveWidget(MainMenu);
}


#pragma endregion

#pragma region Hightlighting Sessions

void UMainMenu::SelectIndex(uint32 Index)
{
	SelectedIndex = Index;
	UpdateChildren();
}

void UMainMenu::UpdateChildren()
{
	for (int32 i = 0; i < SessionsScrollBox->GetChildrenCount(); i++)
	{
		auto ServerRow = Cast<UOpenedSessionWidget>(SessionsScrollBox->GetChildAt(i));
		if (ServerRow != nullptr)
		{
			ServerRow->bIsSelected = (SelectedIndex.IsSet() && SelectedIndex.GetValue() == i);
		}
	}
}

#pragma endregion

#pragma region Buttons Callbacks

void UMainMenu::HostServer()
{
	if (MenuInterface != nullptr)
	{
		FString ServerName = ServerHostName->Text.ToString();
		MenuInterface->Host(ServerName);
	}
}

void UMainMenu::JoinServerByIp()
{
	if (SelectedIndex.IsSet() && MenuInterface != nullptr)
		MenuInterface->Join(SelectedIndex.GetValue());
}

void UMainMenu::QuitGame()
{
	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;

	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (!ensure(PlayerController != nullptr)) return;

	PlayerController->ConsoleCommand("quit");
}

#pragma endregion
