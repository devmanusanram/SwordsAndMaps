// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "A_RTSPlayerController.h"


// Constructor
AA_RTSPlayerController::AA_RTSPlayerController()
{
	// Users can see the cursor while playing
	bShowMouseCursor = true;
	// Cursor is represented by a cross
	DefaultMouseCursor = EMouseCursor::Crosshairs;

	RTS_Mode = true;
}

// Begin play
void AA_RTSPlayerController::BeginPlay()
{
	// Get the HUD object to draw 2D elements to the screen
	HUD = Cast<AA_RTS_HUD>(GetHUD());

	MainCam = Cast<AA_RTSSpectatorCam>(GetWorld()->GetFirstPlayerController()->GetPawn());

	MainCam->ViewportSize = FVector2D(GEngine->GameViewport->Viewport->GetSizeXY());

	CurrentCamLocation = 0;
}


// Set game inputs
void AA_RTSPlayerController::SetupInputComponent()
{
	// Tell the parent class to set the inputs
	Super::SetupInputComponent();
	// Set the LMB, to tell RTS characters to move to a location
	InputComponent->BindAction("RTS_Move", IE_Pressed, this, &AA_RTSPlayerController::MoveToDestination);

	// Set the inputs to start marquee selection (RMB pressed) and stop it (RMB released)
	InputComponent->BindAction("RTS_Select", IE_Pressed,	this, &AA_RTSPlayerController::StartSelecting);
	InputComponent->BindAction("RTS_Select", IE_Released,	this, &AA_RTSPlayerController::StopSelecting);

	InputComponent->BindAction("ChangePersp", IE_Pressed,	this, &AA_RTSPlayerController::ChangePersp);

	InputComponent->BindAction("RTS_ZoomIn",  IE_Pressed,	this, &AA_RTSPlayerController::ZoomIn);
	InputComponent->BindAction("RTS_ZoomOut", IE_Pressed,	this, &AA_RTSPlayerController::ZoomOut);

	InputComponent->BindAction("CreateCam", IE_Pressed, this, &AA_RTSPlayerController::CreateCam);
	InputComponent->BindAction("HakaiCam",  IE_Pressed,	this, &AA_RTSPlayerController::HakaiCam);
	InputComponent->BindAction("SwitchCam", IE_Pressed, this, &AA_RTSPlayerController::SwitchCam);

	InputComponent->BindAction("ACTION_HeroJump", IE_Pressed, this,  &AA_RTSPlayerController::HeroJump);
	InputComponent->BindAction("ACTION_HeroJump", IE_Released, this, &AA_RTSPlayerController::HeroLand);
	InputComponent->BindAction("ACTION_HeroZoomIn", IE_Released, this, &AA_RTSPlayerController::HeroZoomIn);
	InputComponent->BindAction("ACTION_HeroZoomOut", IE_Pressed, this, &AA_RTSPlayerController::HeroZoomOut);

	// PAWN -----------------------------------------------------------------------------------------------------------
	InputComponent->BindAxis("ACTION_HeroForward",		this, &AA_RTSPlayerController::HeroFwd);
	InputComponent->BindAxis("ACTION_HeroRight",		this, &AA_RTSPlayerController::HeroRight);
	InputComponent->BindAxis("ACTION_HeroPitch",		this, &AA_RTSPlayerController::HeroPitch);
	InputComponent->BindAxis("ACTION_HeroYaw",			this, &AA_RTSPlayerController::HeroYaw);

}


