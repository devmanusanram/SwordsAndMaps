// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Pawn.h"
#include "A_RTSSpectatorCam.generated.h"

// 
#define DEFAULT_ZOOM_FACTOR			4000.0f


// How close the camera can zoom in
#define MIN_CAMERA_ZOOM				200.0f
// How far the camera can zoom out
#define MAX_CAMERA_ZOOM				8000.0f


// Move factor 
#define MOVEMENT_FACTOR				75.0f
// Edge to start scrolling camera
#define EDGE_SCROLL_THRESHOLD		50.0f


UCLASS()
class MYPROJECT_API AA_RTSSpectatorCam : public APawn
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AA_RTSSpectatorCam();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Blend time for perspective change
	UPROPERTY(EditAnywhere, Category = "Camera")
		float PerspBlend;
	
	// Zoom factor to zoom in or out
	UPROPERTY(EditAnywhere, Category = "Camera")
		float					m_ZoomFactor;
	// Length of the camera's spring arm
	UPROPERTY(EditAnywhere, Category = "Camera")
		float					m_SAHeight;

	void GetMousePosition();

	// Camera component for this pawn
	UCameraComponent*			m_SpectatorCam;
	// Arm component to locate camera at given height
	USpringArmComponent*		m_CameraSA;



	// Get the mouse location
	FVector2D			MousePosition;
	// Size of the screen
	FVector2D			ViewportSize;
	// Transform to move the camera through the world
	FVector2D			m_CamTransform;
};
