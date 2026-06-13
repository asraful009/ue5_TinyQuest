// Fill out your copyright notice in the Description page of Project Settings.


#include "TQMainMenuWidget.h"

#include "Components/Button.h"
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
        ExitButton->OnClicked.AddDynamic(this, &UTQMainMenuWidget::OnExitButtonClicked);
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
}

void UTQMainMenuWidget::OnOptionButtonClicked()
{
}

void UTQMainMenuWidget::OnExitButtonClicked()
{
    UKismetSystemLibrary::QuitGame(
        this,
        GetOwningPlayer(),
        EQuitPreference::Quit,
        true
    );
}
