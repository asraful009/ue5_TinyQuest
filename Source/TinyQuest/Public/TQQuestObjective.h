/**
 *
 */
#pragma once
#include "GameplayTagContainer.h"
#include "TQQuestTypes.h"

/**
 * Defines a single objective within a quest (e.g. "Collect 5 Herbs", "Talk to the Blacksmith").
 * A quest can contain one or more of these; the quest is considered complete once all
 * required objectives report bCompleted = true.
 */
UCLASS(BlueprintType)
struct FTQQuestObjective
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TinyQuest|Quest Objective")
    ETQQuestType QuestObjectiveType;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TinyQuest|Quest Objective")
    FText QuestObjectiveName;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TinyQuest|Quest Objective")
    FText QuestObjectiveDescription;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TinyQuest|Quest Objective")
    FGameplayTag TargetTag;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TinyQuest|Quest Objective")
    int32 RequiredCount = 1;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TinyQuest|Quest Objective")
    int32 CurrentCount = 0;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TinyQuest|Quest Objective")
    bool IsOptional = false;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TinyQuest|Quest Objective")
};
