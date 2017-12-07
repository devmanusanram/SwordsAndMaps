// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "A_RTS_GameMode.h"


AA_RTS_GameMode::AA_RTS_GameMode()
{
	static ConstructorHelpers::FObjectFinder<UBlueprint> PlayerPawnObject(TEXT("Game/TopDownCPP/Blueprints/ARTS_Cam.ARTS_Cam"));
	if (PlayerPawnObject.Object != NULL)
	{
		DefaultPawnClass = (UClass*)PlayerPawnObject.Object->GeneratedClass;
	}

	PlayerControllerClass = AA_RTSPlayerController::StaticClass();
	HUDClass = AA_RTS_HUD::StaticClass();
}

