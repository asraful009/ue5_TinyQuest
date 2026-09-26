# TQQuest System — To-Do List

Based on `TQQuestTypes.h` (quest state/type enums already defined). Next steps to build out a working quest system.

## 1. Core Data Structures
- [ ] `FTQQuestObjective` struct — objective type (`ETQQuestType`), target actor/tag, required count, current progress
- [ ] `FTQQuestData` struct — quest ID, name, description, list of objectives, rewards
- [ ] Quest data table / data asset (`UDataTable` or `UPrimaryDataAsset`) for designer-authored quests

## 2. Core Classes
- [ ] `UTQQuestInstance` — runtime instance of a quest, holds `ETQQuestState`, tracks objective progress
- [ ] `UTQQuestComponent` — actor component (attach to PlayerState/Controller) managing active/completed quests
- [ ] `UTQQuestManagerSubsystem` (GameInstanceSubsystem) — central registry of all available quests, handles start/complete/cancel

## 3. Quest Lifecycle Logic
- [ ] `StartQuest(QuestID)` — transitions `NotStarted → Active`
- [ ] `CompleteQuest(QuestID)` — transitions `Active → Completed`, grants rewards
- [ ] `CancelQuest(QuestID)` — transitions `Active → Canceled`
- [ ] Objective progress tracking per `ETQQuestType`:
  - [ ] `FindObject` — overlap/interact detection
  - [ ] `TalkToCharacter` — dialogue system hook
  - [ ] `GoToLocation` — trigger volume / distance check
  - [ ] `CollectObjects` — inventory pickup hook
  - [ ] `InteractWithObject` — interaction system hook

## 4. Events & Delegates
- [ ] `OnQuestStateChanged(QuestID, OldState, NewState)`
- [ ] `OnQuestObjectiveUpdated(QuestID, ObjectiveIndex, Progress)`
- [ ] `OnQuestCompleted(QuestID)` / `OnQuestCanceled(QuestID)`

## 5. Blueprint Exposure
- [ ] `BlueprintCallable` functions on the manager/component for Start/Complete/Cancel/GetState
- [ ] `BlueprintImplementableEvent` hooks for UI updates (quest log, HUD markers)
- [ ] Editor-friendly quest data asset with validation (`IsDataValid` override)

## 6. UI
- [ ] Quest log widget (list active/completed/canceled quests)
- [ ] Objective tracker HUD widget (current active quest + progress)
- [ ] Notification/toast on quest state change

## 7. Save/Load
- [ ] Serialize quest states + objective progress into save game
- [ ] Restore quest manager state on load

## 8. Testing
- [ ] Automation tests for state transitions (illegal transitions rejected, e.g. Completed → Active)
- [ ] Test each objective type in a sandbox map

## 9. Polish
- [ ] Designer-facing tooltips/comments for all new UPROPERTYs
- [ ] Consistent naming convention audit (`TQ` prefix)
- [ ] Documentation pass (update this doc + code comments)
