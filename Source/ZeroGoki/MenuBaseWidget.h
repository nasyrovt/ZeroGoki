// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MenuInterface.h"
#include "Blueprint/UserWidget.h"
#include "MenuBaseWidget.generated.h"

/**
 * Base Class for all types of menu interfaces (Main Menu, In Game Menu, etc)
 */
UCLASS()
class ZEROGOKI_API UMenuBaseWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	void Setup();
	void TearDown();

	void SetMenuInterface(IMenuInterface* _MenuInterface);

protected:

	IMenuInterface* MenuInterface;
};
