// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OpenedSessionWidget.generated.h"

/**
 * Available session Widget to insert in a Join Session List
 */
UCLASS()
class ZEROGOKI_API UOpenedSessionWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta = (BindWindget))
	class UTextBlock* SessionName;

	UPROPERTY(meta = (BindWindget))
	class UTextBlock* PlayersCount;

	UPROPERTY(meta = (BindWindget))
	class UTextBlock* UserName;

	void Setup(class UMainMenu* Parent, uint32 Index);

	UPROPERTY(BlueprintReadOnly)
	bool bIsSelected = false;

private:

	UFUNCTION()
	void OnClicked();

	UPROPERTY()
	class UMainMenu* Parent; 

	//Sesion Index to hightlight onChosen
	uint32 Index;

	UPROPERTY(meta = (BindWidget))
	class UButton* SessionButton;
};