// Game tick
void AA_RTSPlayerController::Tick(float DeltaTime)
{
	FString Message;

	if (RTS_Mode)
	{
		MainCam->GetMousePosition();
		MoveCam();
	}


	Message += "MOVE TO NEW LOCATION: ";
	Message += FString::SanitizeFloat(Destination.X);
	Message += ", ";
	Message += FString::SanitizeFloat(Destination.Y);
	Message += "\n\n";

	if (PlayingCharacters.Num() > 0)
	{
		for (int32 i = 0; i < PlayingCharacters.Num(); ++i)
		{
			Message += "Character [";
			Message += FString::FromInt(i);
			Message += "] :: ";
			Message += PlayingCharacters[i]->PrintData();
		}
	}

	if (CamLocations.Num() > 0)
	{
		for (int32 i = 0; i < CamLocations.Num(); ++i)
		{
			Message += "Camera [";
			Message += FString::FromInt(i);
			Message += "]\n\n";
		}
	}

	Message += "Current cam: ";
	Message += FString::FromInt(CurrentCamLocation);
	Message += "\n";

	// Print on screen
	GEngine->AddOnScreenDebugMessage(0, 1.0f, FColor::Red, Message);
}


// Set move destination
void AA_RTSPlayerController::MoveToDestination()
{	
	if (RTS_Mode)
	{
		int32 Selector = 1;
		// If there are actors that can be moved through the world
		if (PlayingCharacters.Num() > 0)
		{
			// Go through all the selected actor
			for (int32 i = 0; i < PlayingCharacters.Num(); ++i)
			{
				// Variable to save a click on the screen 
				FHitResult CursorHit;
				// Get the cursor click by visibility on the screen
				GetHitResultUnderCursor
				(
					ECollisionChannel::ECC_Visibility, // Get by visibility on the screen
					false, // Do not get a complex cursor hit
					CursorHit // Variable to save the cursor hit
				);

				// LEADER IS ALWAYS AGENT AT INDEX 0

				// Formation of four agents following - HARDCODED
				if (!PlayingCharacters[i]->IsGroupLeader)
				{

					//
					float Offset = UNIT_OFFSET;

					// Using the list's index to perform the movement
					Destination =
						FVector
						(
							// Using the selector, they either get their position at the right or the left of the leader
							CursorHit.Location.X + Selector * ((i % PlayingCharacters.Num()) * Offset),
							// They get a point in 'Y' 
							CursorHit.Location.Y + ((i % PlayingCharacters.Num()) * Offset),
							// They still haven't learn to fly
							0
						);

					Selector *= -1;
				}

				else
				{
					// Save the hit's location to perform the move here
					Destination = CursorHit.Location;
				}

				UNavigationSystem::SimpleMoveToLocation
				(
					// Get the characters controller
					PlayingCharacters[i]->GetController(),
					// Point to move to
					Destination
				);
			}
		}
	}
}


// Start selection
void AA_RTSPlayerController::StartSelecting()
{
	if (RTS_Mode)
	{
		// There's no leader selected yet
		HeroCharacter = NULL;
		// Pass the HUD the screen point to begin drawing the marquee rectangle
		HUD->SelectStartPoint = HUD->GetCurrentPoint();
		// Tell the HUD to start drawing the marquee rectangle
		HUD->IsSelecting = true;
	}
}


// Stop selection
void AA_RTSPlayerController::StopSelecting()
{
	if (RTS_Mode)
	{
		// Tell the HUD to stop drawing the marquee rectangle
		HUD->IsSelecting = false;
		// Get the actors inside the marquee area
		PlayingCharacters = HUD->SelectedCharacters;

		if (!HeroCharacter && PlayingCharacters.Num() > 0)
		{
			HeroCharacter = PlayingCharacters[0];
		}
	}
}


// --- RTS CAMERA ---
// Save a location to move the camera... 'Cause why the fuck you create another camera?
void AA_RTSPlayerController::CreateCam()
{
	if (RTS_Mode)
	{
		FVector NewCam;

		NewCam = MainCam->GetActorLocation();

		CamLocations.Add(NewCam);

		// Print on screen
		GEngine->AddOnScreenDebugMessage(1, 1.0f, FColor::Red, "New camera created");
	}
}


// Kill the current location of a camera
void AA_RTSPlayerController::HakaiCam()
{
	if (RTS_Mode)
	{
		uint32 TotalList = CamLocations.Num();

		if (TotalList > 0)
		{
			CamLocations.RemoveAt(CurrentCamLocation);
		}
	}
}


