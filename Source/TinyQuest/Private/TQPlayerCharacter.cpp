// Fill out your copyright notice in the Description page of Project Settings.


#include "TQPlayerCharacter.h"

#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
ATQPlayerCharacter::ATQPlayerCharacter()
{
    // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = false;

    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = false;
    bUseControllerRotationRoll = false;

    if (const TObjectPtr<UCharacterMovementComponent> MoveComp = GetCharacterMovement())
    {
        MoveComp->bOrientRotationToMovement = true; // Character automatically faces moving vector
        MoveComp->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // Smooth turning rate
    }
}

// Called when the game starts or when spawned
void ATQPlayerCharacter::BeginPlay()
{
    Super::BeginPlay();
}

// Called every frame
void ATQPlayerCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ATQPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ATQPlayerCharacter::MoveHandler(const FVector2D& MovementVector)
{
    if (Controller != nullptr)
    {
        // Find which way is forward relative to camera viewing angles
        const FRotator Rotation = Controller->GetControlRotation();
        const FRotator YawRotation(0, Rotation.Yaw, 0);

        const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
        const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

        // Inject directional physical velocity vectors straight into character movement component
        AddMovementInput(ForwardDirection, MovementVector.Y);
        AddMovementInput(RightDirection, MovementVector.X);
    }
}

void ATQPlayerCharacter::LookHandler(const FVector2D& LookVector)
{
    AddControllerYawInput(LookVector.X);
    AddControllerPitchInput(LookVector.Y);
}

void ATQPlayerCharacter::JumpHandler()
{
    Jump();
}

void ATQPlayerCharacter::JumpingStopHandler()
{
    StopJumping();
}
