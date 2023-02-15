// Fill out your copyright notice in the Description page of Project Settings.


#include "OpenedSessionWidget.h"
#include "Components/Button.h"
#include "MainMenu.h"

void UOpenedSessionWidget::Setup(class UMainMenu* InParent, uint32 InIndex) 
{
	Parent = InParent;
	Index = InIndex;
	SessionButton->OnClicked.AddDynamic(this, &UOpenedSessionWidget::OnClicked);
}

void UOpenedSessionWidget::OnClicked() 
{
	Parent->SelectIndex(Index);
}