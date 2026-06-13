// Fill out your copyright notice in the Description page of Project Settings.


#include "TQMenuGameMode.h"

#include "TQMenuPlayerController.h"

ATQMenuGameMode::ATQMenuGameMode()
{
    PlayerControllerClass = ATQMenuPlayerController::StaticClass();
    DefaultPawnClass = nullptr;
}

void ATQMenuGameMode::BeginPlay()
{
    Super::BeginPlay();
}
