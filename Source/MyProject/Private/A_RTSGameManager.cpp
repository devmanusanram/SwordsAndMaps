// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "A_RTSGameManager.h"


// Sets default values
AA_RTSGameManager::AA_RTSGameManager()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AA_RTSGameManager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AA_RTSGameManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AA_RTSGameManager::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

