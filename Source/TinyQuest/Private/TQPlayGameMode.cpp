// Fill out your copyright notice in the Description page of Project Settings.


#include "TQPlayGameMode.h"
#include "TQPlayerController.h"

ATQPlayGameMode::ATQPlayGameMode()
{
    PlayerControllerClass = ATQPlayerController::StaticClass();
}

void ATQPlayGameMode::BeginPlay()
{
    Super::BeginPlay();
}
