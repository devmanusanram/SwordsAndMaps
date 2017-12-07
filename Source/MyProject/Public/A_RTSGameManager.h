// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Pawn.h"
#include "A_RTSGameManager.generated.h"

// This class manages the basic elements of ARTS game
UCLASS()
class MYPROJECT_API AA_RTSGameManager : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AA_RTSGameManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	
	
};
