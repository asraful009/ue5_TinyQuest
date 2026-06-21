# Main Menu System

## Overview

The Main Menu is the first scene presented to the player when the game starts. It provides a simple user interface that allows the player to:

* Start a new game.
* Open the options/settings menu.
* Exit the application.

The menu is implemented using Unreal Engine's UMG (Unreal Motion Graphics) system and follows a clean separation of responsibilities:

| Class                     | Responsibility                                     |
| ------------------------- | -------------------------------------------------- |
| `ATQMenuGameMode`         | Configures the menu-specific gameplay environment. |
| `ATQMenuPlayerController` | Creates and manages the main menu widget.          |
| `UTQMainMenuWidget`       | Handles menu button interactions and user input.   |

---

## Scene Setup

The **MainMenu** map should be configured with:

* `ATQMenuGameMode` as the active GameMode.
* `ATQMenuPlayerController` as the PlayerController.
* No Pawn or Character is required.

When the map loads:

1. Unreal creates `ATQMenuGameMode`.
2. Unreal creates `ATQMenuPlayerController`.
3. The PlayerController creates the Main Menu Widget.
4. Mouse input is enabled.
5. The player can interact with the UI.

---

## UI Layout

The menu widget contains three buttons:

### Play Button

Starts the game by loading the configured gameplay map.

### Options Button

Reserved for future settings functionality such as:

* Audio volume
* Graphics quality
* Key bindings
* Language selection

### Exit Button

Closes the game application.

---

# UTQMainMenuWidget

`UTQMainMenuWidget` is responsible for handling all menu interactions.

## Widget Binding

The widget automatically binds to buttons created inside the UMG Blueprint.

```c++
UPROPERTY(meta = (BindWidget))
TObjectPtr<UButton> PlayButton;
```

The same pattern is used for the Options and Exit buttons.

---

## NativeConstruct

When the widget is added to the viewport, `NativeConstruct()` is called.

Button click delegates are registered here:

```c++
PlayButton->OnClicked.AddDynamic(
    this,
    &UTQMainMenuWidget::OnPlayButtonClicked
);
```

This ensures the appropriate callback is executed whenever the player presses a button.

---

## NativeDestruct

When the widget is removed or destroyed, all delegates are unbound:

```c++
PlayButton->OnClicked.RemoveAll(this);
```

This prevents invalid references and keeps the UI lifecycle clean.

---

## Play Button Flow

When the Play button is clicked:

```text
Play Button
    ↓
UTQMainMenuWidget
    ↓
ATQMenuPlayerController
    ↓
GetPlayLevel()
    ↓
OpenLevelBySoftObjectPtr()
    ↓
Gameplay Map Loads
```

Implementation:

```c++
UGameplayStatics::OpenLevelBySoftObjectPtr(
    this,
    PC->GetPlayLevel()
);
```

The current menu map is unloaded and the gameplay map is loaded.

---

## Exit Button Flow

The Exit button calls:

```c++
UKismetSystemLibrary::QuitGame(...)
```

which closes the application.

---

# ATQMenuPlayerController

The Menu PlayerController manages all menu-related UI.

## Responsibilities

* Create the menu widget.
* Add the widget to the viewport.
* Show the mouse cursor.
* Enable UI-only input mode.
* Store the gameplay map reference.

---

## Gameplay Map Reference

The gameplay level is configured in the editor using a soft reference:

```c++
UPROPERTY(EditDefaultsOnly)
TSoftObjectPtr<UWorld> PlayLevel;
```

Using a soft reference avoids hard dependencies and allows maps to be renamed or moved safely.

---

## Widget Creation

During `BeginPlay()`:

```c++
ActiveMenuWidget =
    CreateWidget<UTQMainMenuWidget>(
        this,
        MainMenuWidgetClass
    );
```

The widget is then displayed:

```c++
ActiveMenuWidget->AddToViewport();
```

---

## Input Configuration

The menu uses UI-only input mode:

```c++
FInputModeUIOnly InputModeData;
SetInputMode(InputModeData);
```

Benefits:

* Prevents gameplay input.
* Allows mouse interaction with widgets.
* Focuses keyboard navigation on the UI.

