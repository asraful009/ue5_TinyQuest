// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TQPlayerCharacter.generated.h"

UCLASS()
class TINYQUEST_API ATQPlayerCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    // Sets default values for this character's properties
    ATQPlayerCharacter();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    // Called to bind functionality to input
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
    
    void MoveHandler(const FVector2D& MovementVector);
    void LookHandler(const FVector2D& LookVector);
    void JumpHandler();
    void JumpingStopHandler();
    
};
