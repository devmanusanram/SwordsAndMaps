// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "A_RTSSpectatorCam.h"


// Sets default values
AA_RTSSpectatorCam::AA_RTSSpectatorCam()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Create the spring arm
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	m_CameraSA = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraSpringArm"));
	m_CameraSA->SetupAttachment(RootComponent);
	m_CameraSA->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, 50.0f), FRotator(-60.0f, 0.0f, 0.0f));
	m_CameraSA->TargetArmLength = m_SAHeight = m_ZoomFactor = DEFAULT_ZOOM_FACTOR;
	m_CameraSA->bEnableCameraLag = true;
	// No lag, let it move along
	m_CameraSA->CameraLagSpeed = 0.0f;

	// Create the camera
	m_SpectatorCam = CreateDefaultSubobject<UCameraComponent>(TEXT("GameCamera"));
	m_SpectatorCam->SetupAttachment(m_CameraSA, USpringArmComponent::SocketName);

}

// Called when the game starts or when spawned
void AA_RTSSpectatorCam::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AA_RTSSpectatorCam::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AA_RTSSpectatorCam::GetMousePosition()
{// Get mouse location to compare with viewport size
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController != nullptr)
	{
		// Get the coordinates of the mouse from our controller  
		float LocationX;
		float LocationY;
		PlayerController->GetMousePosition(LocationX, LocationY);
		// Do a trace and see if there the position intersects something in the world  
		MousePosition.X = LocationX;
		MousePosition.Y = LocationY;

		PlayerController->bShowMouseCursor = true;
		PlayerController->bEnableClickEvents = true;
	}
}

