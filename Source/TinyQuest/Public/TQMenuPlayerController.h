// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TQMenuPlayerController.generated.h"

class UTQMainMenuWidget;
class UWorld;
/**
 * 
 */
UCLASS()
class TINYQUEST_API ATQMenuPlayerController : public APlayerController
{
    GENERATED_BODY()

public:
    UPROPERTY(EditDefaultsOnly, Category = "TQ|UI")
    TSubclassOf<UTQMainMenuWidget> MainMenuWidgetClass;

private:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="TQ|Menu", meta=(AllowPrivateAccess=true))
    TSoftObjectPtr<UWorld> PlayLevel;

    UPROPERTY()
    TObjectPtr<UTQMainMenuWidget> ActiveMenuWidget;

public:
    virtual void BeginPlay() override;

    TSoftObjectPtr<UWorld> GetPlayLevel() const { return PlayLevel; }
};
