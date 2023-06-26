// Copyright Epic Games, Inc. All Rights Reserved.

#include "GASInputSystemGameMode.h"
#include "GASInputSystemCharacter.h"
#include "UObject/ConstructorHelpers.h"

AGASInputSystemGameMode::AGASInputSystemGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
