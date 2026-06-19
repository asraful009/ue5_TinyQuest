// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TQPlayerController.generated.h"

class UInputMappingContext;
struct FInputActionValue;
class UInputAction;
/**
 * 
 */
UCLASS()
class TINYQUEST_API ATQPlayerController : public APlayerController
{
    GENERATED_BODY()

protected:
    /** Input asset configurations mapped with modern UE5 TObjectPtr */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TQ|Input", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UInputMappingContext> Player3rdPersonIMC;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TQ|Input", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UInputAction> MoveAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TQ|Input", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UInputAction> LookAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TQ|Input", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UInputAction> JumpAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TQ|Input", meta = (AllowPrivateAccess = "true"))
    int32 MappingPriority = 0;

public:
    ATQPlayerController();

protected:
    virtual void BeginPlay() override;
    virtual void OnPossess(APawn* InPawn) override;

    virtual void SetupInputComponent() override;

    UFUNCTION()
    void HandleMovement(const FInputActionValue& Value);


};
