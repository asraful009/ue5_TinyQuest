// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TQMenuGameMode.generated.h"

class UTQMainMenuWidget;
/**
 * 
 */
UCLASS()
class TINYQUEST_API ATQMenuGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    ATQMenuGameMode();

protected:
    virtual void BeginPlay() override;
};
