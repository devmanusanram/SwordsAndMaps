// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "MyProject.h"
#include "MyProjectCharacter.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Runtime/Engine/Classes/Components/DecalComponent.h"
#include "Runtime/Engine/Classes/Engine/TextRenderActor.h"
#include "Kismet/HeadMountedDisplayFunctionLibrary.h"

AMyProjectCharacter::AMyProjectCharacter()
{
	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	YawFactor = 45.0f;
	PitchFactor = 45.0f;

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 540.0f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;
	GetCharacterMovement()->JumpZVelocity = 600.0f;
	GetCharacterMovement()->AirControl = 0.8f;


	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = ZoomFactor = DEFAULT_ZOOM;
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level
	CameraBoom->bUsePawnControlRotation = true;

	 // Create a camera...
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Create a decal in the world to show the cursor's location
	CursorToWorld = CreateDefaultSubobject<UDecalComponent>("CursorToWorld");
	CursorToWorld->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UMaterial> DecalMaterialAsset(TEXT("Material'/Game/TopDownCPP/Blueprints/M_Cursor_Decal.M_Cursor_Decal'"));
	
	if (DecalMaterialAsset.Succeeded())
	{
		CursorToWorld->SetDecalMaterial(DecalMaterialAsset.Object);
	}

	CursorToWorld->DecalSize = FVector(16.0f, 48.0f, 48.0f);
	CursorToWorld->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f).Quaternion());
	
	CursorToWorld->SetVisibility(false);

	// Create a decal in the world to show the character is leader
	LeaderDecal = CreateDefaultSubobject<UDecalComponent>("LeaderDecal");
	LeaderDecal->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UMaterial> LeaderDecalMaterialAsset(TEXT("Material'/Game/TopDownCPP/Blueprints/M_LeaderDecal.M_LeaderDecal'"));

	if (LeaderDecalMaterialAsset.Succeeded())
	{
		LeaderDecal->SetDecalMaterial(LeaderDecalMaterialAsset.Object);
	}

	LeaderDecal->DecalSize = FVector(16.0f, 48.0f, 48.0f);
	LeaderDecal->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f).Quaternion());

	LeaderDecal->SetVisibility(false);

	IsGroupLeader = false;
	IsSelected = false;

	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
}


void AMyProjectCharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

	FRotator RotPitch = CameraBoom->GetComponentRotation();
	RotPitch.Pitch = FMath::Clamp(RotPitch.Pitch + PawnCameraInput.Y, -80.0f, 10.0f);
	CameraBoom->SetWorldRotation(RotPitch);

	FRotator RotYaw = CameraBoom->GetComponentRotation();
	RotYaw.Yaw = FMath::Clamp(RotYaw.Yaw + PawnCameraInput.X, -180.0f, 180.0f);
	CameraBoom->SetWorldRotation(RotYaw);

	if (CursorToWorld != nullptr)
	{
		if (APlayerController* PC = Cast<APlayerController>(GetController()))
		{
			FHitResult TraceHitResult;
			PC->GetHitResultUnderCursor(ECC_Visibility, true, TraceHitResult);
			FVector CursorFV = TraceHitResult.ImpactNormal;
			FRotator CursorR = CursorFV.Rotation();
			CursorToWorld->SetWorldLocation(TraceHitResult.Location);
			CursorToWorld->SetWorldRotation(CursorR);
		}
	}
}


// Set game agent as leader of RTS group
void AMyProjectCharacter::SetAsLeader()
{
	// Turn on leader decal
	LeaderDecal->SetVisibility(true);
	// Turn on leader flag
	IsGroupLeader = true;
}


// Unset agent as leader
void AMyProjectCharacter::UnsetLeader()
{
	// Turn off leader decal
	LeaderDecal->SetVisibility(false);
	// Turn off leader flag
	IsGroupLeader = false;
}


// Set game agent as seleted in RTS mode
void AMyProjectCharacter::SetAsSelected() 
{
	if (!this->IsGroupLeader)
	{
		// Turn on selection decal
		CursorToWorld->SetVisibility(true);
	}
	
	// Turn on selected flag
	IsSelected = true;
}


// Set game agent as deselected in RTS mode
void AMyProjectCharacter::SetAsDeselected() 
{
	if (!this->IsGroupLeader)
	{
		// Turn off the selection decal
		CursorToWorld->SetVisibility(false);
	}
	
	// Turn off selected flag
	IsSelected = false;
}


FString AMyProjectCharacter::PrintData()
{
	FVector OutputPosition = GetActorLocation();

	FString Message;

	Message += FString::SanitizeFloat(OutputPosition.X);
	Message += ", ";
	Message += FString::SanitizeFloat(OutputPosition.Y);
	
	if (this->IsGroupLeader)
	{
		// Print a message marking this agent as the leader
		Message += " - GROUP LEADER";
	}

	if (this->IsHero)
	{
		Message += " - HERO";
	}

	Message += "\n";

	return Message;
}


void AMyProjectCharacter::MoveFwd(float _fwd)
{
	if ((Controller != NULL) && (_fwd != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, _fwd);
	}

	//PawnMovementInput.X = FMath::Clamp<float>(_fwd, -1.0f, 1.0f);
}


void AMyProjectCharacter::StrafeRite(float _rite)
{
	if ((Controller != NULL) && (_rite != 0.0f))
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, _rite);
	}

	//PawnMovementInput.Y = FMath::Clamp<float>(_rite, -1.0f, 1.0f);
}


void AMyProjectCharacter::CamYaw(float _yaw)
{
	// calculate delta for this frame from the rate information
	//AddControllerYawInput(_yaw * YawFactor * GetWorld()->GetDeltaSeconds());
	PawnCameraInput.X = _yaw;
}


void AMyProjectCharacter::CamPitch(float _pitch)
{
	// calculate delta for this frame from the rate information
	//AddControllerPitchInput(_pitch * PitchFactor * GetWorld()->GetDeltaSeconds());
	PawnCameraInput.Y = _pitch;
}


void AMyProjectCharacter::CamZoomIn()
{
	float Diff = 50.0f;

	ZoomFactor -= Diff;

	if (ZoomFactor <= MIN_ZOOM_IN)
	{
		CameraBoom->TargetArmLength = MIN_ZOOM_IN;
		ZoomFactor = MIN_ZOOM_IN;
	}

	else
	{
		CameraBoom->TargetArmLength = ZoomFactor;
	}
}


void AMyProjectCharacter::CamZoomOut()
{
	float Diff = 50.0f;

	ZoomFactor += Diff;

	if (ZoomFactor >= MAX_ZOOM_OUT)
	{
		CameraBoom->TargetArmLength = MAX_ZOOM_OUT;
		ZoomFactor = MAX_ZOOM_OUT;
	}

	else
	{
		CameraBoom->TargetArmLength = ZoomFactor;
	}
}

