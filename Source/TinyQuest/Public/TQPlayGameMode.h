// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TQPlayGameMode.generated.h"

/**
 * 
 */
UCLASS()
class TINYQUEST_API ATQPlayGameMode : public AGameModeBase
{
    GENERATED_BODY()
    
    
public:
    ATQPlayGameMode();    
        
    virtual void BeginPlay() override;
};
