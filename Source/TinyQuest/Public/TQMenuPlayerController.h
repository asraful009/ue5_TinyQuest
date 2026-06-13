// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TQMenuPlayerController.generated.h"

class UTQMainMenuWidget;
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
    UPROPERTY()
    TObjectPtr<UTQMainMenuWidget> ActiveMenuWidget;
    
public:
    virtual void BeginPlay() override;
};
