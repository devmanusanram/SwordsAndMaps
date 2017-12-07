// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "A_RTS_HUD.h"

FVector2D AA_RTS_HUD::GetCurrentPoint()
{
	// Variables to save the information
	float PosX, PosY;
	// Get the mouse position from the player controller
	GetOwningPlayerController()->GetMousePosition(PosX, PosY);
	// Return the mouse position
	return FVector2D(PosX, PosY);
}


void AA_RTS_HUD::DrawHUD()
{
	// If player starts selecting
	if (IsSelecting)
	{
		// Check if there's actors selected 
		if (SelectedCharacters.Num() > 0)
		{
			// Go through all the selected actor
			for (int32 i = 0; i < SelectedCharacters.Num(); ++i)
			{
				// Set all as deselected
				SelectedCharacters[i]->SetAsDeselected();

				if (SelectedCharacters[i]->IsGroupLeader)
				{
					// Unset leader
					SelectedCharacters[i]->UnsetLeader();
				}
			}
		}

		// Empty the list of pawns
		SelectedCharacters.Empty();

		// Get point of ending to the marquee selection rectangle
		SelectCurrentPoint = GetCurrentPoint();

		// Draw the marquee selection rect
		DrawRect
		(
			FLinearColor(0.0f, 0.0f, 1.0f, 0.30f), // Set green color for the rectangle
			SelectStartPoint.X, // Rectangle top-left X position
			SelectStartPoint.Y, // Rectangle top-left Y position
			SelectCurrentPoint.X - SelectStartPoint.X, // Rectangle width
			SelectCurrentPoint.Y - SelectStartPoint.Y // Rectangle height
		);

		// Get actors under selection and save them in a list
		GetActorsInSelectionRectangle<AMyProjectCharacter>(SelectStartPoint, SelectCurrentPoint, SelectedCharacters, false, false);

		// Check for any actors found inside the marquee rect
		if (SelectedCharacters.Num() > 0)
		{
			// Go through all the selected actor
			for (int32 i = 0; i < SelectedCharacters.Num(); ++i)
			{
				// Check if actor is first to enter selection list 
				if (i == 0)
				{
					// If first, then set this agent as leader
					SelectedCharacters[i]->SetAsLeader();
				}

				// Set as selected
				SelectedCharacters[i]->SetAsSelected();
			}
		}
	}
}

