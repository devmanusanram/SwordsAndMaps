// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "MyProject.h"
#include "MyProjectGameMode.h"
#include "MyProjectPlayerController.h"
#include "MyProjectCharacter.h"

AMyProjectGameMode::AMyProjectGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = AMyProjectPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDownCPP/Blueprints/TopDownCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}