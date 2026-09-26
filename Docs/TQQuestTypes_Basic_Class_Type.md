# TQQuestTypes.h

Header file defining core quest-related enums for the Unreal Engine quest system module (`TQ`).

## Overview

| Enum | Purpose |
|------|---------|
| `ETQQuestState` | Tracks the current lifecycle state of a quest. |
| `ETQQuestType` | Defines the category/objective type of a quest. |

Both enums are `BlueprintType`, so they're exposed to Blueprints and can be used as UPROPERTY/UFUNCTION parameters.

## Code

```cpp
/**
 *
 */
#pragma once

#include "CoreMinimal.h"
#include "TQQuestTypes.generated.h"

UENUM(BlueprintType)
enum class ETQQuestState : uint8
{
    NotStarted UMETA(DisplayName = "Not Started", ToolTip = "The quest has not started yet."),
    Active UMETA(DisplayName = "Active", ToolTip = "The quest is currently active."),
    Canceled UMETA(DisplayName = "Canceled", ToolTip = "The quest has been canceled."),
    Completed UMETA(DisplayName = "Completed", ToolTip = "The quest has been completed.")
};

UENUM(BlueprintType)
enum class ETQQuestType : uint8
{
    FindObject UMETA(DisplayName = "Find Object", ToolTip = "Find a specific object in the world."),
    TalkToCharacter UMETA(DisplayName = "Talk To Character", ToolTip = "Talk to a specific character."),
    GoToLocation UMETA(DisplayName = "Go To Location", ToolTip = "Reach a specific location."),
    CollectObjects UMETA(DisplayName = "Collect Objects", ToolTip = "Collect one or more objects."),
    InteractWithObject UMETA(DisplayName = "Interact With Object", ToolTip = "Interact with a specific object.")
};
```

## Enum Reference

### `ETQQuestState`

| Value | Display Name | Description |
|-------|--------------|-------------|
| `NotStarted` | Not Started | The quest has not started yet. |
| `Active` | Active | The quest is currently active. |
| `Canceled` | Canceled | The quest has been canceled. |
| `Completed` | Completed | The quest has been completed. |

### `ETQQuestType`

| Value | Display Name | Description |
|-------|--------------|-------------|
| `FindObject` | Find Object | Find a specific object in the world. |
| `TalkToCharacter` | Talk To Character | Talk to a specific character. |
| `GoToLocation` | Go To Location | Reach a specific location. |
| `CollectObjects` | Collect Objects | Collect one or more objects. |
| `InteractWithObject` | Interact With Object | Interact with a specific object. |