// Switch to the next camera location
void AA_RTSPlayerController::SwitchCam()
{//
	if (RTS_Mode)
	{//
		FVector RePositioner;
		//
		uint32 TotalList = CamLocations.Num();
		//
		if (TotalList > 0)
		{//
			if (CurrentCamLocation == TotalList - 1)
			{
				//
				RePositioner = CamLocations[0];
				CurrentCamLocation = 0;
			}

			else
			{
				//
				CurrentCamLocation += 1;
				RePositioner = CamLocations[CurrentCamLocation];
			}

		}

		else
		{// Print on screen
			GEngine->AddOnScreenDebugMessage(1, 1.0f, FColor::Red, "No more cams to switch");
			//
			return;
		}

		//
		MainCam->SetActorLocation(RePositioner);

		// Print on screen
		GEngine->AddOnScreenDebugMessage(1, 1.0f, FColor::Red, "Switching cams");
	}
}


// Move RTS camera with scroll movement
void AA_RTSPlayerController::MoveCam()
{
	// AXIS ALIGNED MOVEMENTS
	// Forward
	if (MainCam->MousePosition.Y <= 50.0f)
	{
		FVector NewLocation = MainCam->GetActorLocation();
		NewLocation += MainCam->GetActorForwardVector() * MOVEMENT_FACTOR;
		MainCam->SetActorLocation(NewLocation);
	}

	// Backward
	else if (MainCam->MousePosition.Y >= MainCam->ViewportSize.Y - 50.0f)
	{
		FVector NewLocation = MainCam->GetActorLocation();
		NewLocation -= MainCam->GetActorForwardVector() * MOVEMENT_FACTOR;
		MainCam->SetActorLocation(NewLocation);
	}

	// Left
	else if (MainCam->MousePosition.X <= 50.0f)
	{
		FVector NewLocation = MainCam->GetActorLocation();
		NewLocation -= MainCam->GetActorRightVector() * MOVEMENT_FACTOR;
		MainCam->SetActorLocation(NewLocation);
	}

	// Right
	else if (MainCam->MousePosition.X >= MainCam->ViewportSize.X - 50.0f)
	{
		FVector NewLocation = MainCam->GetActorLocation();
		NewLocation += MainCam->GetActorRightVector() * MOVEMENT_FACTOR;
		MainCam->SetActorLocation(NewLocation);
	}



	// DIAGONAL ALIGNMENT
	// Forward Left
	if (MainCam->MousePosition.Y <= EDGE_SCROLL_THRESHOLD && MainCam->MousePosition.X <= EDGE_SCROLL_THRESHOLD)
	{
		FVector NewLocation = MainCam->GetActorLocation();
		NewLocation += MainCam->GetActorForwardVector() * MOVEMENT_FACTOR;
		NewLocation -= MainCam->GetActorRightVector() * MOVEMENT_FACTOR;
		MainCam->SetActorLocation(NewLocation);
	}

	// Forward Right
	else if (MainCam->MousePosition.Y <= EDGE_SCROLL_THRESHOLD && MainCam->MousePosition.X >= MainCam->ViewportSize.X - EDGE_SCROLL_THRESHOLD)
	{
		FVector NewLocation = MainCam->GetActorLocation();
		NewLocation += MainCam->GetActorForwardVector() * MOVEMENT_FACTOR;
		NewLocation += MainCam->GetActorRightVector() * MOVEMENT_FACTOR;
		MainCam->SetActorLocation(NewLocation);
	}

	// Backward Left
	else if (MainCam->MousePosition.Y >= MainCam->ViewportSize.Y - EDGE_SCROLL_THRESHOLD && MainCam->MousePosition.X <= EDGE_SCROLL_THRESHOLD)
	{
		FVector NewLocation = MainCam->GetActorLocation();
		NewLocation -= MainCam->GetActorForwardVector() * MOVEMENT_FACTOR;
		NewLocation -= MainCam->GetActorRightVector() * MOVEMENT_FACTOR;
		MainCam->SetActorLocation(NewLocation);
	}

	// Backward right
	else if (MainCam->MousePosition.Y >= MainCam->ViewportSize.Y - EDGE_SCROLL_THRESHOLD && MainCam->MousePosition.X >= MainCam->ViewportSize.X - EDGE_SCROLL_THRESHOLD)
	{
		FVector NewLocation = MainCam->GetActorLocation();
		NewLocation -= MainCam->GetActorForwardVector() * MOVEMENT_FACTOR;
		NewLocation += MainCam->GetActorRightVector() * MOVEMENT_FACTOR;
		MainCam->SetActorLocation(NewLocation);
	}
}


