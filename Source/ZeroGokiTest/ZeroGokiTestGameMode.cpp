// Copyright Epic Games, Inc. All Rights Reserved.

#include "ZeroGokiTestGameMode.h"
#include "ZeroGokiTestCharacter.h"
#include "UObject/ConstructorHelpers.h"

AZeroGokiTestGameMode::AZeroGokiTestGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
