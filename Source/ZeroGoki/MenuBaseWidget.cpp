// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuBaseWidget.h"


/// <summary>
///		Initialize a UI input behaviour
/// </summary>
void UMenuBaseWidget::Setup()
{
	this->bIsFocusable = true;
	this->AddToViewport();

	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;

	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (!ensure(PlayerController != nullptr)) return;

	FInputModeUIOnly InputModeData;
	InputModeData.SetWidgetToFocus(this->TakeWidget());
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

	PlayerController->SetInputMode(InputModeData);
	PlayerController->bShowMouseCursor = true;
}


/// <summary>
///		Reset to normal game input
/// </summary>
void UMenuBaseWidget::TearDown()
{
	this->RemoveFromViewport();

	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;

	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (!ensure(PlayerController != nullptr)) return;

	FInputModeGameOnly InputMode;
	PlayerController->SetInputMode(InputMode);
	PlayerController->bShowMouseCursor = false;
}


/// <summary>
///		Menu Interface Setter for Game Instance
/// </summary>
/// <param name="_MenuInterface"></param>
void UMenuBaseWidget::SetMenuInterface(IMenuInterface* _MenuInterface)
{
	this->MenuInterface = _MenuInterface;
}