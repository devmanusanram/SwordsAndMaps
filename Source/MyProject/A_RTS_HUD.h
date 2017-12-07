// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
// Parent class
#include "GameFramework/HUD.h"
// Include the character to save pointers to world characters
#include "MyProjectCharacter.h"
// Generated file
#include "A_RTS_HUD.generated.h"

/**
 * Draw ARTS stuff to screen
 */
UCLASS()
class MYPROJECT_API AA_RTS_HUD : public AHUD
{
	GENERATED_BODY()
public:
	// Selection start point
	FVector2D SelectStartPoint;
	// Selection current end point
	FVector2D SelectCurrentPoint;
	// Get if player is currently selecting pawns
	bool IsSelecting;
	// Selected pawns
	TArray<AMyProjectCharacter*> SelectedCharacters;
	// Get the current selection point (Bottom right point)
	FVector2D GetCurrentPoint();
	// Similar to Tick function, draws stuff to the screen
	virtual void DrawHUD() override;
};
