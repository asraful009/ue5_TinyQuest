# TQQuestTypes

## Purpose

`TQQuestTypes.h` contains the shared enum types used by the TinyQuest
quest system.

It does not contain quest logic. It only defines the basic types that
other quest classes can use.

## File

``` text
Source/
└── TinyQuest/
    └── Quest/
        └── TQQuestTypes.h
```

## Basic Header

``` cpp
#pragma once

#include "CoreMinimal.h"
#include "TQQuestTypes.generated.h"
```

Because this file contains Unreal reflection types such as `UENUM`, it
includes the generated header.

------------------------------------------------------------------------

# ETQQuestType

`ETQQuestType` defines what kind of quest the player is doing.

``` cpp
UENUM(BlueprintType)
enum class ETQQuestType : uint8
{
    FindObject
        UMETA(
            DisplayName = "Find Object",
            ToolTip = "Find a specific object in the world."
        ),

    TalkToCharacter
        UMETA(
            DisplayName = "Talk To Character",
            ToolTip = "Talk to a specific character."
        ),

    GoToLocation
        UMETA(
            DisplayName = "Go To Location",
            ToolTip = "Reach a specific location."
        ),

    CollectObjects
        UMETA(
            DisplayName = "Collect Objects",
            ToolTip = "Collect one or more objects."
        ),

    InteractWithObject
        UMETA(
            DisplayName = "Interact With Object",
            ToolTip = "Interact with a specific object."
        )
};
```

## Quest Types

  -----------------------------------------------------------------------
  C++ Type                Editor Name             Purpose
  ----------------------- ----------------------- -----------------------
  `FindObject`            Find Object             Find a specific object

  `TalkToCharacter`       Talk To Character       Talk to an NPC

  `GoToLocation`          Go To Location          Reach a location

  `CollectObjects`        Collect Objects         Collect one or more
                                                  objects

  `InteractWithObject`    Interact With Object    Interact with an object
  -----------------------------------------------------------------------

------------------------------------------------------------------------

# ETQQuestState

`ETQQuestState` defines the current state of a quest.

``` cpp
UENUM(BlueprintType)
enum class ETQQuestState : uint8
{
    NotStarted
        UMETA(
            DisplayName = "Not Started",
            ToolTip = "The quest has not started yet."
        ),

    Active
        UMETA(
            DisplayName = "Active",
            ToolTip = "The quest is currently active."
        ),

    Completed
        UMETA(
            DisplayName = "Completed",
            ToolTip = "The quest has been completed."
        )
};
```

## Quest States

``` text
NotStarted
    ↓
Active
    ↓
Completed
```

A quest normally starts as:

``` cpp
ETQQuestState::NotStarted
```

When the player accepts or starts it:

``` cpp
ETQQuestState::Active
```

When the objective is completed:

``` cpp
ETQQuestState::Completed
```

------------------------------------------------------------------------

# Complete TQQuestTypes.h

``` cpp
#pragma once

#include "CoreMinimal.h"
#include "TQQuestTypes.generated.h"


UENUM(BlueprintType)
enum class ETQQuestType : uint8
{
    FindObject
        UMETA(
            DisplayName = "Find Object",
            ToolTip = "Find a specific object in the world."
        ),

    TalkToCharacter
        UMETA(
            DisplayName = "Talk To Character",
            ToolTip = "Talk to a specific character."
        ),

    GoToLocation
        UMETA(
            DisplayName = "Go To Location",
            ToolTip = "Reach a specific location."
        ),

    CollectObjects
        UMETA(
            DisplayName = "Collect Objects",
            ToolTip = "Collect one or more objects."
        ),

    InteractWithObject
        UMETA(
            DisplayName = "Interact With Object",
            ToolTip = "Interact with a specific object."
        )
};


UENUM(BlueprintType)
enum class ETQQuestState : uint8
{
    NotStarted
        UMETA(
            DisplayName = "Not Started",
            ToolTip = "The quest has not started yet."
        ),

    Active
        UMETA(
            DisplayName = "Active",
            ToolTip = "The quest is currently active."
        ),

    Completed
        UMETA(
            DisplayName = "Completed",
            ToolTip = "The quest has been completed."
        )
};
```

# Basic Usage

Other TQ quest classes can include the type header:

``` cpp
#include "Quest/TQQuestTypes.h"
```

Then use the enums:

``` cpp
ETQQuestType QuestType = ETQQuestType::FindObject;

ETQQuestState QuestState = ETQQuestState::NotStarted;
```

## First TQ Quest

Our first playable quest will use:

``` cpp
QuestType  = ETQQuestType::FindObject;
QuestState = ETQQuestState::NotStarted;
```

Example:

``` text
Quest: Find Grandma's Key

Type:
    Find Object

State:
    Not Started
```

When the player starts the quest:

``` text
Not Started
     ↓
Active
```

When the player finds the key:

``` text
Active
   ↓
Completed
```
