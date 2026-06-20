// Fill out your copyright notice in the Description page of Project Settings.


#include "TQPlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "TQPlayerCharacter.h"

ATQPlayerController::ATQPlayerController()
{
    bShowMouseCursor = false;
}

void ATQPlayerController::BeginPlay()
{
    Super::BeginPlay();
    const FInputModeGameOnly InputMode;
    SetInputMode(InputMode);
    bShowMouseCursor = false;
    UE_LOG(LogTemp, Warning, TEXT("ATQPlayerController::BeginPlay()"));
    UE_LOG(LogTemp, Warning, TEXT("Controller: %s"),
        *GetNameSafe(GetPawn()));
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
        if (LookAction)
            EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ATQPlayerController::HandleLook);

        if (JumpAction)
        {
            // Bind both Pressed (Triggered) and Released (Completed) structural events
            EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ATQPlayerController::HandleJumpStart);
            EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ATQPlayerController::HandleJumpEnd);
        }
    }
}

void ATQPlayerController::HandleMovement(const FInputActionValue& Value)
{
    
    const FVector2D MovementVector = Value.Get<FVector2D>();
    if (TObjectPtr<ATQPlayerCharacter> TargetCharacter = Cast<ATQPlayerCharacter>(GetPawn()))
    {
        TargetCharacter->MoveHandler(MovementVector);
    }
}

void ATQPlayerController::HandleLook(const FInputActionValue& Value)
{
    const FVector2D MovementVector = Value.Get<FVector2D>();
    if (TObjectPtr<ATQPlayerCharacter> TargetCharacter = Cast<ATQPlayerCharacter>(GetPawn()))
    {
        TargetCharacter->LookHandler(MovementVector);
    }
}


void ATQPlayerController::HandleJumpStart(const FInputActionValue& Value)
{
    if (TObjectPtr<ATQPlayerCharacter> TargetCharacter = Cast<ATQPlayerCharacter>(GetPawn()))
    {
        TargetCharacter->JumpHandler();
    }
}

void ATQPlayerController::HandleJumpEnd(const FInputActionValue& Value)
{
    if (TObjectPtr<ATQPlayerCharacter> TargetCharacter = Cast<ATQPlayerCharacter>(GetPawn()))
    {
        TargetCharacter->JumpingStopHandler();
    }
}


