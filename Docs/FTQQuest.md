# `FTQQuest`

> A quest — a named container of one or more objectives.

---

## Overview

`FTQQuest` is a **UStruct** that represents a single quest in the TinyQuest system. It holds both the design-time definition (name, description, objectives, prerequisites) and the runtime state (`State`) of a quest.

A quest is considered **complete** once every **required** (non-optional) objective reports `IsComplete == true`. Optional objectives are tracked but do not block completion.

Quests are typically authored as rows in a **Data Table** using `FTQQuest` as the row struct, and are loaded into the `UTQQuestManagerSubsystem` at runtime.

---

## Header

```cpp
#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "TQQuestObjective.h"
#include "TQQuestTypes.h"
#include "TQQuest.generated.h"

USTRUCT(BlueprintType)
struct FTQQuest
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TinyQuest|Quest")
    FName QuestID;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TinyQuest|Quest")
    FText QuestName;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TinyQuest|Quest")
    FText QuestDescription;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TinyQuest|Quest")
    FName PrerequisiteQuestID;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TinyQuest|Quest")
    TArray<FTQQuestObjective> Objectives;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "TinyQuest|Quest|Runtime")
    ETQQuestState State = ETQQuestState::NotStarted;

    bool AreRequiredObjectivesComplete() const;
    bool AreAllObjectivesComplete() const;
    int32 FindObjectiveIndex(ETQQuestType Type, const FGameplayTag& TargetTag) const;

    bool AddProgressToObjective(ETQQuestType Type, const FGameplayTag& TargetTag, int32 Amount = 1);
    void ResetQuest();
};
```

---

## Properties

### Design-Time

| Property | Type | Description |
|---|---|---|
| `QuestID` | `FName` | Unique identifier. Must match the row name in the quest Data Table. |
| `QuestName` | `FText` | Display name shown in the quest journal. |
| `QuestDescription` | `FText` | Long-form description shown in the quest journal. |
| `PrerequisiteQuestID` | `FName` | Another quest that must be Completed before this one can start. Leave `None` for no prerequisite. |
| `Objectives` | `TArray<FTQQuestObjective>` | All objectives that make up this quest. |
| `State` | `ETQQuestState` | Current lifecycle state. |

### Runtime

`State` is marked `VisibleAnywhere` (not `EditAnywhere`) because it represents runtime state, not design-time data. Only the `UTQQuestManagerSubsystem` should ever change it.

---

## State Machine

```
NotStarted ──► Active ──► Completed
                  │
                  └──► Canceled
```

| State | Meaning |
|---|---|
| `NotStarted` | Quest exists but has not been started. |
| `Active` | Quest is in progress. Objectives can receive progress. |
| `Canceled` | Quest was abandoned. Cannot receive progress. |
| `Completed` | All required objectives finished. Terminal state. |

**Rule:** Only the `UTQQuestManagerSubsystem` should flip `State`.

---

## Methods

### `AreRequiredObjectivesComplete() const → bool`

Returns `true` if every non-optional objective has `IsComplete == true`.

- Returns `false` if `State` is `NotStarted` or `Canceled`.
- Returns `true` if `State` is `Completed`.
- Otherwise scans objectives; ignores optional ones.

### `AreAllObjectivesComplete() const → bool`

Same as above, but includes optional objectives.

### `FindObjectiveIndex(ETQQuestType Type, const FGameplayTag& TargetTag) const → int32`

Returns the index of the first **incomplete** matching objective, or `INDEX_NONE`.

### `AddProgressToObjective(ETQQuestType Type, const FGameplayTag& TargetTag, int32 Amount = 1) → bool`

Applies progress to the first matching incomplete objective. Returns `true` if applied.

### `ResetQuest() → void`

Resets all runtime state back to initial values.

---

## Typical Usage

### Starting a quest

```cpp
FTQQuest NewQuest = QuestDatabase[QuestID];
NewQuest.ResetQuest();
NewQuest.State = ETQQuestState::Active;
ActiveQuests.Add(QuestID, MoveTemp(NewQuest));
```

### Reporting progress

```cpp
Quest->AddProgressToObjective(ETQQuestType::CollectObjects, ItemTag, 1);

if (Quest->AreRequiredObjectivesComplete())
{
    Quest->State = ETQQuestState::Completed;
}
```

---

## Design Notes

1. **`State` is runtime, not authored.** Visible in the editor but not editable.
2. **Objectives are ordered.** `FindObjectiveIndex` returns the first match.
3. **`AreRequiredObjectivesComplete` short-circuits on `Completed`.**
4. **Prerequisites are enforced by the manager, not the struct.**
5. **Structs describe, managers mutate.**

---

## Related Types

| Type | Purpose |
|---|---|
| `FTQQuestObjective` | A single objective within a quest. |
| `ETQQuestType` | Objective type enum. |
| `ETQQuestState` | Quest lifecycle state enum. |
| `UTQQuestManagerSubsystem` | Central authority for quest state. |