// Fill out your copyright notice in the Description page of Project Settings.


#include "TQPlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

ATQPlayerController::ATQPlayerController()
{
    bShowMouseCursor = false;
}

void ATQPlayerController::BeginPlay()
{
    Super::BeginPlay();
}

void ATQPlayerController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);
    if (const TObjectPtr<ULocalPlayer> LocalPlayer = Cast<ULocalPlayer>(Player))
    {
        if (const TObjectPtr<UEnhancedInputLocalPlayerSubsystem> Subsystem 
            = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
        {
            if (Player3rdPersonIMC)
            {
                Subsystem->AddMappingContext(Player3rdPersonIMC, MappingPriority);
            }
        }
    }
}

void ATQPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();
    if (const TObjectPtr<UEnhancedInputComponent> EnhancedInputComponent = 
        Cast<UEnhancedInputComponent>(InputComponent))
    {
        if (MoveAction)
        {
            EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ATQPlayerController::HandleMovement);
        }
    }
}

void ATQPlayerController::HandleMovement(const FInputActionValue& Value)
{
    FVector2D MovementVector = Value.Get<FVector2D>();
}
