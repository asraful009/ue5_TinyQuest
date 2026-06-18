// Fill out your copyright notice in the Description page of Project Settings.


#include "TQMainMenuWidget.h"

#include "TQMenuPlayerController.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

void UTQMainMenuWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (PlayButton)
    {
        PlayButton->OnClicked.AddDynamic(this, &UTQMainMenuWidget::OnPlayButtonClicked);
    }

    if (OptionButton)
    {
        OptionButton->OnClicked.AddDynamic(this, &UTQMainMenuWidget::OnOptionButtonClicked);
    }

    if (ExitButton)
    {
        ExitButton->OnClicked.AddDynamic(this, &UTQMainMenuWidget::OnExitButtonClicked);
    }
}

void UTQMainMenuWidget::NativeDestruct()
{
    // Clean up delegate assignments on destruction to prevent memory leaks
    if (PlayButton) { PlayButton->OnClicked.RemoveAll(this); }
    if (OptionButton) { OptionButton->OnClicked.RemoveAll(this); }
    if (ExitButton) { ExitButton->OnClicked.RemoveAll(this); }
    Super::NativeDestruct();
}

void UTQMainMenuWidget::OnPlayButtonClicked()
{
    if (bIsTransitioning)
    {
        return;
    }
    const ATQMenuPlayerController* PC =
        Cast<ATQMenuPlayerController>(GetOwningPlayer());

    if (PC)
    {
        bIsTransitioning = true;
        if (PlayButton)   { PlayButton->SetIsEnabled(false); }
        if (OptionButton) { OptionButton->SetIsEnabled(false); }
        if (ExitButton)   { ExitButton->SetIsEnabled(false); }
        
        RemoveFromParent();
        UGameplayStatics::OpenLevelBySoftObjectPtr(
            this,
            PC->GetPlayLevel()
        );
    }
}

void UTQMainMenuWidget::OnOptionButtonClicked()
{
}

void UTQMainMenuWidget::OnExitButtonClicked()
{
    if (bIsTransitioning)
    {
        return;
    }
 
    bIsTransitioning = true;
    if (PlayButton)   { PlayButton->SetIsEnabled(false); }
    if (OptionButton) { OptionButton->SetIsEnabled(false); }
    if (ExitButton)   { ExitButton->SetIsEnabled(false); }
    
    UKismetSystemLibrary::QuitGame(
        this,
        GetOwningPlayer(),
        EQuitPreference::Quit,
        true
    );
}
