// Fill out your copyright notice in the Description page of Project Settings.


#include "TQMenuPlayerController.h"

#include "TQMainMenuWidget.h"

void ATQMenuPlayerController::BeginPlay()
{
    Super::BeginPlay();
    if (ActiveMenuWidget) return;
    if (MainMenuWidgetClass == nullptr)
    {
        return;
    }
    
    ActiveMenuWidget = CreateWidget<UTQMainMenuWidget>(this, MainMenuWidgetClass);
    ActiveMenuWidget->AddToViewport();
    bShowMouseCursor = true;
    FInputModeUIOnly InputModeData;
    InputModeData.SetWidgetToFocus(ActiveMenuWidget->TakeWidget());
    SetInputMode(InputModeData);
}