// Zoom in RTS cam
void AA_RTSPlayerController::ZoomIn()
{
	if (RTS_Mode)
	{
		if (MainCam)
		{
			float Diff = 300.0f;

			MainCam->m_ZoomFactor -= Diff;

			if (MainCam->m_ZoomFactor <= MIN_CAMERA_ZOOM)
			{
				MainCam->m_CameraSA->TargetArmLength = MIN_CAMERA_ZOOM;
				MainCam->m_ZoomFactor = MIN_CAMERA_ZOOM;
			}

			else
			{
				MainCam->m_CameraSA->TargetArmLength = MainCam->m_ZoomFactor;
			}
		}
	}
}


// Zoom out RTS cam
void AA_RTSPlayerController::ZoomOut()
{
	if (RTS_Mode)
	{
		if (MainCam)
		{
			float Diff = 300.0f;

			MainCam->m_ZoomFactor += Diff;

			if (MainCam->m_ZoomFactor >= MAX_CAMERA_ZOOM)
			{
				MainCam->m_CameraSA->TargetArmLength = MAX_CAMERA_ZOOM;
				MainCam->m_ZoomFactor = MAX_CAMERA_ZOOM;
			}

			else
			{
				MainCam->m_CameraSA->TargetArmLength = MainCam->m_ZoomFactor;
			}
		}
	}
}


// Perspective change
void AA_RTSPlayerController::ChangePersp()
{
	if (HeroCharacter && MainCam)
	{
		if (RTS_Mode)
		{
			SetViewTargetWithBlend(HeroCharacter, MainCam->PerspBlend);
			RTS_Mode = false;
		}

		else
		{
			SetViewTargetWithBlend(MainCam, MainCam->PerspBlend);
			RTS_Mode = true;
		}
	}
}


// --- CHARACTER ACTION MOVEMENT ---
// Move hero forward and back
void AA_RTSPlayerController::HeroFwd(float _axis)
{
	if (HeroCharacter != nullptr && !RTS_Mode)
	{
		HeroCharacter->MoveFwd(_axis);
	}
}


// Strafe hero left and right
void AA_RTSPlayerController::HeroRight(float _axis)
{
	if (HeroCharacter != nullptr && !RTS_Mode)
	{
		HeroCharacter->StrafeRite(_axis);
	}
}


// Hero Pitch
void AA_RTSPlayerController::HeroPitch(float _axis)
{
	if (HeroCharacter != nullptr && !RTS_Mode)
	{
		HeroCharacter->CamPitch(_axis);
	}
}


// Hero Yaw
void AA_RTSPlayerController::HeroYaw(float _axis)
{
	if (HeroCharacter != nullptr && !RTS_Mode)
	{
		HeroCharacter->CamYaw(_axis);
	}
}


// Hero Jump
void AA_RTSPlayerController::HeroJump()
{
	if (HeroCharacter != nullptr && !RTS_Mode)
	{
		HeroCharacter->Jump();
	}
}


// Hero Land
void AA_RTSPlayerController::HeroLand()
{
	if (HeroCharacter != nullptr && !RTS_Mode)
	{
		HeroCharacter->StopJumping();
	}
}

// Zoom in RTS cam
void AA_RTSPlayerController::HeroZoomIn()
{
	if (HeroCharacter != nullptr && !RTS_Mode)
	{
		HeroCharacter->CamZoomIn();
	}
}

// Zoom in RTS cam
void AA_RTSPlayerController::HeroZoomOut()
{
	if (HeroCharacter != nullptr && !RTS_Mode)
	{
		HeroCharacter->CamZoomOut();
	}
}