The mouse cursor is enabled:

```c++
bShowMouseCursor = true;
```

---

# ATQMenuGameMode

The Menu GameMode defines the environment used by the Main Menu map.

## Configuration

```c++
PlayerControllerClass =
    ATQMenuPlayerController::StaticClass();

DefaultPawnClass = nullptr;
```

No Pawn is spawned because the player only interacts with UI.

---

## Purpose

The Menu GameMode exists to provide a dedicated environment separate from gameplay.

Advantages:

* Keeps menu logic isolated.
* Prevents gameplay systems from loading unnecessarily.
* Allows menu-specific PlayerControllers and UI.

---

# Transition to Gameplay

When the Play button is pressed:

```text
MainMenu Map
├── ATQMenuGameMode
├── ATQMenuPlayerController
└── UTQMainMenuWidget
```

is destroyed and replaced by:

```text
Story_Map_01
├── ATQPlayGameMode
├── ATQPlayPlayerController
└── Gameplay Systems
```

The new map creates its own GameMode, PlayerController, HUD, widgets, and gameplay actors.

This clean separation ensures the Main Menu remains lightweight and independent from gameplay systems.


## Appendix: Full Source


**UTQMainMenuWidget.h**
```c++

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
    
    UFUNCTION()
    void SetMenuButtonsEnabled(const bool bNewEnabledState) const;
};

```

**TQMainMenuWidget.cpp**
```c++
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
        SetMenuButtonsEnabled(!bIsTransitioning);
        RemoveFromParent();
        UGameplayStatics::OpenLevelBySoftObjectPtr(
            this,
            PC->GetPlayLevel()
        );
    }
}

void UTQMainMenuWidget::OnOptionButtonClicked()
{
    // TODO : Create a Options
}

void UTQMainMenuWidget::OnExitButtonClicked()
{
    if (bIsTransitioning)
    {
        return;
    }

    bIsTransitioning = true;
    SetMenuButtonsEnabled(!bIsTransitioning);
    UKismetSystemLibrary::QuitGame(
        this,
        GetOwningPlayer(),
        EQuitPreference::Quit,
        true
    );
}

void UTQMainMenuWidget::SetMenuButtonsEnabled(const bool bNewEnabledState) const
{
    if (PlayButton) { PlayButton->SetIsEnabled(bNewEnabledState); }
    if (OptionButton) { OptionButton->SetIsEnabled(bNewEnabledState); }
    if (ExitButton) { ExitButton->SetIsEnabled(bNewEnabledState); }
}

```

**TQMenuPlayerController.h**
```c++

class UTQMainMenuWidget;
class UWorld;

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

protected:
    virtual void BeginPlay() override;

public:
    TSoftObjectPtr<UWorld> GetPlayLevel() const { return PlayLevel; }
};
```

**TQMenuPlayerController.cpp**
```c++

#include "TQMenuPlayerController.h"

#include "TQMainMenuWidget.h"

void ATQMenuPlayerController::BeginPlay()
{
    Super::BeginPlay();
    if (ActiveMenuWidget) return;
    if (MainMenuWidgetClass == nullptr)
    {
        return;
    }
    
    ActiveMenuWidget = CreateWidget<UTQMainMenuWidget>(this, MainMenuWidgetClass);
    ActiveMenuWidget->AddToViewport();
    bShowMouseCursor = true;
    FInputModeUIOnly InputModeData;
    InputModeData.SetWidgetToFocus(ActiveMenuWidget->TakeWidget());
    SetInputMode(InputModeData);
}
```


**TQMenuGameMode.h**
```c++
class UTQMainMenuWidget;

UCLASS()
class TINYQUEST_API ATQMenuGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    ATQMenuGameMode();

protected:
    virtual void BeginPlay() override;
};
```
**TQMenuGameMode.cpp**
```c++
#include "TQMenuGameMode.h"

#include "TQMenuPlayerController.h"

ATQMenuGameMode::ATQMenuGameMode()
{
    PlayerControllerClass = ATQMenuPlayerController::StaticClass();
    DefaultPawnClass = nullptr;
}

void ATQMenuGameMode::BeginPlay()
{
    Super::BeginPlay();
}

```