// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/Character.h"
#include "MyProjectCharacter.generated.h"

// Pawn moves 10 units
#define DISTANCE_ACTION_MOVE	10.0f
#define JUMP_FACTOR				300.0f
#define MID_AIR_CONTROL			0.5F

#define MIN_ZOOM_IN				250.0f
#define DEFAULT_ZOOM			300.0f
#define MAX_ZOOM_OUT			500.0f

UCLASS(Blueprintable)
class AMyProjectCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AMyProjectCharacter();

	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;

	/** Returns TopDownCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns CursorToWorld subobject **/
	FORCEINLINE class UDecalComponent* GetCursorToWorld() { return CursorToWorld; }
	
	// Vector to move pawn front and backwards
	FVector2D				PawnMovementInput;
	// Vector to strafe pawn left or right
	FVector2D				PawnCameraInput;


	// Set game agent as leader of RTS group
	void SetAsLeader();
	// Unset agent as leader
	void UnsetLeader();

	// Set game agent as selected in RTS mode
	void SetAsSelected();
	// Set game agent as deselected in RTS mode
	void SetAsDeselected();
	
	// Print character position
	FString PrintData();

	void MoveFwd(float _fwd);
	void StrafeRite(float _rite);

	void CamYaw(float _yaw);
	void CamPitch(float _pitch);

	void CamZoomIn();
	void CamZoomOut();


private:
	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TopDownCameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** A decal that projects to the cursor location. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UDecalComponent* CursorToWorld;

	/** A decal that projects if character is group leader */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UDecalComponent* LeaderDecal;

public:
	// Determines if character entered marquee selection in RTS mode
	bool IsSelected;
	// Determines if character is leader of a RTS group
	bool IsGroupLeader;

	UPROPERTY(EditAnywhere, Category = "Statistics")
	float					Health;

	UPROPERTY(EditAnywhere, Category = "Statistics")
	float					Attack;

	UPROPERTY(EditAnywhere, Category = "Statistics")
	float					Defense;

	UPROPERTY(EditAnywhere, Category = "Statistics")
	float					Speed;

	UPROPERTY(EditAnywhere, Category = "Statistics")
	float					JumpFactor;

	UPROPERTY(EditAnywhere, Category = "Statistics")
	float					MidAirFactor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Statistics")
	float					YawFactor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Statistics")
	float					PitchFactor;
	UPROPERTY(EditAnywhere, Category = "Camera")
		float					ZoomFactor;


	UPROPERTY(EditAnywhere, Category = "Special")
	bool					IsHero;

	UPROPERTY(EditAnywhere, Category = "Special")
	bool					IsRangeWarrior;

	UPROPERTY(EditAnywhere, Category = "Special")
	bool					IsMeleeWarrior;
};

