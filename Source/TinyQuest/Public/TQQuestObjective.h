/**
* Single objective within a quest (e.g. "Collect 5 Herbs", "Talk to the Blacksmith").
 * A quest is complete once all required (non-optional) objectives are IsComplete.
 */
#pragma once

#include "CoreMinimal.h"

#include "GameplayTagContainer.h"
#include "TQQuestTypes.h"

#include "TQQuestObjective.generated.h"

USTRUCT(BlueprintType)
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

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TinyQuest|Quest Objective", meta = (ClampMin = "1"))
    int32 RequiredCount = 1;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "TinyQuest|Quest Objective")
    int32 CurrentCount = 0;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "TinyQuest|Quest Objective")
    bool IsOptional = false;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "TinyQuest|Quest Objective")
    bool IsComplete = false;

    bool AddProgress(const int32 Amount = 1)
    {
        if (IsComplete)
        {
            return false;
        }

        CurrentCount = FMath::Clamp(CurrentCount + Amount, 0, RequiredCount);

        if (CurrentCount >= RequiredCount)
        {
            IsComplete = true;
            return true;
        }
        return false;
    }

    void ResetProgress()
    {
        CurrentCount = 0;
        IsComplete = false;
    }
};
