// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MenuBaseWidget.h"
#include "InGameMenu.generated.h"

/**
 * 
 */
UCLASS()
class ZEROGOKI_API UInGameMenu : public UMenuBaseWidget
{
	GENERATED_BODY()

protected:
	virtual bool Initialize();

private:

	UPROPERTY(meta = (BindWidget))
		class UButton* QuitButton;

	UPROPERTY(meta = (BindWidget))
		class UButton* CancelButton;

	UFUNCTION()
	void Disconect();

	UFUNCTION()
	void Cancel();
	
};
