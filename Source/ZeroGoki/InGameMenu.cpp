// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameMenu.h"
#include "Components/Button.h"

bool UInGameMenu::Initialize()
{
	if (!Super::Initialize()) return false;

	if (!ensure(CancelButton != nullptr) || !ensure(QuitButton != nullptr)) return false;

	CancelButton->OnClicked.AddDynamic(this, &UInGameMenu::Cancel);
	QuitButton->OnClicked.AddDynamic(this, &UInGameMenu::Disconect);

	return true;
}


void UInGameMenu::Cancel() 
{
	TearDown();
}


void UInGameMenu::Disconect() 
{
	if (MenuInterface == nullptr) return;
	TearDown();
	MenuInterface->LoadMainMenu();
}
