// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerController.h"

#include "Engine.h"
#include "A_RTS_HUD.h"
#include "A_RTSSpectatorCam.h"

#include "A_RTSPlayerController.generated.h"

#define UNIT_OFFSET	180.0f

/**
 * Sets the inputs and controls for the ARTS framework
 */
UCLASS()
class MYPROJECT_API AA_RTSPlayerController : public APlayerController
{
	GENERATED_BODY()
	// Constructor
	AA_RTSPlayerController();

	// Begin play
	virtual void BeginPlay() override;
	// Set game inputs
	virtual void SetupInputComponent() override;
	// Game tick
	virtual void Tick(float DeltaTime) override;

	// Game mode flag : If active, RTS mode is on, else ACTION mode is on
	bool RTS_Mode;

	// HUD component to draw
	AA_RTS_HUD* HUD;

	// MAIN CAM - Do not confuse with Hitler's book
	AA_RTSSpectatorCam* MainCam;

	// Selected characters
	TArray<AMyProjectCharacter*> PlayingCharacters;

	// Hero character - This guy makes the perspective change
	AMyProjectCharacter* HeroCharacter;
	
	// RTS move destination
	FVector Destination;

	// Camera positions
	TArray<FVector> CamLocations;

	// Current camera position in array
	uint32 CurrentCamLocation;

	

	// --- RTS CHARACTER MOVEMENT ---
	// Set RTS characters movement destination
	void MoveToDestination();

	// --- RTS MARQUEE SELECTION ---
	// Start RTS selection
	void StartSelecting();
	// Stop RTS selection
	void StopSelecting();

	// --- RTS CAMERA ---
	// Save a location to move the camera... 'Cause why the fuck you create another camera?
	void CreateCam();
	// Kill th current location of a camera
	void HakaiCam();
	// Switch o the next camera location
	void SwitchCam();

	// Move RTS camera with scroll movement
	void MoveCam();

	// Zoom in RTS cam
	void ZoomIn();
	// Zoom out RTS cam
	void ZoomOut();
	// Zoom in RTS cam
	void HeroZoomIn();
	// Zoom out RTS cam
	void HeroZoomOut();

	// Perspective change
	void ChangePersp();

	// --- CHARACTER ACTION MOVEMENT ---
	// Move hero forward and back
	void HeroFwd(float _axis);
	// Strafe hero left and right
	void HeroRight(float _axis);
	// Hero Pitch
	void HeroPitch(float _axis);
	// Hero Yaw
	void HeroYaw(float _axis);
	// Hero Jump
	void HeroJump();
	// Hero Land
	void HeroLand();
};
