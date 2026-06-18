// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TQMainMenuWidget.generated.h"

class UButton;

/**
 * 
 */
UCLASS()
class TINYQUEST_API UTQMainMenuWidget : public UUserWidget
{
    GENERATED_BODY()

    
protected:
    virtual void NativeConstruct() override;
    virtual void NativeDestruct() override;
 
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> PlayButton;
    
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> OptionButton;
    
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> ExitButton;
    
private:
    bool bIsTransitioning = false;
    
private:
    UFUNCTION()
    void OnPlayButtonClicked();
    
    UFUNCTION()
    void OnOptionButtonClicked();
    
    UFUNCTION()
    void OnExitButtonClicked();
};
