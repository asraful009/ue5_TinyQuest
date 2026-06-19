// Fill out your copyright notice in the Description page of Project Settings.


#include "TQPlayerCharacter.h"

// Sets default values
ATQPlayerCharacter::ATQPlayerCharacter()
{
    // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = false;
    
    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = false;
    bUseControllerRotationRoll = false;
    
    
    
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

void ATQPlayerCharacter::Move(const FVector2D& MovementVector)
{
    
}
